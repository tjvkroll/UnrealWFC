// Fill out your copyright notice in the Description page of Project Settings.


#include "wfc.h"

int16 Awfc::At(int8 x, int8 y, int8 z)
{
    return (z * height) + (y * height) + z; 
}

//// DONE
//TArray<TArray<TArray<AwfcBlock*>>> Awfc::GetTilemap()
//{
//	return tilemap;
//}

// DONE
//int8 Awfc::GetNumThreads()
//{
//	return threads;
//}

//// DONE
//void Awfc::SetTilemap(TArray<TArray<TArray<AwfcBlock*>>> map)
//{
//	tilemap = map; 
//}

// DONE 
TArray<Awfc::Neighbor> Awfc::get_neighbors(location loc, int8 h)
{
    TArray<Neighbor> neighborTup;
    if (loc.x != 0)
        neighborTup.Add(
            Neighbor(DIRECTION::EAST, location(loc.x - 1, loc.y, loc.z)));
    if (loc.y != 0)
        neighborTup.Add(
            Neighbor(DIRECTION::SOUTH, location(loc.x, loc.y - 1, loc.z)));
    if (loc.x < h - 1)
        neighborTup.Add(
            Neighbor(DIRECTION::WEST, location(loc.x + 1, loc.y, loc.z)));
    if (loc.y < h - 1)
        neighborTup.Add(
            Neighbor(DIRECTION::NORTH, location(loc.x, loc.y + 1, loc.z)));
    // New changes for 3D
    if (loc.z != 0) {
        neighborTup.Add(
            Neighbor(DIRECTION::UP, location(loc.x, loc.y, loc.z - 1)));
    }
    if (loc.z < h - 1) {
        neighborTup.Add(
            Neighbor(DIRECTION::DOWN, location(loc.x, loc.y, loc.z + 1)));
    }
    return neighborTup;
}



// DONE
TArray<Awfc::Neighbor> Awfc::get_neighbors_by_tf(location loc)
{
    TArray<Neighbor> neighborTup;

    if (tilemap[At(loc.x, loc.y, loc.z)]->superPositionsTSC[0].GetDefaultObject()->connections[(int8)DIRECTION::WEST] ==
        true &&
        loc.x != 0)
        neighborTup.Add(
            Neighbor(DIRECTION::EAST, location(loc.x - 1, loc.y, loc.z)));
    if (tilemap[At(loc.x, loc.y, loc.z)]->superPositionsTSC[0].GetDefaultObject()->connections[(int8)DIRECTION::NORTH] ==
        true &&
        loc.y != 0)
        neighborTup.Add(
            Neighbor(DIRECTION::SOUTH, location(loc.x, loc.y - 1, loc.z)));
    if (tilemap[At(loc.x, loc.y, loc.z)]->superPositionsTSC[0].GetDefaultObject()->connections[(int8)DIRECTION::EAST] ==
        true &&
        loc.x < height - 1)
        neighborTup.Add(
            Neighbor(DIRECTION::WEST, location(loc.x + 1, loc.y, loc.z)));
    if (tilemap[At(loc.x, loc.y, loc.z)]->superPositionsTSC[0].GetDefaultObject()->connections[(int8)DIRECTION::SOUTH] ==
        true &&
        loc.y < width - 1)
        neighborTup.Add(
            Neighbor(DIRECTION::NORTH, location(loc.x, loc.y + 1, loc.z)));

    // NEW CHANGES FOR 3D
    if (tilemap[At(loc.x, loc.y, loc.z)]->superPositionsTSC[0].GetDefaultObject()->connections[(int8)DIRECTION::UP] == true &&
        loc.z != 0) {
        neighborTup.Add(
            Neighbor(DIRECTION::DOWN, location(loc.x, loc.y, loc.z - 1)));
    }
    if (tilemap[At(loc.x, loc.y, loc.z)]->superPositionsTSC[0].GetDefaultObject()->connections[(int8)DIRECTION::DOWN] ==
        true &&
        loc.z < width - 1) {
        neighborTup.Add(
            Neighbor(DIRECTION::UP, location(loc.x, loc.y, loc.z + 1)));
    }

    return neighborTup;
}


