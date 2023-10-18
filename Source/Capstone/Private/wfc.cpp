// Fill out your copyright notice in the Description page of Project Settings.


#include "wfc.h"

// DONE
TArray<TArray<TArray<AwfcBlock*>>> Awfc::GetTilemap()
{
	return tilemap;
}

// DONE
int8 Awfc::GetNumThreads()
{
	return threads;
}

// DONE
void Awfc::SetTilemap(TArray<TArray<TArray<AwfcBlock*>>> map)
{
	tilemap = map; 
}

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

    if (tilemap[loc.z][loc.y][loc.x]->superPositions[0]->connections[(int8)DIRECTION::WEST] ==
        true &&
        loc.x != 0)
        neighborTup.Add(
            Neighbor(DIRECTION::EAST, location(loc.x - 1, loc.y, loc.z)));
    if (tilemap[loc.z][loc.y][loc.x]->superPositions[0]->connections[(int8)DIRECTION::NORTH] ==
        true &&
        loc.y != 0)
        neighborTup.Add(
            Neighbor(DIRECTION::SOUTH, location(loc.x, loc.y - 1, loc.z)));
    if (tilemap[loc.z][loc.y][loc.x]->superPositions[0]->connections[(int8)DIRECTION::EAST] ==
        true &&
        loc.x < height - 1)
        neighborTup.Add(
            Neighbor(DIRECTION::WEST, location(loc.x + 1, loc.y, loc.z)));
    if (tilemap[loc.z][loc.y][loc.x]->superPositions[0]->connections[(int8)DIRECTION::SOUTH] ==
        true &&
        loc.y < width - 1)
        neighborTup.Add(
            Neighbor(DIRECTION::NORTH, location(loc.x, loc.y + 1, loc.z)));

    // NEW CHANGES FOR 3D
    if (tilemap[loc.z][loc.y][loc.x]->superPositions[0]->connections[(int8)DIRECTION::UP] == true &&
        loc.z != 0) {
        neighborTup.Add(
            Neighbor(DIRECTION::DOWN, location(loc.x, loc.y, loc.z - 1)));
    }
    if (tilemap[loc.z][loc.y][loc.x]->superPositions[0]->connections[(int8)DIRECTION::DOWN] ==
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
                if ((int8)tilemap[layer][row][col]->superPositions.Num() < min &&
                    tilemap[layer][row][col]->superPositions.Num() > 1) {
                    min = tilemap[layer][row][col]->superPositions.Num();
                    locs.Empty();
                }
                if ((int8)tilemap[layer][row][col]->superPositions.Num() == min) {
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
                if (tilemap[layer][row][col]->needsUpdate)
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
                if (tilemap[layer][row][col]->needsUpdate) needsUpdate = true;
            }
        }
    }
    return needsUpdate;
}

// DONE
void Awfc::resetUpdateValues()
{
    for (int8 layer = 0; layer < height; layer++) {
//#pragma omp parallel for num_threads(threads)
        for (int8 row = 0; row < height; row++) {
            for (int8 col = 0; col < width; col++) {
                tilemap[layer][row][col]->needsUpdate = false;
            }
        }
    }
}


// WIP 
bool Awfc::add_constraint(Neighbor& neighbor, TArray<ABuildingBlock*>& sourceSuperPositions)
{
    // Get the sides of each tile we'll be constraining by
    DIRECTION incDirection = neighbor.dir;
    DIRECTION outDirection = reverse(incDirection);
    bool changed = false;
    TSet<bool> neighborConstraint;

    // Get set of constraints from our source tile
    for (auto& superPosition : sourceSuperPositions) {
        neighborConstraint.Add(superPosition->connections[(int8)outDirection]);
    }

    // Only want to constrain neighbors that haven't collapsed
    if ((int8)tilemap[neighbor.loc.z][neighbor.loc.y][neighbor.loc.x]
        ->superPositions.Num() != 1) {
        TArray<int8> indicies_to_remove;
        for (int8 i = 0;
            i < (int8)tilemap[neighbor.loc.z][neighbor.loc.y][neighbor.loc.x]
            ->superPositions.Num();
            i++) {
            auto searchVal = tilemap[neighbor.loc.z][neighbor.loc.y][neighbor.loc.x]
                ->superPositions[i]
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
            tilemap[neighbor.loc.z][neighbor.loc.y][neighbor.loc.x]
                ->RemoveSuperPositionAtIndex(indicies_to_remove[i]);
        }
    }

    // failsafe: if we happen to remove all options for a tile throw an error
    if ((int8)tilemap[neighbor.loc.z][neighbor.loc.y][neighbor.loc.x]
        ->superPositions.Num() == 0) {
        //cerr << "LocX: " << neighbor.loc.x << " LocY: " << neighbor.loc.y
        //    << "LocZ: " << neighbor.loc.z << endl;
        throw std::runtime_error("No patterns left at location.");
    }
    return changed;
}


