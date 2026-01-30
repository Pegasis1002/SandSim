#ifndef SIM_H
#define SIM_H

#include <stdint.h>

#define GRID_SIZE 10
#define CELL_DIM 40

typedef enum {
  EMPTY = 0,
  SAND = 1,
  OBSTACLE = 2
} CellType;

typedef struct {
  int width;
  int height;
  int x_offset;
  int y_offset;
  uint8_t *cells;
  uint8_t *cell_buff;
} Grid;

Grid create_grid(int width, int height);
void destroy_grid(Grid *grid);
void update_simulation(Grid *grid);
void draw_grid(const Grid *grid);

#endif // SIM_H
