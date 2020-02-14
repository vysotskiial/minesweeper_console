#include "cstdlib"
#include "iostream"
#include "time.h"
#include "mine_board.h"

using namespace std;

MineBoard::MineBoard(int _height, int _width, int _bomb_numb): height(_height), width(_width), bomb_numb(_bomb_numb)
{
	if (bomb_numb > height * width) {
		cout << "Too many bombs" << endl;
		exit(-1);
	}

	board = new Cell*[height];
	if (!board) exit(1);


	for (int i = 0; i < height; i++)
	{
		board[i] = new Cell[width];
		if (!board[i]) exit(1);
	}

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			set_neighbors(i, j);
	return;
}

void MineBoard::set_neighbors(int i, int j)
{
	Cell &c = board[i][j];

	add_neighbor(c, i + 1, j);
	add_neighbor(c, i + 1, j + 1);
	add_neighbor(c, i + 1, j - 1);
	add_neighbor(c, i, j + 1);
	add_neighbor(c, i, j - 1);
	add_neighbor(c, i - 1, j);
	add_neighbor(c, i - 1, j + 1);
	add_neighbor(c, i - 1, j - 1);
}

void MineBoard::add_neighbor(Cell &c, int i, int j)
{
	if (i >= 0 && i < height && j >= 0 && j < width) c.neighbors.push_back(&board[i][j]);
}

void MineBoard::place_bombs(int clear_x, int clear_y)
{
	srand(time(NULL));

	int n_surround_clear = board[clear_x][clear_y].neighbors.size() + 1;

	for (int i = 0; i < bomb_numb; i++)
	{
		int x = 0, y = 0;
		int cur_bomb = rand() % (height * width - i - n_surround_clear);

		while (1) {
			if (!board[x][y].is_mine && ((abs(x - clear_x) > 1) || (abs(y - clear_y) > 1)))  cur_bomb--;
			if (cur_bomb < 0) break;
			if (y == width - 1) {
				y = 0;
				x++;
			}
			else y++;
		}    board[x][y].is_mine = true;
		for (unsigned k = 0; k < board[x][y].neighbors.size(); k++) board[x][y].neighbors[k]->bombs_around++;
	}

	return;
}

void MineBoard::open(int x, int y)
{
	if (!open_cells_num) {
		flag_numb = 0;
		place_bombs(x, y);
	}

	if (x < 0 || x >= height || y < 0 || y >= width) return;

	Cell &c = board[x][y];
	if (c.is_open || c.is_flag) return;
	if (c.is_mine)
	{
		game_over = 1;
		blow_up_x = y + 0.5;
		blow_up_y = x + 0.5;
		return;
	}

	c.is_open = true;
	open_cells_num += 1;
	if (open_cells_num == height * width - bomb_numb && flag_numb == bomb_numb) {
		victory_flag = true;
	}

	if (c.bombs_around) return;

	open(x + 1, y + 1);
	open(x, y + 1);
	open(x - 1, y + 1);
	open(x + 1, y);
	open(x - 1, y);
	open(x + 1, y - 1);
	open(x, y - 1);
	open(x - 1, y - 1);
	return;
}

void MineBoard::open_around(int x, int y)
{
	Cell &c = board[x][y];
	if (!c.is_open) return;

	int flags_around = 0;
	for (unsigned i = 0; i < c.neighbors.size(); i++) flags_around += c.neighbors[i]->is_flag;
	if (flags_around != c.bombs_around) return;
	open(x + 1, y + 1);
	open(x, y + 1);
	open(x - 1, y + 1);
	open(x + 1, y);
	open(x - 1, y);
	open(x + 1, y - 1);
	open(x, y - 1);
	open(x - 1, y - 1);
	return;
}

void MineBoard::reset()
{
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			board[i][j].is_mine = false;
			board[i][j].is_flag = false;
			board[i][j].is_open = false;
			board[i][j].bombs_around = 0;
		}
	}
	open_cells_num = 0;
	game_over = false;
	victory_flag = false;

	return;
}

void MineBoard::put_flag(int x, int y)
{
	if (board[x][y].is_open) return;
	board[x][y].is_flag = !board[x][y].is_flag;
	flag_numb += 2 * board[x][y].is_flag - 1;
	if (flag_numb == bomb_numb && open_cells_num == width * height - bomb_numb) {
		victory_flag = true;
	}
}

MineBoard::~MineBoard()
{
	for (int i = 0; i < height; i++)
	{
		delete[] board[i];
	}
	delete[] board;
}
