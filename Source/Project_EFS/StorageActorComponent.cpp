// Fill out your copyright notice in the Description page of Project Settings.


#include "StorageActorComponent.h"

// Sets default values for this component's properties
UStorageActorComponent::UStorageActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	storageSize.Get<0>() = 10;
	storageSize.Get<1>() = 10;

	int storage_width = storageSize.Get<0>();
	int storage_height = storageSize.Get<1>();

	storageTwoDimensionalArray.Init(false, storage_width * storage_height);

	// ...
}

void UStorageActorComponent::AddItem(UItemActorComponent* item)
{

	int item_width = item->itemSize.Get<0>();
	int item_height = item->itemSize.Get<1>();

	if (IsEmpty(item_width, item_height))
	{
		storageItems.Add(itemTempPosition, item);

		PushStorage(itemTempPosition, item->itemSize, false);

		item->PushItem(itemTempPosition, false);

		return;
	}

	int temp = item_width;
	item_width = item_height;
	item_height = temp;

	if (IsEmpty(item_width, item_height))
	{
		storageItems.Add(itemTempPosition, item);

		PushStorage(itemTempPosition, item->itemSize, true);

		item->PushItem(itemTempPosition, true);

		return;
	}
}

void UStorageActorComponent::RemoveItem(UItemActorComponent* item)
{
	auto tempItem = storageItems.Find(item->itemPosition);

	if (nullptr != tempItem)
	{
		storageItems.Remove(item->itemPosition);

		PullStorage(item->itemPosition, item->itemSize, item->itemRotation);

		item->PullItem();
	}
}

void UStorageActorComponent::PushStorage(TTuple<int, int> itemPosition, TTuple<int, int> itemSize, bool itemRotation)
{
	int storage_width = storageSize.Get<0>();

	int itemPosition_x = itemPosition.Get<0>();
	int itemPosition_y = itemPosition.Get<1>();

	int item_width;
	int item_height;

	if (itemRotation)
	{
		item_width = itemSize.Get<1>();
		item_height = itemSize.Get<0>();
	}
	else
	{
		item_width = itemSize.Get<0>();
		item_height = itemSize.Get<1>();
	}

	int left_top = storage_width * itemPosition_y + itemPosition_x;

	for (int n = 0; n < item_width; ++n)
	{
		for (int m = 0; m < item_height; ++m)
			storageTwoDimensionalArray[left_top + n + m * item_width] = true;
	}
}

void UStorageActorComponent::PullStorage(TTuple<int, int> itemPosition, TTuple<int, int> itemSize, bool itemRotation)
{
	int storage_width = storageSize.Get<0>();

	int itemPosition_x = itemPosition.Get<0>();
	int itemPosition_y = itemPosition.Get<1>();

	int item_width;
	int item_height;

	if (itemRotation)
	{
		item_width = itemSize.Get<1>();
		item_height = itemSize.Get<0>();
	}
	else
	{
		item_width = itemSize.Get<0>();
		item_height = itemSize.Get<1>();
	}

	int left_top = storage_width * itemPosition_y + itemPosition_x;

	for (int n = 0; n < item_width; ++n)
	{
		for (int m = 0; m < item_height; ++m)
			storageTwoDimensionalArray[left_top + n + m * item_width] = false;
	}
}

bool UStorageActorComponent::HasFreeSpace(UItemActorComponent* item)
{
	int item_width = item->itemSize.Get<0>();
	int item_height = item->itemSize.Get<1>();

	if (IsEmpty(item_width, item_height))
		return true;

	int temp = item_width;
	item_width = item_height;
	item_height = temp;

	if (IsEmpty(item_width, item_height))
		return true;

	return false;
}

bool UStorageActorComponent::IsEmpty(int item_width, int item_height)
{
	int storage_width = storageSize.Get<0>();
	int storage_height = storageSize.Get<1>();

	int research_width = storage_width - item_width;
	int research_height = storage_height - item_height;

	int left_top = 0;
	
	for (int i = 0; i < research_width; ++i)
	{
		for (int j = 0; j < research_height; ++j)
		{
			left_top = i + j * storage_width;

			if (storageTwoDimensionalArray[left_top]) continue;

			for (int n = 0; n < item_width; ++n)
			{
				for (int m = 0; m < item_height; ++m)
				{
					if (storageTwoDimensionalArray[left_top + n + m * research_width]) 
						goto SKIP;

					if (n == item_width - 1 && m == item_height - 1)
					{
						itemTempPosition.Get<0>() = i;
						itemTempPosition.Get<1>() = j;

						return true;
					}
				}
			}

			SKIP: {}
		}
	}

	return false;
}

bool UStorageActorComponent::IsEmpty(TTuple<int, int> itemSize)
{
	int storage_width = storageSize.Get<0>();
	int storage_height = storageSize.Get<1>();

	int item_width = itemSize.Get<0>();
	int item_height = itemSize.Get<1>();

	int research_width = storage_width - item_width;
	int research_height = storage_height - item_height;

	int left_top = 0;

	for (int i = 0; i < research_width; ++i)
	{
		for (int j = 0; j < research_height; ++j)
		{
			left_top = i + j * storage_width;

			if (storageTwoDimensionalArray[left_top]) continue;

			for (int n = 0; n < item_width; ++n)
			{
				for (int m = 0; m < item_height; ++m)
				{
					if (storageTwoDimensionalArray[left_top + n + m * research_width])
						goto SKIP;

					if (n == item_width - 1 && m == item_height - 1)
					{
						itemTempPosition.Get<0>() = i;
						itemTempPosition.Get<1>() = j;

						return true;
					}
				}
			}

		SKIP: {}
		}
	}

	return false;
}
