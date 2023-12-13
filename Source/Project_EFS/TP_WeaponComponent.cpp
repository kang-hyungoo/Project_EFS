// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "Project_EFSCharacter.h"
#include "Project_EFSProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
	FireRate = 0.25f;

	IsAutoShoot = true;
}


void UTP_WeaponComponent::Fire()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Find not character"));
		return;
	}

	IsFire = true;

	// Try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			auto timeElapsed = FireTimerHandle.IsValid() ? World->GetTimerManager().GetTimerElapsed(FireTimerHandle) : -1.0f;

			if (0.0f < timeElapsed && timeElapsed < FireRate)
			{
				UE_LOG(LogTemp, Warning, TEXT("Already fire"));
				return;
			}

			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
	
			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	
			// Spawn the projectile at the muzzle
			World->SpawnActor<AProject_EFSProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

			// Fire Delay
			World->GetTimerManager().SetTimer(FireTimerHandle, this, &UTP_WeaponComponent::FireDelay, FireRate, false);

			// Try and play the sound if specified
			if (FireSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
			}

			// Try and play a firing animation if specified
			if (FireAnimation != nullptr)
			{
				// Get the animation object for the arms mesh
				UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
				if (AnimInstance != nullptr)
				{
					AnimInstance->Montage_Play(FireAnimation, 1.f);
				}
			}
		}
	}
}

void UTP_WeaponComponent::StopFire()
{
	IsFire = false;
}

void UTP_WeaponComponent::FireDelay()
{
	if (!IsFire)
	{
		UE_LOG(LogTemp, Warning, TEXT("StopFire"));
		if (FireTimerHandle.IsValid())
			FireTimerHandle.Invalidate();

		return;
	}

	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		if (IsAutoShoot && World->GetTimerManager().IsTimerActive(FireTimerHandle))
			Fire();
	}
}

void UTP_WeaponComponent::ChangeMode()
{
	UE_LOG(LogTemp, Warning, TEXT("ChangeMode Start"));

	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		if (World->GetTimerManager().IsTimerActive(FireTimerHandle))
			return;
	}

	UE_LOG(LogTemp, Warning, TEXT("ChangeMode"));

	IsAutoShoot = !IsAutoShoot;
}

void UTP_WeaponComponent::AttachWeapon(AProject_EFSCharacter* TargetCharacter)
{
	Character = TargetCharacter;
	if (Character == nullptr)
	{
		return;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));
	
	// switch bHasRifle so the animation blueprint can switch to another animation set
	Character->SetHasRifle(true);

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::Fire);
			EnhancedInputComponent->BindAction(StopFireAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::StopFire);
			EnhancedInputComponent->BindAction(ChageModeAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::ChangeMode);
		}
	}
}

void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}