// DONE
Awfc::location Awfc::get_location_with_fewest_choices()
{
    int8 min = 100;
    TArray<location> locs;
    for (int8 layer = 0; layer < height; layer++) {
        for (int8 row = 0; row < height; row++) {
            for (int8 col = 0; col < height; col++) {
                if ((int8)tilemap[At(col,row,layer)]->superPositionsTSC.Num() < min &&
                    tilemap[At(col, row, layer)]->superPositionsTSC.Num() > 1) {
                    min = tilemap[At(col, row, layer)]->superPositionsTSC.Num();
                    locs.Empty();
                }
                if ((int8)tilemap[At(col, row, layer)]->superPositionsTSC.Num() == min) {
                    locs.Add(location(col, row, layer));
                }
            }
        }
    }

    if (locs.Num() != 0) {
        int randindx = rand() % locs.Num();
        return locs[randindx];
    }
    else {
        return location();
    }
}


// DONE
TArray<Awfc::location> Awfc::get_locations_requiring_updates()
{
    TArray<location> locs;

    // PRAGMA CANDIDATE
    for (int8 layer = 0; layer < height; layer++) {
//#pragma omp parallel for num_threads(threads)
        for (int8 row = 0; row < height; row++) {
//#pragma omp parallel for num_threads(threads)
            for (int8 col = 0; col < width; col++) {
                if (tilemap[At(col, row, layer)]->needsUpdate)
                    locs.Add(location(row, col, layer));
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
    //#pragma omp parallel for num_threads(threads)
        for (int8 row = 0; row < height; row++) {
    //#pragma omp parallel for num_threads(threads)
            for (int8 col = 0; col < width; col++) {
                if (tilemap[At(col, row, layer)]->needsUpdate) needsUpdate = true;
            }
        }
    }
    return needsUpdate;
}




// DEALING WITH INSTANCE OF BLOCKS

// DONE
void Awfc::resetUpdateValues()
{
    for (int8 layer = 0; layer < height; layer++) {
//#pragma omp parallel for num_threads(threads)
        for (int8 row = 0; row < height; row++) {
            for (int8 col = 0; col < width; col++) {
                tilemap[At(col, row, layer)]->needsUpdate = false;
            }
        }
    }
}


// WIP 
bool Awfc::add_constraint(Neighbor& neighbor, TArray<TSubclassOf<ABuildingBlock>>& sourceSuperPositions)
{
    // Get the sides of each tile we'll be constraining by
    DIRECTION incDirection = neighbor.dir;
    DIRECTION outDirection = reverse(incDirection);
    bool changed = false;
    TSet<bool> neighborConstraint;

    // Get set of constraints from our source tile
    for (auto& superPosition : sourceSuperPositions) {
        neighborConstraint.Add(superPosition.GetDefaultObject()->connections[(int8)outDirection]);
    }

    // Only want to constrain neighbors that haven't collapsed
    if ((int8)tilemap[At(neighbor.loc.x, neighbor.loc.y, neighbor.loc.z)]
        ->superPositionsTSC.Num() != 1) {
        TArray<int8> indicies_to_remove;
        for (int8 i = 0;
            i < (int8)tilemap[At(neighbor.loc.x, neighbor.loc.y, neighbor.loc.z)]
            ->superPositionsTSC.Num();
            i++) {
            auto searchVal = tilemap[At(neighbor.loc.x, neighbor.loc.y, neighbor.loc.z)]
                ->superPositionsTSC[i].GetDefaultObject()
                ->connections[(int8)incDirection];
            if (!neighborConstraint.Contains(searchVal)) {
                // remove superPosition at i
                indicies_to_remove.Add(i);
                changed = true;
            }
        }

        // Remove building blocks that don't fit based on the constraints of our
        // source
        for (int8 i = indicies_to_remove.Num() - 1; i >= 0; i--) {
            tilemap[At(neighbor.loc.x, neighbor.loc.y, neighbor.loc.z)]
                ->RemoveSuperPositionAtIndex(indicies_to_remove[i]);
        }
    }

    // failsafe: if we happen to remove all options for a tile throw an error
    if ((int8)tilemap[At(neighbor.loc.x, neighbor.loc.y, neighbor.loc.z)]
        ->superPositionsTSC.Num() == 0) {
        //cerr << "LocX: " << neighbor.loc.x << " LocY: " << neighbor.loc.y
        //    << "LocZ: " << neighbor.loc.z << endl;
        throw std::runtime_error("No patterns left at location.");
    }
    return changed;
}





// DEALING WITH INSTANCE OF BLOCKS  (266-267)

// DONE? 
void Awfc::Propogate(location next_location)
{
    // sets our starting point for propagation
    resetUpdateValues();
    tilemap[At(next_location.x, next_location.y, next_location.z)]->needsUpdate = true;

    // Loop to continue propagating until there are no more tiles that have been
    // constrained
    while (updatesRequired()) {
        // Gathers a list of locations that need to be propagated from
        TArray<location> locations = get_locations_requiring_updates();
        resetUpdateValues();
        for (auto& location : locations) {
            // Gets all neighbors of the location we're propagating from
            TArray<Neighbor> neighbors = get_neighbors(location, height);

            // PRAGMA CANDIDATE
//#pragma omp parallel for num_threads(threads)
            for (auto& neighbor : neighbors) {
                // Applying constraints to each neighbor of our source(location)
                bool wasUpdated = add_constraint(
                    neighbor,
                    tilemap[At(neighbor.loc.x, neighbor.loc.y, neighbor.loc.z)]->superPositionsTSC);

                // If any constraints were added, mark it as needing to update so we
                // propagate from this location next
                tilemap[At(neighbor.loc.x, neighbor.loc.y, neighbor.loc.z)]->needsUpdate |=
                    wasUpdated;
            }
        }
    }

}


// CAN WE THROW ERRORS IN THE SAME WAY WE NORMALLY WOULD
void Awfc::Iterate()
{
    location next_location = get_location_with_fewest_choices();

    // failsafe / end if iterating
    if (next_location.x == -1) {
        throw(std::runtime_error("No more tiles to collapse!"));
    }
    if ((int8)tilemap[At(next_location.x, next_location.y, next_location.z)]
        ->superPositionsTSC.Num() < 2) {
        throw(std::runtime_error("No choices at tile location"));
    }
    tilemap[At(next_location.x, next_location.y, next_location.z)]
        ->CollapseSuperPositions();
    Propogate(next_location);
}


// DONE
void Awfc::IterateSpecific(int8 x, int8 y, int8 z, FString name)
{
    tilemap[At(x,y,z)]->CollapseSpecific(name);
    location collapsed(x, y, z);
    Propogate(collapsed);
}


// DONE
bool Awfc::validate()
{
    // Loops to work through entire tilemap
    for (int8 h = 0; h < height; h++) {
        for (int8 i = 0; i < height; i++) {
            for (int8 j = 0; j < height; j++) {
                location cur(j, i, h);
                TArray<Neighbor> neighbors = get_neighbors(cur, height);
                // compare current tile to its neighbors for valid connections
                for (int8 k = 0; k < neighbors.Num(); k++) {
                    // get the indexes of "connections" for cur tile and its neighbor
                    DIRECTION neighbor_dir = neighbors[k].dir;
                    DIRECTION cur_dir = reverse(neighbor_dir);

                    // cout << "\nComparing home dir:" << cur_dir << boolalpha << " conn:"
                    //      << tilemap[i][j].superPositionsTSC[0].connections[cur_dir]
                    //      << "   type:" << tilemap[i][j].superPositionsTSC[0].density
                    //      << " \nTo neighbor dir:" << neighbor_dir << "   conn:"
                    //      << tilemap[neighbors[k].loc.y][neighbors[k].loc.x]
                    //             .superPositionsTSC[0]
                    //             .connections[neighbor_dir]
                    //      << "   type:"
                    //      << tilemap[neighbors[k].loc.y][neighbors[k].loc.x]
                    //             .superPositionsTSC[0]
                    //             .density;

                    // first element is the connection of the cur tile while the 2nd is
                    // the connection of its touching neighbor
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

void Awfc::init()
{
    for (int16 i = 0; i < height * height * height; i++) {

        FVector worldCoords(1, 1, 1);

        tilemap.Add(GetWorld()->SpawnActor<AwfcBlock>(block_class, worldCoords, FRotator()));
    }
}


// DONE
bool Awfc::validPath()
{
    for (int8 z = 0; z < height; z++) {
        for (int8 y = 0; y < height; y++) {
            for (int8 x = 0; x < height; x++) {
                tilemap[At(x, y, z)]->visited = false;

                //tilemap[At(x, y, z)].superPositionsTSC[0]->visited = false;
            }
        }
    }
    validPathRecur(location(start.x, start.y, start.z));
    
    // was before moving visited, start, finish ect to AwfcBlock
    //if (tilemap[At(finish.x, finish.y, finish.z)].superPositionsTSC[0]->visited == true) {


    if (tilemap[At(finish.x, finish.y, finish.z)]->visited == true) {
        return true;
    }
    return false;
}


// DONE
void Awfc::validPathRecur(location next)
{

    //tilemap[At(next.x, next.y, next.z)].superPositionsTSC[0]->visited = true;

    tilemap[At(next.x, next.y, next.z)]->visited = true;
    TArray<Neighbor> tile_neighbors = get_neighbors_by_tf(next);

    // debugging
    // cout << "Looking at "
    //      << tilemap[next.z][next.y][next.x].superPositionsTSC[0].name << " tile.
    //      ";
    // cout << "# of neighbors at this tile: " << tile_neighbors.size() << endl;

    for (auto& neighbor : tile_neighbors) {
        // cout << "\t NEIGHBOR: " << endl;
        // neighbor.loc.Display();
        // if not at the end then keep searching

        //if (tilemap[At(neighbor.loc.x, neighbor.loc.y, neighbor.loc.z)]
        //    .superPositionsTSC[0]
        //    ->visited == false) {


        if (tilemap[At(neighbor.loc.x, neighbor.loc.y, neighbor.loc.z)]
            ->visited == false) {
            validPathRecur(neighbor.loc);
        }
    }
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
    start = location(in, height - 1, height - 1);
    finish = location(out, 0, 0);

    // setting in and out
    IterateSpecific(out, 0, 0, "vertPipe");
    IterateSpecific(in, height - 1, height - 1, "vertPipe");

    // Before moving start finish visited to AwfcBlock
    /*tilemap[At(out, 0, 0)].superPositionsTSC[0]->finish = true;
    tilemap[At(in, height - 1, height - 1)].superPositionsTSC[0]->start = true;*/



    tilemap[At(out, 0, 0)]->finish = true;
    tilemap[At(in, height - 1, height - 1)]->start = true;

    // border off T/B except for
    for (int8 h = 0; h < height; h++) {
        for (int8 i = 0; i < height; i++) {
            if (i != in && h != height - 1)
                IterateSpecific(i, height - 1, h, "Blank");
            if (i != out && h != 0) IterateSpecific(i, 0, h, "Blank");
        }
    }

    // border off L/R
    for (int8 h = 0; h < height; h++) {
        for (int8 i = 1; i < width - 1; i++) {
            IterateSpecific(0, i, h, "Blank");
            IterateSpecific(height - 1, i, h, "Blank");
        }
    }
}

void Awfc::Spawn()
{
    for (int8 z = 0; z < height; z++) {
        for (int8 y = 0; y < height; y++) {
            for (int8 x = 0; x < height; x++) {
                FVector worldCoords(10*x, 10*y, 10*z);
                GetWorld()->SpawnActor<ABuildingBlock>(tilemap[At(x, y,z)]->superPositionsTSC[0], worldCoords, FRotator());
            }
        }
    }

}


// Debugging helper mostly : NOT CURRENTLY BEING CALLED
void Awfc::LogTilemap() {

    int8 count = 0; 
    for (int8 z = 0; z < height; z++) {
        UE_LOG(LogTemp, Warning, TEXT("Layer: %d"), count);
        for (int8 y = 0; y < height; y++) {
        
            FString tiles = "[ ";
            for (int8 x = 0; x < height; x++) {
                tiles += FString::FromInt(tilemap[At(x,y,z)]->superPositionsTSC[0].GetDefaultObject()->density);
                tiles += " ";
            }
            tiles += "]";
            GLog->Log(tiles);
        }
        count++;
    }
}


void Awfc::CreateMaze() {
    MazeHelper();
    while (1) {
        try {
            Iterate();
        }
        catch (std::runtime_error) {
            break;
        }
    }
    validate();
    validPath();
    LogTilemap();
    Spawn();
    //UE_LOG(LogTemp, Warning, TEXT("Successful?: %s"), validate ? "True" : "False");
}
