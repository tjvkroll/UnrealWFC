// Fill out your copyright notice in the Description page of Project Settings.


#include "wfc.h"

#define QLOG(A) UE_LOG(LogTemp, Warning, TEXT(A))
#define QLOGFMT(A, B) UE_LOG(LogTemp, Warning, TEXT(A), B)

void Awfc::BeginPlay()
{
    //CreateMaze();
    PrimaryActorTick.bCanEverTick = false;
}

int16 Awfc::At(int8 x, int8 y, int8 z)
{
    return (z * height * height) + (y * height) + x; 
}

int16 Awfc::At(const Flocation& n) const
{
    return (n.z * height * height) + (n.y * height) + n.x;
}

int16 Awfc::At(const Neighbor& n) const
{
    return (n.loc.z * height * height) + (n.loc.y * height) + n.loc.x;
}


// South = +y
// North = -y
// East = +x
// west = -x
// DONE 
TArray<Awfc::Neighbor> Awfc::get_neighbors(Flocation loc, int8 h)
{
    TArray<Neighbor> neighborTup;
    if (loc.x != 0)
        neighborTup.Add(
            Neighbor(DIRECTION::EAST, Flocation(loc.x - 1, loc.y, loc.z)));
    if (loc.y != 0)
        neighborTup.Add(
            Neighbor(DIRECTION::SOUTH, Flocation(loc.x, loc.y - 1, loc.z)));
    if (loc.x < h - 1)
        neighborTup.Add(
            Neighbor(DIRECTION::WEST, Flocation(loc.x + 1, loc.y, loc.z)));
    if (loc.y < h - 1)
        neighborTup.Add(
            Neighbor(DIRECTION::NORTH, Flocation(loc.x, loc.y + 1, loc.z)));
    // New changes for 3D
    if (loc.z != 0) {
        neighborTup.Add(
            Neighbor(DIRECTION::UP, Flocation(loc.x, loc.y, loc.z - 1)));
    }
    if (loc.z < h - 1) {
        neighborTup.Add(
            Neighbor(DIRECTION::DOWN, Flocation(loc.x, loc.y, loc.z + 1)));
    }
    return neighborTup;
}






bool Awfc::CheckDirection(Flocation loc, DIRECTION dir) {
    switch (dir) {
    case DIRECTION::EAST:
        return (tilemap[At(loc.x, loc.y, loc.z)]->superPositionsTSC[0].GetDefaultObject()->connections[(int8)DIRECTION::EAST] ==
            true &&
            loc.x < height - 1);
        break;
    case DIRECTION::WEST:
        return (tilemap[At(loc.x, loc.y, loc.z)]->superPositionsTSC[0].GetDefaultObject()->connections[(int8)DIRECTION::WEST] ==
            true &&
            loc.x != 0);
        break; 

    case DIRECTION::NORTH:
        return (tilemap[At(loc.x, loc.y, loc.z)]->superPositionsTSC[0].GetDefaultObject()->connections[(int8)DIRECTION::NORTH] ==
            true &&
            loc.y != 0);
        break; 
    case DIRECTION::SOUTH:
        return (tilemap[At(loc.x, loc.y, loc.z)]->superPositionsTSC[0].GetDefaultObject()->connections[(int8)DIRECTION::SOUTH] ==
            true &&
            loc.y < height - 1);
        break;
    case DIRECTION::UP:
        return (tilemap[At(loc.x, loc.y, loc.z)]->superPositionsTSC[0].GetDefaultObject()->connections[(int8)DIRECTION::UP] ==
            true &&
            loc.z < height - 1);
        break;
    case DIRECTION::DOWN:
        return  (tilemap[At(loc.x, loc.y, loc.z)]->superPositionsTSC[0].GetDefaultObject()->connections[(int8)DIRECTION::DOWN] == true &&
            loc.z != 0);
        break; 
    default:
        return false;
        break; 
    }

    return false; 
}


