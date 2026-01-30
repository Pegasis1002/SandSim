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
  int x_offset;
  int y_offset;
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

  grid.x_offset = 0;
  grid.y_offset = 0;

  size_t totalCells = grid.width * grid.height;

  grid.cells = (uint8_t *)calloc(totalCells, sizeof(uint8_t));
  if (grid.cells == NULL) {
    return 1;
  }

  grid.cell_buff = (uint8_t *)calloc(totalCells, sizeof(uint8_t));
  if (grid.cell_buff == NULL) {
    return 1;
  }

  grid.cells[2] = 1; grid.cells[3] = 1; grid.cells[12] = 1; grid.cells[13] = 1;

  size_t totalSize = totalCells * sizeof(uint8_t);

  srand(time(NULL));
  while (!WindowShouldClose()) {

    // Sim logic
    for (int y = grid.height - 1; y >= 0; y--) {
      for (int x = 0; x < grid.width; x++) {
        int index = (y * grid.width) + x;
        uint8_t cell = grid.cells[index];

        if (cell == 0) continue;

        if (cell == 2) {
          grid.cell_buff[index] = 2;
          continue;
        }

        if (y == grid.height - 1) {
          grid.cell_buff[index] = 1;
          continue;
        }

        int b  = index + grid.width;
        int bl = b - 1;
        int br = b + 1;

        if (grid.cells[b] == 0) {
          grid.cell_buff[b] = 1;
        } else {
          bool can_go_left  = (x > 0) && (grid.cells[bl] == 0);
          bool can_go_right = (x < grid.width - 1) && (grid.cells[br] == 0);

          if (can_go_left && can_go_right) {
            grid.cell_buff[rand() % 2 == 0 ? bl : br] = 1;
          } else if (can_go_left) {
            grid.cell_buff[bl] = 1;
          } else if (can_go_right) {
            grid.cell_buff[br] = 1;
          } else {
            grid.cell_buff[index] = 1;
          }
        }
      }
    }

    // Buffer Swap
    uint8_t * temp = grid.cells;
    grid.cells = grid.cell_buff;
    grid.cell_buff = temp;

    memset(grid.cell_buff, 0, totalSize);

    // Draw
    BeginDrawing();
    ClearBackground(BLACK);

    for (int i = 0; i < totalCells; i++) {
      if (grid.cells[i] != 0) {
        int dx = ((i % grid.width) * CELL_DIM) + grid.x_offset;
        int dy = ((i / grid.width) * CELL_DIM) + grid.y_offset;

        Color c = (grid.cells[i] == 1) ? YELLOW : GRAY;
        DrawRectangle(dx, dy, CELL_DIM - 1, CELL_DIM - 1, c);
      }
    }

    EndDrawing();

  }
  CloseWindow();
  return 0;
}
