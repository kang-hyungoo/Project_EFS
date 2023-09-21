// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameStruct.h"
#include "ItemActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_EFS_API UItemActorComponent : public UActorComponent
{
	GENERATED_BODY()

	// value
public:
	TTuple<int, int> itemSize;

	TTuple<int, int> itemPosition;

	bool itemRotation;

	// function
public:	
	// Sets default values for this component's properties
	UItemActorComponent();

	void Init(FItemStruct itemData);

	void PushItem(TTuple<int, int> itemStoragePosition, bool itemStorageRosition);
	void PullItem();

protected:


};