TArray<Awfc::Neighbor> Awfc::get_neighbors_by_tf(Flocation loc)
{
    TArray<Neighbor> neighborTup;
    if (CheckDirection(loc, DIRECTION::WEST)) neighborTup.Add(Neighbor(DIRECTION::EAST, Flocation(loc.x - 1, loc.y, loc.z)));
    if (CheckDirection(loc, DIRECTION::NORTH)) neighborTup.Add(Neighbor(DIRECTION::SOUTH, Flocation(loc.x, loc.y - 1, loc.z)));
    if (CheckDirection(loc, DIRECTION::DOWN)) neighborTup.Add(Neighbor(DIRECTION::UP, Flocation(loc.x, loc.y, loc.z - 1))); 
    if (CheckDirection(loc, DIRECTION::SOUTH)) neighborTup.Add(Neighbor(DIRECTION::NORTH, Flocation(loc.x, loc.y + 1, loc.z)));
    if (CheckDirection(loc, DIRECTION::EAST)) neighborTup.Add(Neighbor(DIRECTION::WEST, Flocation(loc.x + 1, loc.y, loc.z)));
    if (CheckDirection(loc, DIRECTION::UP)) neighborTup.Add(Neighbor(DIRECTION::DOWN, Flocation(loc.x, loc.y, loc.z + 1)));
    return neighborTup;
}

TArray<Awfc::Neighbor> Awfc::RM_get_neighbors_by_tf(Flocation loc)
{
    TArray<Neighbor> neighborTup;
    if (CheckDirection(loc, DIRECTION::WEST)) neighborTup.Add(Neighbor(DIRECTION::EAST, Flocation(loc.x - 1, loc.y, loc.z)));
    if (CheckDirection(loc, DIRECTION::UP)) neighborTup.Add(Neighbor(DIRECTION::DOWN, Flocation(loc.x, loc.y, loc.z + 1)));
    if (CheckDirection(loc, DIRECTION::SOUTH)) neighborTup.Add(Neighbor(DIRECTION::NORTH, Flocation(loc.x, loc.y + 1, loc.z)));
    if (CheckDirection(loc, DIRECTION::DOWN)) neighborTup.Add(Neighbor(DIRECTION::UP, Flocation(loc.x, loc.y, loc.z - 1)));
    if (CheckDirection(loc, DIRECTION::EAST)) neighborTup.Add(Neighbor(DIRECTION::WEST, Flocation(loc.x + 1, loc.y, loc.z)));
    if (CheckDirection(loc, DIRECTION::NORTH)) neighborTup.Add(Neighbor(DIRECTION::SOUTH, Flocation(loc.x, loc.y - 1, loc.z)));
    return neighborTup;
}

TArray<Awfc::Neighbor> Awfc::UM_get_neighbors_by_tf(Flocation loc)
{
    TArray<Neighbor> neighborTup;
    if (CheckDirection(loc, DIRECTION::DOWN)) neighborTup.Add(Neighbor(DIRECTION::UP, Flocation(loc.x, loc.y, loc.z - 1)));
    if (CheckDirection(loc, DIRECTION::SOUTH)) neighborTup.Add(Neighbor(DIRECTION::NORTH, Flocation(loc.x, loc.y + 1, loc.z)));
    if (CheckDirection(loc, DIRECTION::EAST)) neighborTup.Add(Neighbor(DIRECTION::WEST, Flocation(loc.x + 1, loc.y, loc.z)));
    if (CheckDirection(loc, DIRECTION::NORTH)) neighborTup.Add(Neighbor(DIRECTION::SOUTH, Flocation(loc.x, loc.y - 1, loc.z)));
    if (CheckDirection(loc, DIRECTION::WEST)) neighborTup.Add(Neighbor(DIRECTION::EAST, Flocation(loc.x - 1, loc.y, loc.z)));
    if (CheckDirection(loc, DIRECTION::UP)) neighborTup.Add(Neighbor(DIRECTION::DOWN, Flocation(loc.x, loc.y, loc.z + 1)));
    return neighborTup;
}

