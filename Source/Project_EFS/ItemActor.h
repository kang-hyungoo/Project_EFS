// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameStruct.h"
#include "ItemActor.generated.h"

UCLASS()
class PROJECT_EFS_API AItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemActor();

	void Init(FItemStruct itemData);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
