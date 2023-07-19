// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemActorComponent.h"

// Sets default values for this component's properties
UItemActorComponent::UItemActorComponent()
{
	itemSize.Get<0>() = 3;
	itemSize.Get<1>() = 3;

	itemRotation = false;
}

void UItemActorComponent::PushItem(TTuple<int, int> itemStoragePosition, bool itemStorageRosition)
{
	itemPosition = itemStoragePosition;
	itemRotation = itemStorageRosition;
}

void UItemActorComponent::PullItem()
{
	itemPosition.Get<0>() = -1;
	itemPosition.Get<1>() = -1;

	itemRotation = false;
}