#include <GL/glut.h>
#include <iostream>
#include <map>
#include "mine_drawer.h"

using namespace std;

int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(900, 510);
	glutCreateWindow("Minesweeper");
	glClearColor(0.6, 0.6, 0.6, 1.0);
	glOrtho(0, 30.0, 16.0, -1.0, -5.0, 5.0);
	glutMouseFunc(MineDrawer::mouse_func);
	glutDisplayFunc(MineDrawer::render_scene);

	glutMainLoop();
	return 0;
}
