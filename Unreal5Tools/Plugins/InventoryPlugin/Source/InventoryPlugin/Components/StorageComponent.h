// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Delegates/DelegateCombinations.h"
#include "../Base/InventoryItem.h"
#include "StorageComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStorageCompUpdated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStorageCompFull);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYPLUGIN_API UStorageComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

#pragma region Variables
	
	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	int Capacity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	int SlotsFilled;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	TArray<FInventoryItem> Items;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	class UDataTable* dataTable;

	UPROPERTY(BlueprintAssignable, Category = Inventory)
	FOnStorageCompUpdated OnInventoryUpdated;

	UPROPERTY(BlueprintAssignable, Category = Inventory)
	FOnStorageCompFull OnInventoryFull;
	
#pragma endregion Variables

#pragma region BP Functions
	UFUNCTION(BlueprintCallable, Category = Inventory)
	TArray<FInventoryItem> GetItems() {return Items;}

#pragma endregion BP Functions
	
	// Sets default values for this component's properties
	UStorageComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;		
};
