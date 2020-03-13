#include "precomp.h"
#pragma once
#include "Grid.h"

std::mutex mut;

Grid::Grid(int width, int height, int cellSize) : m_width(width),
                                                  m_height(height),
                                                  m_cellSize(cellSize)
{
    numXcells = ceil((float)m_width / m_cellSize);
    numYcells = ceil((float)m_height / m_cellSize);
    m_cells.resize(numYcells * numXcells);
}

Grid::~Grid()
{
}

//add tank to list of tanks from cell, where cell is unknown
void Grid::AddTank(Tank* tank)
{
    std::lock_guard<std::mutex> lock(mut);
    Cell* cell = GetCell(tank->position);
    if (cell != nullptr)
    {
        cell->tanks.push_back(tank);
        tank->ownerCell = cell;
        tank->cellIndex = cell->tanks.size() - 1;
    }
}

//add tank to list of tanks from cell, where cell is known
void Grid::AddTank(Tank* tank, Cell* cell)
{
    cell->tanks.push_back(tank);
    tank->ownerCell = cell;
    tank->cellIndex = cell->tanks.size() - 1;
}

//gets cell using x and y coordinates
Cell* Grid::GetCell(int x, int y)
{
    if (x < 0) x = 0;
    if (x >= numXcells) x = numXcells;
    if (y < 0) y = 0;
    if (y >= numYcells) y = numYcells;
    return &m_cells[(numYcells - y) + (numXcells - x) - 1];
}

//gets cell using position vect2
Cell* Grid::GetCell(const vec2& pos)
{
    int cellX = (int)(pos.x / m_cellSize);
    int cellY = (int)(pos.y / m_cellSize);

    return GetCell(cellX, cellY);
}

//gets ownercell and neighbour cells of tank
vector<Cell*> Grid::GetNeighbours(Tank* tank)
{
    vector<Cell*> cells;

    //set x and y
    int x = tank->position.x;
    int y = tank->position.y;
    if (x < 0) x = 0;
    if (x >= numXcells) x = numXcells;
    if (y < 0) y = 0;
    if (y >= numYcells) y = numYcells;

    //check if there are  is a neighbour cell left
    if (x > CELL_SIZE)
    {
        Cell* left = &m_cells[(x - CELL_SIZE) + (y)];
        cells.push_back(left);
        //check if there are is a neighbour cell down
        if (y > CELL_SIZE)
        {
            Cell* down = &m_cells[(x) + (y - CELL_SIZE)];
            cells.push_back(down);
            Cell* downleft = &m_cells[(x - CELL_SIZE) + (y - CELL_SIZE)];
            cells.push_back(downleft);
            Cell* downright = &m_cells[(x + CELL_SIZE) + (y + CELL_SIZE)];
            cells.push_back(downright);
        }
    }
    //check if there are is a neighbour cell right
    if (x < 1280 - CELL_SIZE)
    {
        Cell* right = &m_cells[(x + CELL_SIZE) + (y)];
        cells.push_back(right);
        //check if there are is a neighbour cell down
        if (y < 720 - CELL_SIZE)
        {
            Cell* up = &m_cells[(x) + (y + CELL_SIZE)];
            cells.push_back(up);
            Cell* upright = &m_cells[(x + CELL_SIZE) + (y - CELL_SIZE)];
            cells.push_back(upright);
            Cell* upleft = &m_cells[(x - CELL_SIZE) + (y + CELL_SIZE)];
            cells.push_back(upleft);
        }
    }

    //Cell* neighbour = &m_cells[(numYcells - y) + (numXcells - x)];
    cells.push_back(tank->ownerCell);
    return cells;
}

//gets cells of particle_beam
//as each beam has an area of 100x50(see particle_beam.cpp), 3 cells are always enough(cell_size = 80)
vector<Cell*> Grid::GetCells(Particle_beam* beam)
{
    vector<Cell*> cells;
    //get cell of beams min position
    int minx = (int)(beam->min_position.x / m_cellSize);
    int miny = (int)(beam->min_position.y / m_cellSize);

    //get cell of beams middle position
    int midx = (int)((beam->min_position.x + CELL_SIZE) / m_cellSize);
    int midy = (int)(beam->min_position.y / m_cellSize);

    //get cell of beams max position
    int maxx = (int)(beam->max_position.x / m_cellSize);
    int maxy = (int)(beam->max_position.y / m_cellSize);
    cells.push_back(GetCell(minx, miny));
    cells.push_back(GetCell(midx, midy));
    cells.push_back(GetCell(maxx, maxy));

    return cells;
}

//remove tank from cell's list of tanks
void Grid::RemoveTank(Tank* tank)
{
    std::vector<Tank*>& tanks = tank->ownerCell->tanks;
    //vector swap
    tanks[tank->cellIndex] = tanks.back();
    tanks.pop_back();
    //update index
    if (tank->cellIndex < tanks.size())
    {
        tanks[tank->cellIndex]->cellIndex = tank->cellIndex;
    }
    //set index of tank to -1
    tank->cellIndex = -1;
    tank->ownerCell = nullptr;
}

//used when tank crosses a cell border, removed from old cell and added in new cell
void Grid::SwitchCells(Tank* tank, Cell* cell)
{
    std::lock_guard<std::mutex> lock(mut);
    RemoveTank(tank);
    AddTank(tank, cell);
}
