#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>
#include "../Components/StorageComponent.h"

#include "SharedStorage.generated.h"

UCLASS()
class INVENTORYPLUGIN_API ASharedStorage : public AActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	ASharedStorage(const FObjectInitializer& ObjectInitializer);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Category = Inventory)
	class UStorageComponent* StorageComponent;

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = Inventory)
	void ServerAddBPItem(FInventoryItem item);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = Inventory)
	void ServerRemoveBPItem(FInventoryItem item);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = Inventory)
	void ServerRemoveBPItemStack(FName uniqueName, int stackSize);
	
};