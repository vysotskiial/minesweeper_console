#pragma once
#include <map>
#include "mine_board.h"

constexpr int height = 16;
constexpr int width = 30;
constexpr int bomb_n = 99;

namespace MineDrawer {
	void render_scene();
	void mouse_func(int, int, int, int);
	void timer_f(int i);
}
