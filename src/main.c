#include "raylib.h"
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>

#define GRID_SIZE 10
#define CELL_DIM 40

typedef enum {
  EMPTY = 0,
  SAND = 1,
  OBSTACLE =2
} CellType;

typedef struct {
  int width;
  int height;
  uint8_t *cells;
  uint8_t *cell_buff;
} Grid;

const int screenSize = GRID_SIZE * CELL_DIM;

int main(int argc, char *argv[])
{
  InitWindow(GRID_SIZE * CELL_DIM, GRID_SIZE*CELL_DIM, "Falling Sand PoC");
  SetTargetFPS(10);

  Grid grid;
  grid.width = GRID_SIZE;
  grid.height = GRID_SIZE;

  grid.cells = (uint8_t *)calloc(grid.width * grid.height, sizeof(uint8_t));
  if (grid.cells == NULL) {
    return 1;
  }

  grid.cell_buff = (uint8_t *)calloc(grid.width * grid.height, sizeof(uint8_t));
  if (grid.cell_buff == NULL) {
    return 1;
  }

  grid.cells[2] = 1; grid.cells[3] = 1; grid.cells[13] = 1;

  srand(time(NULL));
  while (!WindowShouldClose()) {

    // Sim logic
    for (int y = grid.height; y >= 0; y--) {
      for (int x = grid.width; x >= 0; x--) {
        int index = (y * grid.width) + x;

        int b = ((y+1) * grid.width) + x;
        int br = ((y+1) * grid.width) + x + 1;
        int bl = ((y+1) * grid.width) + x - 1;

        if (grid.cells[index] == 1) {
          if (grid.cells[b] == 0) {
            grid.cell_buff[b] = 1;
          } else {
            int bit = rand() % 2;
            if (bit == 0) {
              if (grid.cells[bl] == 0) {
                grid.cell_buff[bl] = 1;
              } else if (grid.cells[br] == 0) {
                grid.cell_buff[br] = 1;
              }
            } else if (bit == 1) {
              if (grid.cells[br] == 0) {
                grid.cell_buff[br] = 1;
              } else if (grid.cells[bl] == 0) {
                grid.cell_buff[bl] = 1;
              }
            }
          }
        }
      }
    }


    // Draw
    BeginDrawing();

    ClearBackground(BLACK);
    DrawText("Sand Simulation!!!", screenSize/2, screenSize/2, 20, WHITE);

    EndDrawing();

    // Buffer switching
    size_t totalSize = grid.width * grid.height * sizeof(uint8_t);
    memset(grid.cells, 0, totalSize);
    memcpy(grid.cells, grid.cell_buff, totalSize);
    memset(grid.cell_buff, 0, totalSize);
  }
  CloseWindow();
  return 0;
}
