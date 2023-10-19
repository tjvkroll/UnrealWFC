// Fill out your copyright notice in the Description page of Project Settings.

#include "wfcBlock.h"

int8 AwfcBlock::RandomBasedOnWeights() {
    // Approach implemented from: https://www.keithschwarz.com/darts-dice-coins/
    int32 sum_weights = 0;
    for (int8 i = 0; i < superPositionsTSC.Num(); i++) {
        //sum_weights += superPositions[i]->weight;
        sum_weights += Cast<ABuildingBlock>(superPositionsTSC[i])->weight;
    }
    TArray<int8> probs;
    for (int8 i = 0; i < superPositionsTSC.Num(); i++) {
        //for (int j = 0; j < superPositions[i]->weight; j++) {
        for (int j = 0; j < Cast<ABuildingBlock>(superPositionsTSC[i])->weight; j++) {
            probs.Add(i);
        }
    }
    int random_idx = rand() % probs.Num();
    return probs[random_idx];
}



void AwfcBlock::Init()
{
    for (auto& block : initial_bb->initial_blocks) {
        superPositionsTSC.Add(block);
    }
}

// Choosing a BUILDING BLOCK OUT OF REMAINING OPTIONS
void AwfcBlock::CollapseSuperPositions() {
    int rnd = RandomBasedOnWeights();

    for (int8 i = superPositionsTSC.Num() - 1; i > rnd; i--) {
        superPositionsTSC.RemoveAt(i); 
    }

    for (int8 i = rnd-1; i >= 0; i--) {
        superPositionsTSC.RemoveAt(i); 
    }


    // old way of doing it. 

//    ABuildingBlock tmp = superPositions[rnd];
//    superPositionsTSC.Empty();
//    superPositionsTSC.Add(tmp);
}

// Allowing for collapsing of specific tiles so we can force images
void AwfcBlock::CollapseSpecific(FString name) {
    int8 saveIdx = 0; 
    for (int8 i = 0; i < superPositionsTSC.Num(); i++) {
        //if (superPositions[i]->name == name) {
        if (Cast<ABuildingBlock>(superPositionsTSC[i])->name == name) {
            saveIdx = i; 
            break; 
            //OLD WAY OF DOING IT
            //ABuildingBlock tmp = index;
            //superPositionsTSC.Empty();
            //superPositionsTSC.Add(tmp);
        }
    }

    for (int8 i = superPositionsTSC.Num() - 1; i > saveIdx; i--) {
        superPositionsTSC.RemoveAt(i);
    }

    for (int8 i = saveIdx - 1; i >= 0; i--) {
        superPositionsTSC.RemoveAt(i);
    }
}

// removing BUILDING BLOCKS
void AwfcBlock::RemoveSuperPositionAtIndex(int8 index) {
    superPositionsTSC.RemoveAt(index);
}

// returns the mirror of any given direction, used in find all neighbors
DIRECTION reverse(DIRECTION dir) {
    switch (dir) {
    case DIRECTION::UP:
        return DIRECTION::DOWN;
        break;
    case DIRECTION::DOWN:
        return DIRECTION::UP;
        break;
    case DIRECTION::WEST:
        return DIRECTION::EAST;
        break;
    case DIRECTION::EAST:
        return DIRECTION::WEST;
        break;
    case DIRECTION::NORTH:
        return DIRECTION::SOUTH;
        break;
    case DIRECTION::SOUTH:
        return DIRECTION::NORTH;
        break;
    default:
        //assert(false);
        break; 
    }
    UE_LOG(LogTemp, Warning, TEXT("NO VALID REVERSE!!!! RETURNING NORTH AS DEFAULT"));
    return DIRECTION::NORTH;
}



void AwfcBlock::SpawnBB(int x, int y, int z)
{
    FVector worldCoords(x, y, z);
    GetWorld()->SpawnActor<ABuildingBlock>(superPositionsTSC[0], worldCoords, FRotator());
}