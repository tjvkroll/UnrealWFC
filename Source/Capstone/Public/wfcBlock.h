// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingBlockDataAsset.h"
#include "Containers/Array.h"
#include "Containers/UnrealString.h"
#include "BuildingBlock.h"
#include "wfcBlock.generated.h"


class UStaticMesh;

UENUM(BlueprintType)
enum class DIRECTION : uint8 {
	NORTH = 0,
	SOUTH = 1,
	WEST = 2,
	EAST = 3,
	UP = 4, 
	DOWN = 5
};

DIRECTION reverse(DIRECTION dir);

UCLASS()
class CAPSTONE_API AwfcBlock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AwfcBlock() {
		Init();
	};	

	UPROPERTY(EditDefaultsOnly, category = "block_data")
	UBuildingBlockDataAsset* initial_bb;

	//AwfcBlock(const TArray<ABuildingBlock*>& bb) : superPositions(bb) {};

	//UPROPERTY(EditDefaultsOnly, Category = "Block Data")
	//TArray<ABuildingBlock*> superPositions; 


	UPROPERTY(EditDefaultsOnly, Category = "Block Data")
	TArray<TSubclassOf<ABuildingBlock>> superPositionsTSC;

	UPROPERTY(EditAnywhere, Category = "Block Data")
	bool visited = false;

	UPROPERTY(EditAnywhere, Category = "Block Data")
	bool start = false;

	UPROPERTY(EditAnywhere, Category = "Block Data")
	bool finish = false;

	bool needsUpdate = false;

	void Init(); 

	void CollapseSuperPositions();

	void CollapseSpecific(FString name);

	void RemoveSuperPositionAtIndex(int8 index);

	int8 RandomBasedOnWeights();

	UFUNCTION(BlueprintCallable)
	void SpawnBB(int x, int y, int z);
};
