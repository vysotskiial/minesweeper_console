#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <iostream>
#include <string>
#include "mine_drawer.h"

using namespace std;

namespace MineDrawer {

namespace {

MineBoard my_board(height, width, bomb_n);
time_t cur_time;
int timer_id = 0;
std::map<int, int> buttons = {{GLUT_RIGHT_BUTTON, GLUT_UP}, {GLUT_LEFT_BUTTON, GLUT_UP}};

void draw_grid() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3b(0, 0, 0);
	glBegin(GL_LINES);
	for (int i = 0; i < height; i++) {
		glVertex3f(0, i, 0.0);
		glVertex3f(width, i, 0.0);
	}

	glBegin(GL_LINES);
	for (int i = 0; i < width; i++) {
		glVertex3f(i, 0, 0.0);
		glVertex3f(i, height, 0.0);
	}
	glEnd();
}

void draw_mine(float x, float y)
{
	float x_1 = x - 0.5;
	float y_1 = y - 0.5;

	glBegin(GL_QUADS);
		glColor3f(0, 0, 0);
		glVertex3f(x_1, y_1 + 0.03, 0);
		glVertex3f(x_1 + 0.97, y_1 + 0.03, 0);
		glVertex3f(x_1 + 0.97, y_1 + 1, 0);
		glVertex3f(x_1, y_1 + 1, 0);
	glEnd();

	glBegin(GL_TRIANGLES);
		glColor3f(1, 0, 0);
		glVertex3f(x - 0.33, y - 0.166, 0.0);
		glVertex3f(x + 0.33, y - 0.166, 0.0);
		glVertex3f(x, y + 0.33, 0.0);
		glVertex3f(x - 0.33, y + 0.166, 0.0);
		glVertex3f(x + 0.33, y + 0.166, 0.0);
		glVertex3f(x, y - 0.33, 0.0);
	glEnd();

	glFlush();
}

void draw_number(int i, int j)
{
	glBegin(GL_QUADS);
		glColor3f(0.8, 0.8, 0.8);
		glVertex3f(j, i+0.03, 0);
		glVertex3f(j+0.97, i+0.03, 0);
		glVertex3f(j+0.97, i+1, 0);
		glVertex3f(j, i+1, 0);
	glEnd();

	switch (my_board.get_board(i, j)) {
		case 1:
			glColor3f(0, 0, 1);
			break;
		case 2:
			glColor3f(0, 0.8, 0);
			break;
		case 3:
			glColor3f(1, 0, 0);
			break;
		case 4:
			glColor3f(0, 1, 1);
			break;
		default:
			glColor3f(0, 0, 0);
	}

	if (my_board.get_board(i, j) != 0) {
		glRasterPos2f(j + 0.3, i + 0.8);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, my_board.get_board(i, j) + '0');
	}
}

void draw_open_cells()
{
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			if (my_board.get_open(i, j)) draw_number(i, j);
}

void draw_flag(int j, int i)
{
	glBegin(GL_QUADS);
		glColor3f(0.8, 0.8, 0.8);
		glVertex3f(i, j + 0.03, 0);
		glVertex3f(i + 0.97, j + 0.03, 0);
		glVertex3f(i + 0.97, j + 1, 0);
		glVertex3f(i, j + 1, 0);
	glEnd();

	glBegin(GL_LINES);
		glColor3f(0, 0, 0);
		glVertex3f(i + 0.2, j + 0.8, 0);
		glVertex3f(i + 0.7, j + 0.8, 0);
		glVertex3f(i + 0.45, j + 0.1, 0);
		glVertex3f(i + 0.45, j + 0.8, 0);
	glEnd();

	glBegin(GL_QUADS);
		glColor3f(1, 0, 0);
		glVertex3f(i + 0.45, j + 0.1, 0);
		glVertex3f(i + 0.85, j + 0.1, 0);
		glVertex3f(i + 0.85, j + 0.5, 0);
		glVertex3f(i + 0.45, j + 0.5, 0);
	glEnd();
}

void draw_flags()
{
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			if (my_board.get_flag(i, j) == 1) draw_flag(i, j);
}

void draw_clock()
{
	glRasterPos2f(0, -0.1);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char *)to_string(cur_time).c_str());
}

void draw_flags_num()
{
	glRasterPos2f(29, -0.1);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char *)to_string(my_board.get_flags_num()).c_str());
}

void draw_button()
{
	glBegin(GL_QUADS);
		glColor3f(1, 1, 1);
		glVertex2d(14.5, 0);
		glVertex2d(14.5, -1);
		glVertex2d(15.5, -1);
		glVertex2d(15.5, 0);
	glEnd();
}

} //Unnamed namespace

void render_scene()
{
	draw_grid();

	draw_clock();
	draw_flags_num();
	draw_button();

	if (my_board.game_over)
		draw_mine(my_board.get_blow_x(), my_board.get_blow_y());

	draw_open_cells();
	draw_flags();

	glutSwapBuffers();
}

void mouse_func(int button, int state, int x, int y)
{
	buttons[button] = state;
	if (state == GLUT_UP) return;
	if ((button == GLUT_LEFT_BUTTON) && (y < 30) && (x > 435) && (x < 465)) {
		my_board.reset();
		cur_time = 0;
		glutPostRedisplay();
		return;
	}

	if (y < 30) return;

	if (my_board.game_over || my_board.victory_flag) return;

	int cell_x = y / width - 1;
	int cell_y = x / width;

	if ((buttons[GLUT_LEFT_BUTTON] == GLUT_DOWN) && (buttons[GLUT_RIGHT_BUTTON] == GLUT_DOWN))
		my_board.open_around(cell_x, cell_y);
	else if (buttons[GLUT_LEFT_BUTTON] == GLUT_DOWN) {
		if (!my_board.is_active()) glutTimerFunc(1000, timer_f, ++timer_id);
		my_board.open(cell_x, cell_y);
	}
	else if (buttons[GLUT_RIGHT_BUTTON] == GLUT_DOWN)
		my_board.put_flag(cell_x, cell_y);

	glutPostRedisplay();
}

void timer_f(int i) {
	if (!my_board.is_active() || i != timer_id) return;
	cur_time++;
	glutTimerFunc(1000, timer_f, i);
	glutPostRedisplay();
}

} //namespace MineDrawer
