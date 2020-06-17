#pragma once
#include <array>
#include <vector>

constexpr int height = 16;
constexpr int width = 30;
constexpr int bomb_n = 99;

class MineBoard {
private:
	class Cell {
		friend class MineBoard;
		bool is_mine{false};
		std::vector<Cell *> neighbors;
		MineBoard *owner;

	public:
		Cell &operator=(const Cell &) = delete;
		bool is_open{false};
		int bombs_around{0};
		bool is_flag{false};
		void open();
		void open_around();
		void flag();
	};
	using Row = std::array<Cell, width>;
	std::array<Row, height> board;
	void set_neighbors(int i, int j);
	void find_explosion();

	int _flag_num{0};
	int open_cells_num{0};
	bool _lose{false};
	bool _win{false};
	int blow_up_x{0};
	int blow_up_y{0};

public:
	MineBoard();
	Cell &operator()(int x, int y) { return board[x][y]; }
	bool is_active() { return !(_win || _lose || (open_cells_num == 0)); }
	void place_bombs(int i, int j);
	void reset();
	constexpr int flag_num() const noexcept { return _flag_num; }
	constexpr bool win() const noexcept { return _win; }
	constexpr bool lose() const noexcept { return _lose; }
	constexpr int explosion_x() const noexcept { return blow_up_x; }
	constexpr int explosion_y() const noexcept { return blow_up_y; }
};
