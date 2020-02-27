#include "precomp.h"
#pragma once
#include "Grid.h"

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

void Grid::AddTank(Tank* tank)
{
    Cell* cell = GetCell(tank->position);
    if (cell != nullptr){
        cell->tanks.push_back(tank);
        tank->ownerCell = cell;
        tank->cellIndex = cell->tanks.size() - 1;
    }
}

void Grid::AddTank(Tank* tank, Cell* cell){
    cell->tanks.push_back(tank);
    tank->ownerCell = cell;
    tank->cellIndex = cell->tanks.size() - 1;
}

Cell* Grid::GetCell(int x, int y){
    if (x < 0) x = 0;
    if (x >= numXcells) x = numXcells;
    if (y < 0) y = 0;
    if (y >= numYcells) y = numYcells;
    return &m_cells[(numYcells - y) + (numXcells - x) - 1];
}

Cell* Grid::GetCell(const vec2& pos)
{
    int cellX = (int)(pos.x / m_cellSize);
    int cellY = (int)(pos.y / m_cellSize);

    return GetCell(cellX, cellY);
}

vector<Cell*> Grid::GetCells(Particle_beam* beam){
    vector<Cell*> cells;
    int minx = (int)(beam->min_position.x / m_cellSize);
    int miny = (int)(beam->min_position.y / m_cellSize);

    int midx = (int)((beam->min_position.x + CELL_SIZE) / m_cellSize);
    int midy = (int)(beam->min_position.y / m_cellSize);

    int maxx = (int)(beam->max_position.x / m_cellSize);
    int maxy = (int)(beam->max_position.y / m_cellSize);
    cells.push_back(GetCell(minx, miny));
    cells.push_back(GetCell(midx, midy));
    cells.push_back(GetCell(maxx, maxy));

    return cells;
}

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

void Grid::SwitchCells(Tank* tank, Cell* cell){
    RemoveTank(tank);
    AddTank(tank, cell);
}