// DONE
Flocation Awfc::get_location_with_fewest_choices()
{
    int8 min = 100;
    TArray<Flocation> locs;
    for (int8 layer = 0; layer < height; layer++) {
        for (int8 row = 0; row < height; row++) {
            for (int8 col = 0; col < height; col++) {
                if ((int8)tilemap[At(col,row,layer)]->superPositionsTSC.Num() < min &&
                    tilemap[At(col, row, layer)]->superPositionsTSC.Num() > 1) {
                    min = tilemap[At(col, row, layer)]->superPositionsTSC.Num();
                    locs.Empty();
                }
                if ((int8)tilemap[At(col, row, layer)]->superPositionsTSC.Num() == min) {
                    locs.Add(Flocation(col, row, layer));
                }
            }
        }
    }

    if (locs.Num() != 0) {
        int randindx = rand() % locs.Num();
        return locs[randindx];
    }
    else {
        return Flocation();
    }
}

// DONE
TArray<Flocation> Awfc::get_locations_requiring_updates()
{
    TArray<Flocation> locs;

    // PRAGMA CANDIDATE
    for (int8 layer = 0; layer < height; layer++) {
        for (int8 row = 0; row < height; row++) {
            for (int8 col = 0; col < height; col++) {
                if (tilemap[At(col, row, layer)]->needsUpdate)
                    locs.Add(Flocation(col, row, layer));
            }
        }
    }
    return locs;
}

// DONE
bool Awfc::updatesRequired()
{
    bool needsUpdate = false;

    for (int8 layer = 0; layer < height; layer++) {
        for (int8 row = 0; row < height; row++) {
            for (int8 col = 0; col < height; col++) {
                if (tilemap[At(col, row, layer)]->needsUpdate) needsUpdate = true;
            }
        }
    }
    return needsUpdate;
}

// DONE
void Awfc::resetUpdateValues()
{
    for (int8 layer = 0; layer < height; layer++) {
        for (int8 row = 0; row < height; row++) {
            for (int8 col = 0; col < height; col++) {
                tilemap[At(col, row, layer)]->needsUpdate = false;
            }
        }
    }
}

// DONE 
bool Awfc::add_constraint(Neighbor& neighbor, TArray<TSubclassOf<ABuildingBlock>>& sourceSuperPositions)
{
   /* neighbor.loc.Display();
    int idxcur = At(neighbor);*/
    // Get the sides of each tile we'll be constraining by
    DIRECTION incDirection = neighbor.dir;
    DIRECTION outDirection = reverse(incDirection);
    bool changed = false;
    TSet<bool> neighborConstraint;

    for (auto& superPosition : sourceSuperPositions) {
        //FString v = superPosition.GetDefaultObject()->connections[(int8)outDirection] ? "True" : "False";
        neighborConstraint.Add(superPosition.GetDefaultObject()->connections[(int8)outDirection]);
    }
    //for (auto& cb : neighborConstraint) { FString isvalidstr = cb ? "True" : "False"; QLOGFMT("%s", *isvalidstr); }
    // Only want to constrain neighbors that haven't collapsed
    if ((int8)tilemap[At(neighbor)]->superPositionsTSC.Num() != 1) {
        TArray<int8> indicies_to_remove;
        for (int8 i = 0; i < (int8)tilemap[At(neighbor)]->superPositionsTSC.Num(); i++) {
            bool searchVal = tilemap[At(neighbor)]->superPositionsTSC[i].GetDefaultObject()->connections[(int8)incDirection];
            if (!neighborConstraint.Contains(searchVal)) {
                // remove superPosition at i
                indicies_to_remove.Add(i);
                changed = true;
            }
        }
        // Remove building blocks that don't fit based on the constraints of our source
        for (int8 i = indicies_to_remove.Num() - 1; i >= 0; i--) {
            //if (neighbor.loc.x == 1 && neighbor.loc.y == 1 && neighbor.loc.z == 1) { QLOGFMT("%s", *tilemap[At(neighbor)]->superPositionsTSC[indicies_to_remove[i]].GetDefaultObject()->name); }
            tilemap[At(neighbor)]->RemoveSuperPositionAtIndex(indicies_to_remove[i]);
        }
    }

    // failsafe: if we happen to remove all options for a tile throw an error
    if ((int8)tilemap[At(neighbor)]->superPositionsTSC.Num() == 0) {
        throw std::runtime_error("No patterns left at Flocation.");
    }
    return changed;
}

