// Fill out your copyright notice in the Description page of Project Settings.


#include "StorageComponent.h"
#include "Engine/DataTable.h"
#include "../Base/BaseItem.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UStorageComponent::UStorageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStorageComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UStorageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStorageComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UStorageComponent, Capacity);
	DOREPLIFETIME(UStorageComponent, SlotsFilled);
	DOREPLIFETIME(UStorageComponent, Items);
}

void UStorageComponent::OnRep_StorageUpdated()
{
	OnInventoryUpdated.Broadcast();
}

void UStorageComponent::UpdateUI()
{
	OnInventoryUpdated.Broadcast();
}

int UStorageComponent::GetFirstEmpty()
{
	for (FInventoryItem a : Items)
	{
		if (a.IsEmpty)
		{
			return a.Index;
		}
	}
	return -1;
}

int UStorageComponent::AddEmptyAtIndex(int index)
{
	FInventoryItem empty = FInventoryItem();
	empty.Index = index;
	empty.IsEmpty = true;
	empty.ItemOwner = this;
	Items.Insert(empty, index);
	UpdateUI();
	return index;
}

bool UStorageComponent::AddItem(FInventoryItem item)
{
	if (SlotsFilled >= Capacity)
	{
		OnInventoryFull.Broadcast();
		return false;
	}

	// process if the item is stackable
	if (dataTable)
	{
		FItemRow* ItemRow = dataTable->FindRow<FItemRow>(item.UniqueName, "");

		int pendingStackSize = item.StackSize;

		if (ItemRow->IsStackable)
		{
			for (FInventoryItem& a : Items)
			{
				if (a == item)
				{
					if (!ItemRow->HasMaxStackSize)
					{
						a.StackSize += pendingStackSize;
						UpdateUI();
						return true;
					}
					else
					{
						if (a.StackSize < ItemRow->MaxStackSize)
						{
							int oldStackSize = a.StackSize;

							// overflows max stack size, we want to max out the stack size
							// and subtract from the pending stack size and continue
							if (oldStackSize + pendingStackSize > ItemRow->MaxStackSize)
							{
								int diff = (oldStackSize + pendingStackSize) - ItemRow->MaxStackSize;
								a.StackSize = ItemRow->MaxStackSize;
								pendingStackSize = diff;
							}
							else
							{
								a.StackSize += pendingStackSize;
								pendingStackSize = 0;
							}
						}
					}
				}
				if (pendingStackSize == 0)
				{
					UpdateUI();
					return true;
				}
			}
		}

		int newIndex = GetFirstEmpty();
		Items.RemoveAt(newIndex);

		item.Index = newIndex;
		item.StackSize = pendingStackSize;
		item.ItemOwner = this;

		Items.Insert(item, newIndex);
		SlotsFilled++;

		UpdateUI();
		return true;
	}
	
	return false;
}

bool UStorageComponent::RemoveItem(FInventoryItem item)
{
	int index = item.Index;
	if (item.Index > -1)
	{
		Items.RemoveAt(index);

		// AddEmptyAtIndex(Index);
		SlotsFilled--;
		UpdateUI();
		return true;
	}
	return false;
}

bool UStorageComponent::HasItem(FName uniqueName, int stackSize)
{
	int pendingCount = stackSize;
	for (FInventoryItem& a : Items)
	{
		if (a.UniqueName == uniqueName)
		{
			pendingCount -= a.StackSize;
			if (pendingCount <= 0)
			{
				return true;
			}
		}
	}

	return false;
}

bool UStorageComponent::RemoveItemStack(FName uniqueName, int stackSize)
{
	int pendingCount = stackSize;

	for (FInventoryItem& a : Items)
	{
		if (a.UniqueName == uniqueName)
		{
			if (a.StackSize <= pendingCount)
			{
				pendingCount -= a.StackSize;
				RemoveItem(a);
			}
			else
			{
				a.StackSize -= pendingCount;
				pendingCount = 0;
			}
		}
		if (pendingCount <= 0)
		{
			UpdateUI();
			return true;
		}
	}
	return false;
}

void UStorageComponent::ServerAddBPItem_Implementation(FInventoryItem item)
{
	AddItem(item);
}

void UStorageComponent::ServerRemoveBPItem_Implementation(FInventoryItem item)
{
	RemoveItem(item);
}

bool UStorageComponent::BPHasItem(FName uniqueName, int stackSize)
{
	return HasItem(uniqueName, stackSize);
}

void UStorageComponent::ServerRemoveBPItemStack_Implementation(FName uniqueName, int stackSize)
{
	RemoveItemStack(uniqueName, stackSize);
}