// DONE? 
void Awfc::Propogate(location next_location)
{
    // sets our starting point for propagation
    resetUpdateValues();
    tilemap[next_location.z][next_location.x][next_location.y]->needsUpdate = true;

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
                    tilemap[location.z][location.y][location.x]->superPositions);

                // If any constraints were added, mark it as needing to update so we
                // propagate from this location next
                tilemap[neighbor.loc.z][neighbor.loc.y][neighbor.loc.x]->needsUpdate |=
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
    if ((int8)tilemap[next_location.z][next_location.y][next_location.x]
        ->superPositions.Num() < 2) {
        throw(std::runtime_error("No choices at tile location"));
    }
    tilemap[next_location.z][next_location.y][next_location.x]
        ->CollapseSuperPositions();
    Propogate(next_location);
}


// DONE
void Awfc::IterateSpecific(int8 x, int8 y, int8 z, FString name)
{
    tilemap[z][y][x]->CollapseSpecific(name);
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
                    //      << tilemap[i][j].superPositions[0].connections[cur_dir]
                    //      << "   type:" << tilemap[i][j].superPositions[0].density
                    //      << " \nTo neighbor dir:" << neighbor_dir << "   conn:"
                    //      << tilemap[neighbors[k].loc.y][neighbors[k].loc.x]
                    //             .superPositions[0]
                    //             .connections[neighbor_dir]
                    //      << "   type:"
                    //      << tilemap[neighbors[k].loc.y][neighbors[k].loc.x]
                    //             .superPositions[0]
                    //             .density;

                    // first element is the connection of the cur tile while the 2nd is
                    // the connection of its touching neighbor
                    if (tilemap[h][i][j]->superPositions[0]->connections[(int8)cur_dir] !=
                        tilemap[neighbors[k].loc.z][neighbors[k].loc.y]
                        [neighbors[k].loc.x]
                        ->superPositions[0]
                        ->connections[(int8)neighbor_dir]) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}


// DONE
bool Awfc::validPath()
{
    for (auto& layer : tilemap) {
        for (auto& row : layer) {
            for (auto& tile : row) {
                tile->superPositions[0]->visited = false;
            }
        }
    }
    validPathRecur(location(start.x, start.y, start.z));

    if (tilemap[finish.z][finish.y][finish.x]->superPositions[0]->visited == true) {
        return true;
    }
    return false;
}


// DONE
void Awfc::validPathRecur(location next)
{
    tilemap[next.z][next.y][next.x]->superPositions[0]->visited = true;
    TArray<Neighbor> tile_neighbors = get_neighbors_by_tf(next);

    // debugging
    // cout << "Looking at "
    //      << tilemap[next.z][next.y][next.x].superPositions[0].name << " tile.
    //      ";
    // cout << "# of neighbors at this tile: " << tile_neighbors.size() << endl;

    for (auto& neighbor : tile_neighbors) {
        // cout << "\t NEIGHBOR: " << endl;
        // neighbor.loc.Display();
        // if not at the end then keep searching
        if (tilemap[neighbor.loc.z][neighbor.loc.y][neighbor.loc.x]
            ->superPositions[0]
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
    tilemap[0][0][out]->superPositions[0]->finish = true;
    tilemap[height - 1][height - 1][in]->superPositions[0]->start = true;

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


// Debugging helper mostly : NOT CURRENTLY BEING CALLED
void Awfc::LogTilemap() {

    int8 count = 0; 
    for (auto& layer : tilemap) {
        UE_LOG(LogTemp, Warning, TEXT("Layer: %d"), count);
        for (auto& row : layer) {
        
            FString tiles = "[ ";
            for (auto tile : row) {
                tiles += FString::FromInt(tile->superPositions[0]->density);
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
    LogTilemap();
    //UE_LOG(LogTemp, Warning, TEXT("Successful?: %s"), validate ? "True" : "False");
}
