#pragma once
#include "time.h"
#include <vector>

class MineBoard
{
private:
  struct Cell {
    bool is_mine = false;
    bool is_open = false;
    bool is_flag = false;
    std::vector<Cell*> neighbors;
    int bombs_around = 0;
  } **board;
  void place_bombs(int clear_x, int clear_y);  //set mines
  int height;
  int width;
  int bomb_numb;
  int flag_numb;
  float blow_up_x, blow_up_y;
  void set_neighbors(int i, int j);
  void add_neighbor(Cell &c, int i, int j);
  int open_cells_num = 0;
  time_t start_time = 0;
public:
  MineBoard(int _height, int _width, int _bomb_numb);
  void start(int clear_x, int clear_y);
  float get_blow_x() { return blow_up_x; }
  float get_blow_y() { return blow_up_y; }
  int get_board(int i, int j) { return board[i][j].bombs_around; }
  void open_around(int x, int y);   //double click solver
  void open(int x, int y);        //opens cell x,y and surroundings if board[x][y]=0, if there is bomb on x,y returns 0, otherwise 1
  bool get_flag(int i, int j) { return board[i][j].is_flag; }
  bool get_open(int i, int j) { return board[i][j].is_open; }
  bool game_over = false;
  bool victory_flag = false;
  void put_flag(int x, int y);
  void set_start_time() { start_time = time(NULL); }
  void reset();
  ~MineBoard();
};
