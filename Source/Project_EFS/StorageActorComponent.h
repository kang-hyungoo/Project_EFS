// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemActorComponent.h"
#include "StorageActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_EFS_API UStorageActorComponent : public UActorComponent
{
	GENERATED_BODY()

	// value
public:
	TTuple<int, int> storageSize;
	TTuple<int, int> itemTempPosition;

	TArray<bool> storageTwoDimensionalArray;

	TMap<TTuple<int, int>, UItemActorComponent*> storageItems;


	// function
public:	
	// Sets default values for this component's properties
	UStorageActorComponent();

	void AddItem(UItemActorComponent* item);
	void RemoveItem(UItemActorComponent* item);

	void PushStorage(TTuple<int, int> itemPosition, TTuple<int, int> itemSize, bool itemRotation);
	void PullStorage(TTuple<int, int> itemPosition, TTuple<int, int> itemSize, bool itemRotation);

	bool HasFreeSpace(UItemActorComponent* item);

	bool IsEmpty(int item_width, int item_height);
	bool IsEmpty(TTuple<int, int> itemSize);


protected:

};
