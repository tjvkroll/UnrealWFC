// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingBlock.generated.h"

class UStaticMeshComponent;

UCLASS(BlueprintType)
class CAPSTONE_API ABuildingBlock : public AActor
{
	GENERATED_BODY()

public:
	ABuildingBlock() {};

	ABuildingBlock(FString n, TArray<bool> s, int8 w, int8 d)
		: name(n), connections(s), weight(w), density(d) {}

	UPROPERTY(EditAnywhere, Category="Block Data")
	FString name = "noname";

	UPROPERTY(EditAnywhere, Category = "Block Data")
	TArray<bool> connections = {};

	UPROPERTY(EditAnywhere, Category = "Block Data")
	int8 weight = 0;

	UPROPERTY(EditAnywhere, Category = "Block Data")
	int8 density = 0;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* MaterialRef;

	//UFUNCTION(BlueprintImplementableEvent)
	//void HighlightBlock(bool IsInPath);
};

