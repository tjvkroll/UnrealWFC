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

UCLASS()
class CAPSTONE_API Awfc : public AActor
{
	GENERATED_BODY()

	
	struct location {
		location() {
			x = -1;
			y = -1;
			z = -1;
		}
		location(int8 x, int8 y, int8 z) : x(x), y(y), z(z) {}
		int8 x;
		int8 y; 
		int8 z;
		void Display() {
			UE_LOG(LogTemp, Warning, TEXT("X: %d Y: %d Z: %d"), x, y , z);
		}
	};
protected:
	int8 threads;

	UPROPERTY(EditAnywhere, category = "wfc_data")
	int8 height;

	UPROPERTY(EditAnywhere, category = "wfc_data")
	int LocationOffset;

	int8 width;
	TArray<AwfcBlock*> tilemap; 
	location start;
	location finish; 

	UPROPERTY(EditDefaultsOnly, category = "wfc_data")
	TSubclassOf<AwfcBlock> block_class; 


	struct Neighbor {
		Neighbor(DIRECTION d, location l) : dir(d), loc(l) {}
		DIRECTION dir;
		location loc; 
	};

	TArray<Neighbor> get_neighbors(location loc, int8 h);
	location get_location_with_fewest_choices();
	TArray<location> get_locations_requiring_updates();
	bool updatesRequired();
	void resetUpdateValues();
	bool add_constraint(Neighbor& neighbor,
		TArray<TSubclassOf<ABuildingBlock>>& sourceSuperPositions);
	TArray<Neighbor> get_neighbors_by_tf(location loc);


public:	

	// Need 3d TArray struct????
	Awfc(){}
	virtual void BeginPlay() override;
	//Awfc(int8 height, int8 num_threads, AwfcBlock choices) : threads(num_threads) {
	//	//tilemap.Init(choices, height * height * height);
	//}

	int16  At(int8 x, int8 y, int8 z);
	int16  At(const Neighbor& n) const;
	int16  At(const location& n) const;
	//TArray<TArray<TArray<AwfcBlock*>>> GetTilemap();
	//int8 GetNumThreads();
	//void SetTilemap(TArray<TArray<TArray<AwfcBlock*>>> map);
	void init();
	void Propogate(location next_location);
	void Iterate();
	void IterateSpecific(int8 x, int8 y, int8 z, FString name);
	bool validate();
	bool validPath();
	void validPathRecur(location next);
	void MazeHelper();

	UFUNCTION(BlueprintCallable)
	void CreateMaze();
	
	UFUNCTION(BlueprintCallable)
	void LogTilemap();

	void Solidify();
};






