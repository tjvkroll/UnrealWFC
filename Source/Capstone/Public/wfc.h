// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingBlockDataAsset.h"
#include "BuildingBlock.h"
#include "wfcBlock.h"
#include "wfc.generated.h"

//enum class EWFCFunctionResponse : uint8 {
//	EFR_NoMoreTilesToCollapse,
//	EFR_Worked,
//	EFR_NoChoiceAtTileLocation
//};

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

	UPROPERTY(EditAnywhere, category = "wfc_data")
	int LocationOffset;

	TArray<ABuildingBlock*> BB_refs; 

	int8 width;
	TArray<AwfcBlock*> tilemap;

 	
	
	Flocation start;
	Flocation finish; 

	UPROPERTY(EditDefaultsOnly, category = "wfc_data")
	TSubclassOf<AwfcBlock> block_class; 


	struct Neighbor {
		Neighbor(DIRECTION d, Flocation l) : dir(d), loc(l) {}
		DIRECTION dir;
		Flocation loc; 
	};

	TArray<Neighbor> get_neighbors(Flocation loc, int8 h);
	Flocation get_location_with_fewest_choices();
	TArray<Flocation> get_locations_requiring_updates();
	bool updatesRequired();
	void resetUpdateValues();
	bool add_constraint(Neighbor& neighbor,
		TArray<TSubclassOf<ABuildingBlock>>& sourceSuperPositions);
	TArray<Neighbor> get_neighbors_by_tf(Flocation loc);


public:	

	Awfc(){}
	virtual void BeginPlay() override;


	int16  At(int8 x, int8 y, int8 z);
	int16  At(const Neighbor& n) const;
	int16  At(const Flocation& n) const;

	bool Already_Generated = false;
	bool Already_Solved = false; 


	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "wfc_data")
	int height;

	// Holds all the paths for AI solvers
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "wfc_data")
	TArray<Flocation> LeftMostPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "wfc_data")
	int LMP_steps;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "wfc_data")
	int LMP_solved;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "wfc_data")
	TArray<Flocation> RightMostPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "wfc_data")
	int RMP_steps;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "wfc_data")
	int RMP_solved;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "wfc_data")
	TArray<Flocation> UpMostPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "wfc_data")
	int UMP_steps;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "wfc_data")
	int UMP_solved;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "wfc_data")
	TArray<Flocation> RandomPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "wfc_data")
	int Rando_steps;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "wfc_data")
	bool Rando_solved = false;


	UFUNCTION(BlueprintCallable)
	void ClearTiles();
	UFUNCTION(BlueprintCallable)
	void HighlightLocations(TArray<Flocation> path);
	UFUNCTION(BlueprintCallable)
	void WFCInit();
	UFUNCTION(BlueprintCallable)
	void SolveMaze();
	void Propogate(Flocation next_Flocation);
	void Iterate();
	void IterateSpecific(int8 x, int8 y, int8 z, FString name);
	bool validate();
	bool validPath(int8 choice);
	void validPathRecur(Flocation next);
	void LeftMostPathRecur(Flocation next);
	void RightMostPathRecur(Flocation next);
	void UpwardPathRecur(Flocation next);
	void RandomPathRecur(Flocation next);


	void MazeHelper();



	UFUNCTION(BlueprintCallable)
	void CreateMaze();
	
	UFUNCTION(BlueprintCallable)
	void LogTilemap();

	void Solidify();
};






