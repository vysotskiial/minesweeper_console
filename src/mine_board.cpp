#include <algorithm>
#include <ctime>
#include "mine_board.h"

using namespace std;

MineBoard::MineBoard()
{
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			set_neighbors(i, j);
			board[i][j].owner = this;
		}
	}
	return;
}

void MineBoard::set_neighbors(int i, int j)
{
	auto add_neighbor = [this, i, j](int x, int y) {
		if (x >= 0 && x < height && y >= 0 && y < width)
			board[i][j].neighbors.push_back(&board[x][y]);
	};

	add_neighbor(i + 1, j);
	add_neighbor(i + 1, j + 1);
	add_neighbor(i + 1, j - 1);
	add_neighbor(i, j + 1);
	add_neighbor(i, j - 1);
	add_neighbor(i - 1, j);
	add_neighbor(i - 1, j + 1);
	add_neighbor(i - 1, j - 1);
}

void MineBoard::place_bombs(int i, int j)
{
	srand(time(NULL));
	auto &c = board[i][j];

	// We cannot place bomb in the chosen cell or its neighbors
	int available_spaces = height * width - c.neighbors.size() - 1;
	// For now mark cell and its neighbors as mines
	c.is_mine = true;
	for (auto cell : c.neighbors)
		cell->is_mine = true;

	for (int i = 0; i < bomb_n; i++) {
		int x = 0;
		int y = 0;
		int cur_bomb = rand() % available_spaces--;

		while (true) {
			if (!board[x][y].is_mine)
				cur_bomb--;
			if (cur_bomb < 0)
				break;
			y = (y + 1) % width;
			x += !y;
		}
		board[x][y].is_mine = true;
		for (auto cell : board[x][y].neighbors)
			cell->bombs_around++;
	}

	c.is_mine = false;
	for (auto cell : c.neighbors)
		cell->is_mine = false;

	return;
}

void MineBoard::Cell::open()
{
	if (is_open || is_flag)
		return;
	is_open = true;
	if (is_mine) {
		owner->_lose = true;
		owner->find_explosion();
		return;
	}

	owner->open_cells_num++;
	if (owner->open_cells_num == height * width - bomb_n &&
	    owner->_flag_num == bomb_n)
		owner->_win = true;

	if (bombs_around)
		return;
	for (auto cell : neighbors)
		cell->open();

	return;
}

void MineBoard::find_explosion()
{
	for (auto i = 0; i < height; i++)
		for (auto j = 0; j < width; j++)
			if (board[i][j].is_open && board[i][j].is_mine) {
				blow_up_y = i;
				blow_up_x = j;
				board[i][j].is_open = false;
				return;
			}
}

void MineBoard::Cell::open_around()
{
	if (!is_open)
		return;

	int flags_around = count_if(neighbors.begin(), neighbors.end(),
	                            [](Cell *c) { return c->is_flag; });
	if (flags_around != bombs_around)
		return;
	for (auto cell : neighbors)
		cell->open();

	return;
}

void MineBoard::reset()
{
	for (auto &row : board) {
		for (auto &cell : row) {
			cell.is_mine = false;
			cell.is_flag = false;
			cell.is_open = false;
			cell.bombs_around = 0;
		}
	}

	open_cells_num = 0;
	_flag_num = 0;
	_win = false;
	_lose = false;

	return;
}

void MineBoard::Cell::flag()
{
	if (is_open)
		return;

	is_flag = !is_flag;
	owner->_flag_num += 2 * is_flag - 1;
	if (owner->_flag_num == bomb_n &&
	    owner->open_cells_num == width * height - bomb_n)
		owner->_win = true;
}
