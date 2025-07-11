
#include "SharedStorage.h"
#include "Net/UnrealNetwork.h"

ASharedStorage::ASharedStorage(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SetReplicates(true);

	StorageComponent = CreateDefaultSubobject<UStorageComponent>("StorageComponent");
	StorageComponent->SetIsReplicated(true);
	StorageComponent->Capacity = 20;
}

void ASharedStorage::BeginPlay()
{
	Super::BeginPlay();
}

void ASharedStorage::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASharedStorage, StorageComponent);
}

void ASharedStorage::ServerAddBPItem_Implementation(FInventoryItem item)
{
	StorageComponent->AddItem(item);
}

void ASharedStorage::ServerRemoveBPItem_Implementation(FInventoryItem item)
{
	StorageComponent->RemoveItem(item);
}

void ASharedStorage::ServerRemoveBPItemStack_Implementation(FName uniqueName, int stackSize)
{
	StorageComponent->RemoveItemStack(uniqueName, stackSize);
}