// DONE? 
void Awfc::Propogate(Flocation next_Flocation)
{
   
    //next_Flocation.Display();
    if (tilemap[At(next_Flocation)]->superPositionsTSC.Num() == 0) {
        QLOG("EMPTY TSC!");
        return;
    }

    // sets our starting point for propagation
    resetUpdateValues();
    tilemap[At(next_Flocation)]->needsUpdate = true;
    // Loop to continue propagating until there are no more tiles that have been
    while (updatesRequired()) {
        // Gathers a list of Flocations that need to be propagated from
        TArray<Flocation> Flocations = get_locations_requiring_updates();
        resetUpdateValues();
        for (auto& Flocation : Flocations) {
            //Flocation.Display();
            // Gets all neighbors of the Flocation we're propagating from
            TArray<Neighbor> neighbors = get_neighbors(Flocation, height);
            for (auto& neighbor : neighbors) {
                // Applying constraints to each neighbor of our source(Flocation)
                bool wasUpdated; 
                try {
                    wasUpdated = add_constraint(neighbor, tilemap[At(Flocation)]->superPositionsTSC);
                }
                catch (...) {
                    QLOG("Caught Add_Constraint!");
                }
                // If any constraints were added, mark it as needing to update so we
                // propagate from this Flocation next
                tilemap[At(neighbor)]->needsUpdate |= wasUpdated;
            }
        }
    }

}


// CAN WE THROW ERRORS IN THE SAME WAY WE NORMALLY WOULD
void Awfc::Iterate()
{
    Flocation next_Flocation = get_location_with_fewest_choices();
    // failsafe / end if iterating
    if (next_Flocation.x == -1) {
        throw(std::runtime_error("No more tiles to collapse!"));
    }
    if ((int8)tilemap[At(next_Flocation.x, next_Flocation.y, next_Flocation.z)]->superPositionsTSC.Num() < 2) {
        throw(std::runtime_error("No choices at tile Flocation"));
    }
    tilemap[At(next_Flocation.x, next_Flocation.y, next_Flocation.z)]->CollapseSuperPositions();
    try {

        Propogate(next_Flocation);
    }
    catch (...) {
        QLOG("Caught Propagate!")
    }
}

// DONE
void Awfc::IterateSpecific(int8 x, int8 y, int8 z, FString name)
{
    tilemap[At(x, y, z)]->CollapseSpecific(name);
    if (tilemap[At(x, y, z)]->superPositionsTSC.Num() == 1) {
    }
    Flocation collapsed(x, y, z);
    try {
        Propogate(collapsed);
    }
    catch (...) {
        UE_LOG(LogTemp, Warning, TEXT("Caught Propagate!"));
    }
}

