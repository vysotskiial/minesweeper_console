#include <GL/glut.h>
#include "mine_drawer.h"

using namespace std;

constexpr int screen_width = 900;
constexpr int screen_height = 510;

//int main(int argc, char *argv[])
int WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd
)
{
	int argc = 0;
	char* argv = "abcd";
	glutInit(&argc, &argv);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(screen_width, screen_height);
	glutCreateWindow("Minesweeper");
	glutReshapeFunc([](int width, int height) {
		if ((width != screen_width) || (height != screen_height))
			glutReshapeWindow(screen_width, screen_height);
	});
	glClearColor(0.6, 0.6, 0.6, 1.0);
	glOrtho(0, 30.0, 16.0, -1.0, -5.0, 5.0);
	glutMouseFunc(MineDrawer::mouse_func);
	glutDisplayFunc(MineDrawer::render_scene);

	glutMainLoop();
	return 0;
}
