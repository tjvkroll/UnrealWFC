// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingBlockDataAsset.h"
#include "BuildingBlock.h"
#include "wfcBlock.h"
#include "wfc.generated.h"

// location struct for... you guessed it locations
USTRUCT(BlueprintType)
struct Flocation {
	GENERATED_BODY()
	Flocation() {
		x = -1;
		y = -1;
		z = -1;
	}
	Flocation(int8 x, int8 y, int8 z) : x(x), y(y), z(z) {}
	UPROPERTY()
	int x;
	UPROPERTY()
	int y; 
	UPROPERTY()
	int z;
	void Display() {
		UE_LOG(LogTemp, Warning, TEXT("X: %d Y: %d Z: %d"), x, y , z);
	}
};

UCLASS()
class CAPSTONE_API Awfc : public AActor
{
	GENERATED_BODY()
protected:
	// The meat and potatos 
	TArray<AwfcBlock*> tilemap;

	// used to offset the blocks so when they're spawned they are touching but don't overlap
	UPROPERTY(EditAnywhere, category = "wfc_data")
	int LocationOffset;
	
	// references to building blocks to fill up each cell in tilemap
	TArray<ABuildingBlock*> BB_refs; 
	
	
	bool finish_found; 
	Flocation start;
	Flocation finish; 

	// a single cell in the tilemap
	UPROPERTY(EditDefaultsOnly, category = "wfc_data")
	TSubclassOf<AwfcBlock> block_class; 

	// struct used in finding a source cells neighbors to propagation and pathing
	struct Neighbor {
		Neighbor(DIRECTION d, Flocation l) : dir(d), loc(l) {}
		DIRECTION dir;
		Flocation loc; 
	};


	// various helper functions
	TArray<Neighbor> get_neighbors(Flocation loc, int8 h);
	Flocation get_location_with_fewest_choices();
	TArray<Flocation> get_locations_requiring_updates();
	bool updatesRequired();
	void resetUpdateValues();
	bool add_constraint(Neighbor& neighbor,
		TArray<TSubclassOf<ABuildingBlock>>& sourceSuperPositions);
	TArray<Neighbor> get_neighbors_by_tf(Flocation loc);
	TArray<Neighbor> RM_get_neighbors_by_tf(Flocation loc);
	TArray<Neighbor> UM_get_neighbors_by_tf(Flocation loc);
	bool CheckDirection(Flocation loc, DIRECTION dir);



public:	

	Awfc(){}
	virtual void BeginPlay() override;

	// helper functions
	int16  At(int8 x, int8 y, int8 z);
	int16  At(const Neighbor& n) const;
	int16  At(const Flocation& n) const;

	UPROPERTY(BlueprintReadWrite)
	bool Already_Generated = false;
	UPROPERTY(BlueprintReadWrite)
	bool Already_Solved = false; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "wfc_data")
	int height;

	// Solver variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "wfc_data")
	TArray<Flocation> LeftMostPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "wfc_data")
	int LMP_steps;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "wfc_data")
	bool LMP_solved = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "wfc_data")
	TArray<Flocation> RightMostPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "wfc_data")
	int RMP_steps;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "wfc_data")
	bool RMP_solved = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "wfc_data")
	TArray<Flocation> UpMostPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "wfc_data")
	int UMP_steps;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "wfc_data")
	bool UMP_solved = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "wfc_data")
	TArray<Flocation> EuclidianPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "wfc_data")
	int Euclidian_steps;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "wfc_data")
	bool Euclidian_solved = false;
	
	UPROPERTY(BlueprintReadWrite)
	bool Visualize_LMP = false;
	UPROPERTY(BlueprintReadWrite)
	bool Visualize_RMP = false;
	UPROPERTY(BlueprintReadWrite)
	bool Visualize_UMP = false;
	UPROPERTY(BlueprintReadWrite)
	bool Visualize_Euclidian = false;


	// C++ only functions
	void Propogate(Flocation next_Flocation);
	void Iterate();
	void IterateSpecific(int8 x, int8 y, int8 z, FString name);
	bool validate();
	bool validPath(int8 choice);
	void validPathRecur(Flocation next);
	void LeftMostPathRecur(Flocation next);
	void RightMostPathRecur(Flocation next);
	void UpwardPathRecur(Flocation next);
	void EuclidianPathRecur(Flocation next);
	TArray<DIRECTION> FindEuclidianDir(int x, int y, int z);
	void MazeHelper();
	void Solidify();


	// Blueprint callable functions
	UFUNCTION(BlueprintCallable)
	void ClearBBREFS();
	UFUNCTION(BlueprintCallable)
	void SpawnBlockAtLocation(Flocation loc);
	UFUNCTION(BlueprintCallable)
	void ClearTiles();
	UFUNCTION(BlueprintCallable)
	void WFCInit();
	UFUNCTION(BlueprintCallable)
	void SolveMaze();
	UFUNCTION(BlueprintCallable)
	void CreateMaze();
	UFUNCTION(BlueprintCallable)
	void LogTilemap();
};