// DONE
bool Awfc::validate()
{
    // Loops to work through entire tilemap
    for (int8 h = 0; h < height; h++) {
        for (int8 i = 0; i < height; i++) {
            for (int8 j = 0; j < height; j++) {
                Flocation cur(j, i, h);
                TArray<Neighbor> neighbors = get_neighbors(cur, height);
                // compare current tile to its neighbors for valid connections
                for (int8 k = 0; k < neighbors.Num(); k++) {
                    // get the indexes of "connections" for cur tile and its neighbor
                    DIRECTION neighbor_dir = neighbors[k].dir;
                    DIRECTION cur_dir = reverse(neighbor_dir);

                    if (tilemap[At(j,i,h)]->superPositionsTSC[0].GetDefaultObject()->connections[(int8)cur_dir] !=
                        tilemap[At(neighbors[k].loc.x, neighbors[k].loc.y, neighbors[k].loc.z)]
                        ->superPositionsTSC[0].GetDefaultObject()
                        ->connections[(int8)neighbor_dir]) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

void Awfc::WFCInit()
{
    for (int8 z = 0; z < height; z++) {
        for (int8 y = 0; y < height; y++) {
            for (int8 x = 0; x < height; x++) {
                FVector worldCoords(LocationOffset * x, LocationOffset * y, LocationOffset * z);
                tilemap.Add(GetWorld()->SpawnActor<AwfcBlock>(block_class, worldCoords, FRotator()));
                tilemap[At(x, y, z)]->Init();
            }
        }
    }
}

// DONE
bool Awfc::validPath(int8 choice)
{

    for (int8 z = 0; z < height; z++) {
        for (int8 y = 0; y < height; y++) {
            for (int8 x = 0; x < height; x++) {
                tilemap[At(x, y, z)]->visited = false;
            }
        }
    }

    if (choice == 0) { LeftMostPathRecur(Flocation(start.x, start.y, start.z)); LMP_steps = LeftMostPath.Num(); }
    else if (choice == 1) { RightMostPathRecur(Flocation(start.x, start.y, start.z)); RMP_steps = RightMostPath.Num();}
    else if (choice == 2) { UpwardPathRecur(Flocation(start.x, start.y, start.z)); UMP_steps = UpMostPath.Num(); }
    else if (choice == 3) { EuclidianPathRecur(Flocation(start.x, start.y, start.z)); Euclidian_steps = EuclidianPath.Num(); }

    // debugging option 
    else if (choice == 10) { validPathRecur(Flocation(start.x, start.y, start.z)); LMP_steps = LeftMostPath.Num(); }

    //validPathRecur(Flocation(start.x, start.y, start.z));
    if (tilemap[At(finish.x, finish.y, finish.z)]->visited == true) {
        return true;
    }
    return false;
}

//Basic
void Awfc::validPathRecur(Flocation next)
{
    if (finish_found == true) return; 
    
    tilemap[At(next.x, next.y, next.z)]->visited = true;


    // Add Flocation to list
    LeftMostPath.Add(next);

    TArray<Neighbor> tile_neighbors = get_neighbors_by_tf(next);

    for (auto& neighbor : tile_neighbors) {

        if (neighbor.loc.x == finish.x && neighbor.loc.y == finish.y && neighbor.loc.z == finish.z) {
            finish_found = true;
            tilemap[At(finish.x, finish.y, finish.z)]->visited = true;
            LeftMostPath.Add(neighbor.loc);
            return; 
        }

        if (tilemap[At(neighbor)]
            ->visited == false) {
            validPathRecur(neighbor.loc);
        }
    }
}

//LeftMost
void Awfc::LeftMostPathRecur(Flocation next)
{

    if (finish_found == true) return;

    tilemap[At(next.x, next.y, next.z)]->visited = true;

    // Add Flocation to list
    LeftMostPath.Add(next);

    TArray<Neighbor> tile_neighbors = get_neighbors_by_tf(next);
    
    for (auto& neighbor : tile_neighbors) {
        if (neighbor.loc.x == finish.x && neighbor.loc.y == finish.y && neighbor.loc.z == finish.z) {
            finish_found = true;
            tilemap[At(finish.x, finish.y, finish.z)]->visited = true;
            LeftMostPath.Add(neighbor.loc);
            return;
        }

        if (tilemap[At(neighbor)]->visited == false) {
            LeftMostPathRecur(neighbor.loc);
        }
    }

}

//RightMost
void Awfc::RightMostPathRecur(Flocation next)
{

    if (finish_found == true) return;

    tilemap[At(next.x, next.y, next.z)]->visited = true;

    // Add Flocation to list
    RightMostPath.Add(next);

    TArray<Neighbor> tile_neighbors = RM_get_neighbors_by_tf(next);

    for (auto& neighbor : tile_neighbors) {

        if (neighbor.loc.x == finish.x && neighbor.loc.y == finish.y && neighbor.loc.z == finish.z) {
            finish_found = true;
            tilemap[At(finish.x, finish.y, finish.z)]->visited = true;
            RightMostPath.Add(neighbor.loc);
            return;
        }

        if (tilemap[At(neighbor)]->visited == false) {
            RightMostPathRecur(neighbor.loc);
        }
    }

}

//Upward
void Awfc::UpwardPathRecur(Flocation next)
{

    if (finish_found == true) return;

    tilemap[At(next.x, next.y, next.z)]->visited = true;

    // Add Flocation to list
    UpMostPath.Add(next);

    TArray<Neighbor> tile_neighbors = UM_get_neighbors_by_tf(next);

    for (auto& neighbor : tile_neighbors) {

        if (neighbor.loc.x == finish.x && neighbor.loc.y == finish.y && neighbor.loc.z == finish.z) {
            finish_found = true;
            tilemap[At(finish.x, finish.y, finish.z)]->visited = true;
            UpMostPath.Add(neighbor.loc);
            return;
        }

        if (tilemap[At(neighbor)]->visited == false) {
            UpwardPathRecur(neighbor.loc);
        }
    }
}

TArray<DIRECTION> Awfc::FindEuclidianDir(int x, int y, int z) {
    TArray<DIRECTION> Priority;
    if (abs(x) >= abs(y)) {
        if (abs(x) >= abs(z)) {
            // x is priority now decide which dir to travel
            if (x >= 0) { Priority.Add(DIRECTION::EAST);}
            else { Priority.Add(DIRECTION::WEST); }
            // Add 2 other priorities
            if (abs(y) >= abs(z)) {
                if (y >= 0) { Priority.Add(DIRECTION::SOUTH); }
                else { Priority.Add(DIRECTION::NORTH); }
                
                if (z >= 0) { Priority.Add(DIRECTION::UP); }
                else { Priority.Add(DIRECTION::DOWN); }
            }
            else {
                if (z >= 0) { Priority.Add(DIRECTION::UP); }
                else { Priority.Add(DIRECTION::DOWN); }
                
                if (y >= 0) { Priority.Add(DIRECTION::SOUTH); }
                else { Priority.Add(DIRECTION::NORTH); }
            }
        }
        else {
            // z is priority now decide which dir to travel
            if (z >= 0) { Priority.Add(DIRECTION::UP); }
            else { Priority.Add(DIRECTION::DOWN); }
            // Add 2 other priorities
            if (abs(y) >= abs(x)) {
                if (y >= 0) { Priority.Add(DIRECTION::SOUTH); }
                else { Priority.Add(DIRECTION::NORTH); }

                if (x >= 0) { Priority.Add(DIRECTION::EAST); }
                else { Priority.Add(DIRECTION::WEST); }
            }
            else {
                if (x >= 0) { Priority.Add(DIRECTION::EAST); }
                else { Priority.Add(DIRECTION::WEST); }

                if (y >= 0) { Priority.Add(DIRECTION::SOUTH); }
                else { Priority.Add(DIRECTION::NORTH); }
            }

        }
    }
    else {
        if (abs(y) >= abs(z)) {
            // y is priority now decide which dir to travel
            if (y >= 0) { Priority.Add(DIRECTION::SOUTH); }
            else { Priority.Add(DIRECTION::NORTH); }
            if (abs(x) > abs(z)) {
                if (x >= 0) { Priority.Add(DIRECTION::EAST); }
                else { Priority.Add(DIRECTION::WEST); }

                if (z >= 0) { Priority.Add(DIRECTION::UP); }
                else { Priority.Add(DIRECTION::DOWN); }
            }
            else {
                if (z >= 0) { Priority.Add(DIRECTION::UP); }
                else { Priority.Add(DIRECTION::DOWN); }
                
                if (x >= 0) { Priority.Add(DIRECTION::EAST); }
                else { Priority.Add(DIRECTION::WEST); }
            }
        }
        else {
            // z is priority now decide which dir to travel
            if (z >= 0) { Priority.Add(DIRECTION::UP); }
            else { Priority.Add(DIRECTION::DOWN); }
            if (abs(x) > abs(y)) {
                if (x >= 0) { Priority.Add(DIRECTION::EAST); }
                else { Priority.Add(DIRECTION::WEST); }

                if (y >= 0) { Priority.Add(DIRECTION::SOUTH); }
                else { Priority.Add(DIRECTION::NORTH); }
            }
            else {
                if (y >= 0) { Priority.Add(DIRECTION::SOUTH); }
                else { Priority.Add(DIRECTION::NORTH); }

                if (x >= 0) { Priority.Add(DIRECTION::EAST); }
                else { Priority.Add(DIRECTION::WEST); }
            }
        }
    }
    return Priority; 
}

//Random
void Awfc::EuclidianPathRecur(Flocation next)
{
    if (finish_found == true) return;
    
    tilemap[At(next.x, next.y, next.z)]->visited = true;

    TArray<Neighbor> tile_neighbors;
    
    // Add Flocation to list
    EuclidianPath.Add(next);

    // LOGIC TO FIGURE +/- for x, y, z
    TArray<DIRECTION> Priorities = FindEuclidianDir(finish.x - next.x, finish.y - next.y, finish.z - next.z);

    // Get list of neighbors by priority
    for (auto& dir : Priorities) {

        if (dir == DIRECTION::WEST && CheckDirection(next, DIRECTION::WEST)) tile_neighbors.Add(Awfc::Neighbor(DIRECTION::EAST, Flocation(next.x - 1, next.y, next.z)));
        else if (dir == DIRECTION::NORTH && CheckDirection(next, DIRECTION::NORTH)) tile_neighbors.Add(Awfc::Neighbor(DIRECTION::SOUTH, Flocation(next.x, next.y - 1, next.z)));
        else if (dir == DIRECTION::DOWN && CheckDirection(next, DIRECTION::DOWN)) tile_neighbors.Add(Awfc::Neighbor(DIRECTION::UP, Flocation(next.x, next.y, next.z - 1)));
        else if (dir == DIRECTION::SOUTH && CheckDirection(next, DIRECTION::SOUTH)) tile_neighbors.Add(Awfc::Neighbor(DIRECTION::NORTH, Flocation(next.x, next.y + 1, next.z)));
        else if (dir == DIRECTION::EAST && CheckDirection(next, DIRECTION::EAST)) tile_neighbors.Add(Awfc::Neighbor(DIRECTION::WEST, Flocation(next.x + 1, next.y, next.z)));
        else if (dir == DIRECTION::UP && CheckDirection(next, DIRECTION::UP)) tile_neighbors.Add(Awfc::Neighbor(DIRECTION::DOWN, Flocation(next.x, next.y, next.z + 1)));
    }
    
    // fill remainder of list by inverse priority
    for (int i = Priorities.Num() - 1; i >= 0; i--) {
        DIRECTION dir = reverse(Priorities[i]);

        if (dir == DIRECTION::WEST && CheckDirection(next, DIRECTION::WEST)) tile_neighbors.Add(Awfc::Neighbor(DIRECTION::EAST, Flocation(next.x - 1, next.y, next.z)));
        else if (dir == DIRECTION::NORTH && CheckDirection(next, DIRECTION::NORTH)) tile_neighbors.Add(Awfc::Neighbor(DIRECTION::SOUTH, Flocation(next.x, next.y - 1, next.z)));
        else if (dir == DIRECTION::DOWN && CheckDirection(next, DIRECTION::DOWN)) tile_neighbors.Add(Awfc::Neighbor(DIRECTION::UP, Flocation(next.x, next.y, next.z - 1)));
        else if (dir == DIRECTION::SOUTH && CheckDirection(next, DIRECTION::SOUTH)) tile_neighbors.Add(Awfc::Neighbor(DIRECTION::NORTH, Flocation(next.x, next.y + 1, next.z)));
        else if (dir == DIRECTION::EAST && CheckDirection(next, DIRECTION::EAST)) tile_neighbors.Add(Awfc::Neighbor(DIRECTION::WEST, Flocation(next.x + 1, next.y, next.z)));
        else if (dir == DIRECTION::UP && CheckDirection(next, DIRECTION::UP)) tile_neighbors.Add(Awfc::Neighbor(DIRECTION::DOWN, Flocation(next.x, next.y, next.z + 1)));
    }

    for (auto& neighbor : tile_neighbors) {

        if (neighbor.loc.x == finish.x && neighbor.loc.y == finish.y && neighbor.loc.z == finish.z) {
            finish_found = true;
            tilemap[At(finish.x, finish.y, finish.z)]->visited = true;
            EuclidianPath.Add(neighbor.loc);
            return;
        }

        if (tilemap[At(neighbor)]->visited == false) {
            EuclidianPathRecur(neighbor.loc);
        }
    }
}

// This is where I can call all of my different solvers
void Awfc::SolveMaze() {
    if (Already_Solved || !Already_Generated) {
        return; 
    }
    
    //LMP_solved = validPath(0);
    
    // this is actually running the OG path recur as a test 
    LMP_solved = validPath(10);
    finish_found = false; 
    RMP_solved = validPath(1);
    finish_found = false;
    UMP_solved = validPath(2);
    finish_found = false;
    Euclidian_solved = validPath(3);

    LogTilemap();
    Already_Solved = true; 
}

// DONE
void Awfc::MazeHelper()
{
    int8 in = rand() % height;
    int8 out = rand() % height;

    // moving them out of corners to avoid weird edge cases
    if (in == 0) in++;
    if (out == 0) out++;
    if (in == height - 1) in--;
    if (out == height - 1) out--;

    // used by AI agents and validating a path through the maze

    // FOR NOW WELL MAKE THE IN/OUT at TOP/BOTTOM
    start = Flocation(in, height - 1, height - 2);
    finish = Flocation(out, 0, 1);

     // PARAMS ARE X, Y, Z 
    IterateSpecific(out, 0, 1, "NorthSouth");
    IterateSpecific(in, height - 1, height - 2, "NorthSouth");



    tilemap[At(out, 0, 0)]->finish = true;
    tilemap[At(in, height - 1, height - 1)]->start = true;

    //TODO: Optimize
    for (int8 z = 0; z < height; z++) {
        for (int8 y = 0; y < height; y++) {
            for (int8 x = 0; x < height; x++) {
                if ((z == 0 || z == height - 1) || (y == 0 || y == height - 1) || (x == 0 || x == height - 1)) {
                    IterateSpecific(x, y, z, "Blank"); 
                    
                }
            }
        }
    }
}

// Debugging helper mostly
void Awfc::LogTilemap() {

    UE_LOG(LogTemp, Warning, TEXT("------ START -------"));
    start.Display();
    UE_LOG(LogTemp, Warning, TEXT("------ FINISH -------"));
    finish.Display();


    UE_LOG(LogTemp, Warning, TEXT("------LEFT MOST PATH : %s -------"), LMP_solved? TEXT("True"): TEXT("False"));
    for (auto& s : LeftMostPath) {
        s.Display();
    }

    UE_LOG(LogTemp, Warning, TEXT("------RIGHT MOST PATH: %s -------"), RMP_solved? TEXT("True") : TEXT("False"));
    for (auto& s : RightMostPath) {
        s.Display();
    }
    
    UE_LOG(LogTemp, Warning, TEXT("------UP MOST PATH: %s -------"), UMP_solved? TEXT("True") : TEXT("False"));
    for (auto& s : UpMostPath) {
        s.Display();
    }
    
    UE_LOG(LogTemp, Warning, TEXT("------Euclidian PATH: %s -------"), Euclidian_solved? TEXT("True") : TEXT("False"));
    for (auto& s : EuclidianPath) {
        s.Display();
    }

}

void Awfc::Solidify()
{
    for (auto& Block : tilemap) {
        if (Block->superPositionsTSC.Num() == 0) continue;
        BB_refs.Add(Block->Solidify());
    }
}

void Awfc::CreateMaze() {
    if (Already_Generated) {
        return; 
    }
    WFCInit();
    MazeHelper();
    while (1) {
        try {
            Iterate();
        }
        catch (...) {
            break;
        }
    }
    Solidify();
    Already_Generated = true; 
    //bool valid = validate();

    //validPath();
    //LogTilemap();
    //UE_LOG(LogTemp, Warning, TEXT("Successful?: %s"), validate ? "True" : "False");
}

void Awfc::ClearTiles()
{
    //Remove all BB
    for (auto& BB : BB_refs) {
        BB->Destroy();
    }
    BB_refs.Empty();

    // Remove all WFC Blocks
    for (auto& tile : tilemap) {
        tile->Destroy();
    }
    tilemap.Empty();


    // Clearing paths
    LeftMostPath.Empty();
    RightMostPath.Empty();
    UpMostPath.Empty();
    EuclidianPath.Empty();

    LMP_steps = 0; 
    RMP_steps = 0;
    UMP_steps = 0; 
    Euclidian_steps = 0; 

    LMP_solved = false; 
    RMP_solved = false; 
    UMP_solved = false; 
    Euclidian_solved = false; 


    Already_Generated = false; 
    Already_Solved = false; 
}

void Awfc::ClearBBREFS()
{
    for (int i = 0; i < BB_refs.Num(); i++) {
        BB_refs[i]->Destroy();
    }
    BB_refs.Empty();
}

void Awfc::SpawnBlockAtLocation(Flocation loc)
{
    BB_refs.Add(tilemap[At(loc)]->Solidify());
}
