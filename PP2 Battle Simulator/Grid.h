#pragma once
#include "Tank.h"
#include <vector>

class Cell
{
  public:
    std::vector<Tank*> tanks;
} S;

class Grid
{
  public:
    Grid(int width, int height, int cellSize);
    ~Grid();

    //adds tank to cell and determines what cell it is in
    void AddTank(Tank* tank);
    //adds tank to specified cell
    void AddTank(Tank* tank, Cell* cell);
    //gets cell using x,y coordinates
    Cell* GetCell(int x, int y);
    //gets cell using position
    Cell* GetCell(const vec2& pos);
    //removes tank from cell
    void RemoveTank(Tank* tank);
	//gets cells from particle_beam
    vector<Cell*> GetCells(Particle_beam* beam);

    // Remove and add in one function
    void SwitchCells(Tank* tank, Cell* cell);

  private:
    std::vector<Cell> m_cells;
    int m_cellSize;
    int m_width;
    int m_height;
    int numXcells;
    int numYcells;

};
