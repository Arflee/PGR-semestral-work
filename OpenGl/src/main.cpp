#include "Scene.h"
#include "Input.h"
#include <pgr.h>

static void ReshapeCallback(int newWidth, int newHeight)
{
		glViewport(0, 0, (GLsizei)newWidth, (GLsizei)newHeight);
}

int main(int argc, char** argv)
{
		glutInit(&argc, argv);
		glutInitContextVersion(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR);
		glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
		glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);

		glutInitWindowSize(800, 800);
		glutCreateWindow("Winter scene");

		glutDisplayFunc(&Scene::DisplayCallback);
		glutReshapeFunc(&ReshapeCallback);

		glutKeyboardFunc(&Input::KeyCallback);
		glutKeyboardUpFunc(&Input::KeyUpCallback);
		glutSpecialFunc(&Input::SpecialKeyCallback);
		glutSpecialUpFunc(&Input::SpecialKeyUpCallback);
		glutPassiveMotionFunc(&Input::PassiveMouseMotionCallback);
		glutMouseFunc(&Input::MouseCallback);

		glutTimerFunc(33, &Scene::TimerCallback, 0);
		glutSetCursor(GLUT_CURSOR_NONE);

		if (!pgr::initialize(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR))
				pgr::dieWithError("pgr init failed, required OpenGL not supported?");

		Scene::InitializeApplication();

		glutMainLoop();

		return 0;
}
