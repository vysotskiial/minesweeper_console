#include "GL/glut.h"
#include "mine_board.h"
#include "iostream"

constexpr int height = 16;
constexpr int width = 30;
constexpr int bomb_n = 99;

using namespace std;

void draw_mine(float x, float y);
void draw_open_cells();
void draw_number(int i, int j);
void draw_flags();
void draw_flag(int i, int j);

MineBoard my_board(height, width, bomb_n);
bool right_button_down = 0, left_button_down = 0, first_click = 1;

void renderScene(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glColor3b(0, 0, 0);
  glBegin(GL_LINES);
  for (int i = 0; i < height; i++)
  {
    glVertex3f(0, i, 0.0);
    glVertex3f(width, i, 0.0);
  }

  glBegin(GL_LINES);
  for (int i = 0; i < width; i++)
  {
    glVertex3f(i, 0, 0.0);
    glVertex3f(i, height, 0.0);
  }
  glEnd();

  if (my_board.game_over)
  {
    draw_mine(my_board.get_blow_x(), my_board.get_blow_y());
  }
  draw_open_cells();
  draw_flags();

  glutSwapBuffers();
}

void mouse_func(int button, int state, int x, int y)
{
  if (my_board.game_over || my_board.victory_flag) return;
  if (state != GLUT_DOWN)
  {
    if (button == GLUT_RIGHT_BUTTON) right_button_down = 0;
    if (button == GLUT_LEFT_BUTTON) left_button_down = 0;
    return;
  }

  if (button == GLUT_LEFT_BUTTON)
  {
    left_button_down = 1;
    int cell_x = y / width;
    int cell_y = x / width;

    if (!right_button_down)
    {
      if (first_click)
      {
        my_board.start(cell_x, cell_y);
        my_board.set_start_time();
        first_click = false;
      }
      my_board.open(cell_x, cell_y);
      return;
    }

    my_board.open_around(cell_x, cell_y);
    return;
  }

  if (button == GLUT_RIGHT_BUTTON)
  {
    right_button_down = 1;
    int cell_x = y / width;
    int cell_y = x / width;
    if (!left_button_down)
    {
      my_board.put_flag(cell_x, cell_y);
      return;
    }
    my_board.open_around(cell_x, cell_y);
    return;
  }
}

void keyboard_func(unsigned char key, int x, int y)
{
  if (key == 'r')
  {
    my_board.reset();
    right_button_down = 0;
    left_button_down = 0;
    first_click = 1;
  }
  return;
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

void draw_open_cells()
{
  for (int i = 0; i < height; i++)
    for (int j = 0; j < width; j++)
      if (my_board.get_open(i, j)) draw_number(i, j);
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

  switch (my_board.get_board(i, j))
  {
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


  if (my_board.get_board(i, j) != 0)
  {
    glRasterPos2f(j + 0.3, i + 0.8);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, my_board.get_board(i, j) + '0');
  }
}

void draw_flags()
{
  for (int i = 0; i < height; i++)
    for (int j = 0; j < width; j++)
      if (my_board.get_flag(i, j) == 1) draw_flag(i, j);
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

int main(int argc, char** argv)
{
  glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(900, 480);
  glutCreateWindow("Сапер");
  glClearColor(0.6, 0.6, 0.6, 1.0);
  glOrtho(0, 30.0, 16.0, 0, -5.0, 5.0);
  glutMouseFunc(mouse_func);
  glutKeyboardFunc(keyboard_func);

  glutDisplayFunc(renderScene);
  glutIdleFunc(renderScene);

  glutMainLoop();
  return 0;
}
