#include "raylib.h"
#include "sim.h"
#include <stdlib.h>
#include <time.h>

const int screenSize = GRID_SIZE * CELL_DIM;

int main(int argc, char *argv[])
{
  InitWindow(screenSize, screenSize, "Falling Sand PoC");
  SetTargetFPS(1);

  Grid grid = create_grid(GRID_SIZE, GRID_SIZE);
  if (grid.cells == NULL || grid.cell_buff == NULL) {
    return 1;
  }

  // Initial sand setup
  grid.cells[2] = SAND;
  grid.cells[3] = SAND;
  grid.cells[4] = SAND;
  grid.cells[13] = SAND;

  srand(time(NULL));
  while (!WindowShouldClose()) {
    update_simulation(&grid);

    BeginDrawing();
    ClearBackground(BLACK);
    draw_grid(&grid);
    EndDrawing();
  }

  destroy_grid(&grid);
  CloseWindow();
  return 0;
}
