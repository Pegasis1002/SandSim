#include "sim.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "raylib.h"

Grid create_grid(int width, int height) {
  Grid grid;
  grid.width = width;
  grid.height = height;
  grid.x_offset = 0;
  grid.y_offset = 0;

  size_t totalCells = (size_t)grid.width * grid.height;

  grid.cells = (uint8_t *)calloc(totalCells, sizeof(uint8_t));
  grid.cell_buff = (uint8_t *)calloc(totalCells, sizeof(uint8_t));

  return grid;
}

void destroy_grid(Grid *grid) {
  free(grid->cells);
  free(grid->cell_buff);
  grid->cells = NULL;
  grid->cell_buff = NULL;
}

void update_simulation(Grid *grid) {
    size_t totalCells = (size_t)grid->width * grid->height;
    memset(grid->cell_buff, EMPTY, totalCells * sizeof(uint8_t));

    for (int y = grid->height - 1; y >= 0; y--) {
        for (int x = 0; x < grid->width; x++) {
            int index = (y * grid->width) + x;
            CellType cell = (CellType)grid->cells[index];

            if (cell == EMPTY) continue;

            if (cell == OBSTACLE) {
                grid->cell_buff[index] = OBSTACLE;
                continue;
            }

            if (cell == SAND) {
                // If at the bottom, stay there
                if (y == grid->height - 1) {
                    grid->cell_buff[index] = SAND;
                    continue;
                }

                int b = index + grid->width;
                
                // Rule 1: Move Straight Down
                // We check both the current frame AND the buffer
                if (grid->cells[b] == EMPTY && grid->cell_buff[b] == EMPTY) {
                    grid->cell_buff[b] = SAND;
                } 
                else {
                    // Rule 2: Try Diagonals
                    int dir = (rand() % 2 == 0) ? 1 : -1;
                    int diag_prime = (y + 1) * grid->width + (x + dir);
                    int diag_sec   = (y + 1) * grid->width + (x - dir);

                    // Check primary diagonal (Logic and Buffer check)
                    if ((x + dir >= 0 && x + dir < grid->width) &&
                        grid->cells[diag_prime] == EMPTY && 
                        grid->cell_buff[diag_prime] == EMPTY) {
                        grid->cell_buff[diag_prime] = SAND; // Fixed '==' typo
                    } 
                    // Check secondary diagonal
                    else if ((x - dir >= 0 && x - dir < grid->width) &&
                             grid->cells[diag_sec] == EMPTY && 
                             grid->cell_buff[diag_sec] == EMPTY) {
                        grid->cell_buff[diag_sec] = SAND; // Fixed '==' typo
                    } 
                    else {
                        // Rule 3: Stay Put (if nothing else works)
                        grid->cell_buff[index] = SAND;
                    }
                }
            }
        }
    }

    // Pointer Swap
    uint8_t *temp = grid->cells;
    grid->cells = grid->cell_buff;
    grid->cell_buff = temp;
}

void draw_grid(const Grid *grid) {
  size_t totalCells = (size_t)grid->width * grid->height;
  for (int i = 0; i < totalCells; i++) {
    if (grid->cells[i] != EMPTY) {
      int dx = ((i % grid->width) * CELL_DIM) + grid->x_offset;
      int dy = ((i / grid->width) * CELL_DIM) + grid->y_offset;

      Color c = (grid->cells[i] == SAND) ? YELLOW : GRAY;
      DrawRectangle(dx, dy, CELL_DIM - 1, CELL_DIM - 1, c);
    }
  }
}
