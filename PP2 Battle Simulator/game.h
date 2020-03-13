#pragma once
#include "Grid.h"
#include <memory>

const size_t CELL_SIZE = 80;

namespace Tmpl8
{
//forward declarations
class Tank;
class Rocket;
class Smoke;
class Particle_beam;

class Game
{
  public:
    ~Game();
    void SetTarget(Surface* surface) { screen = surface; }
    void Init();
    void Shutdown();
    void Update(float deltaTime);
    void Draw();
    void Tick(float deltaTime);
    void insertion_sort_tanks_health(const std::vector<Tank>& original, std::vector<const Tank*>& sorted_tanks, UINT16 begin, UINT16 end);
    vector<int> countSort(const std::vector<Tank>& toSort);
    void MeasurePerformance();

    void UpdateGameTanks();
    void UpdateGameRockets();
    void UpdateGameLaserBeams();
    void UpdateGameExplosions();

    Tank& FindClosestEnemy(Tank& current_tank);

    void MouseUp(int button)
    { /* implement if you want to detect mouse button presses */
    }

    void MouseDown(int button)
    { /* implement if you want to detect mouse button presses */
    }

    void MouseMove(int x, int y)
    { /* implement if you want to detect mouse movement */
    }

    void KeyUp(int key)
    { /* implement if you want to handle keys */
    }

    void KeyDown(int key)
    { /* implement if you want to handle keys */
    }

  private:
    Surface* screen;

    vector<Tank> tanks;
    vector<Tank*> Bluetanks;
    vector<Tank*> Redtanks;

    vector<Rocket> rockets;
    vector<Smoke> smokes;
    vector<Explosion> explosions;
    vector<Particle_beam> particle_beams;


    Font* frame_count_font;
    long long frame_count = 0;

    bool lock_update = false;

    std::unique_ptr<ThreadPool> tPool;

    std::unique_ptr<Grid> m_grid; // grid voor spatial collision
};

}; // namespace Tmpl8