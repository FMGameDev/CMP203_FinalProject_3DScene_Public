// Main entry point for application. 
// Initialises main window, captures user input and passes onto appropriate class for handling.
// Utilises FreeGLUT API for window.
// Initialises Scene and Input class.
// @author Paul Robertson


// Include glut, opengl libraries and custom classes
#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Scene.h"
#include "SharedContext.h"
#include <iostream>

// Required variables; pointer to scene and input objects. Initialise variable used in delta time calculation.
Scene* scene;
SharedContext shared_context;
int oldTimeSinceStart = 0;

// Called when the window detects a change in size.
// GLUT handles the window refresh, this function passes the new width and height to the
// scene object to refresh the OpenGL buffers to the new dimensions.
void changeSize(int w, int h) 
{
	scene->resize(w, h);
}

// Called as part of the GLUT main loop.
// Calculates delta time (the time between frames, in seconds)
// Calls Scene update and render for the next frame.
void renderScene(void) 
{
	// Calculate delta time.
	int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	float deltaTime = (float)timeSinceStart - (float)oldTimeSinceStart;
	oldTimeSinceStart = timeSinceStart;
	deltaTime = deltaTime / 1000.0f;

	// Update Scene and render next frame.
	scene->handleInput(deltaTime);
	scene->update(deltaTime);
	scene->render();
}

// Handles keyboard input events from GLUT.
// Called whenever a "normal" key is pressed.
// Normal keys are defined as any key not including the F keys, CTRL, SHIFT, ALT, etc.
// Key press is recorded in Input class 
// Parameters include key pressed and current mouse x, y coordinates.
// Mouse coordinates are handled separately.
void processNormalKeys(unsigned char key, int x, int y)
{
	// If the ESCAPE key was pressed, exit application.
	if (key == VK_ESCAPE)	// Escape key (in non-windows you can use 27, the ASCII value for escape)
		exit(0);
	// Send key down to input class.
	shared_context.input->setKeyDown(key);
}

// Handles keyboard input events from GLUT.
// Called whenever a "normal" key is released.
// Normal keys are defined as any key not including the F keys, CTRL, SHIFT, ALT, arrow keys, etc.
// Key press is recorded in Input class 
// Parameters include key pressed and current mouse x, y coordinates.
// Mouse coordinates are handled separately.
void processNormalKeysUp(unsigned char key, int x, int y)
{
	// Send key up to input class.
	shared_context.input->setKeyUp(key);
}

// Handles keyboard input events from GLUT.
// Called whenever a "special" key is pressed.
// Special keys are defined as F keys, CTRL, SHIFT, ALT, arrow keys, etc
// Currently a place holder function, can be utilised if required.
// Parameters include key pressed and current mouse x, y coordinates.
// Mouse coordinates are handled separately.
void processSpecialKeys(int key, int x, int y)
{
	// TODO: Pass special key press to Input class.
}

// Handles keyboard input events from GLUT.
// Called whenever a "special" key is released.
// Special keys are defined as F keys, CTRL, SHIFT, ALT, arrow keys, etc
// Currently a place holder function, can be utilised if required.
// Parameters include key pressed and current mouse x, y coordinates.
// Mouse coordinates are handled separately.
void processSpecialKeysUp(int key, int x, int y)
{
	// TODO: Pass special key release to Input class.
}

// Handles mouse movement events from GLUT.
// Active movement is define as mouse movement while a mouse button is pressed.
// Called every loop. Parameters are the new x, y coordinates of the mouse.
void processActiveMouseMove(int x, int y)
{
	// Record mouse position in Input class.
	shared_context.input->setMousePos(x, y);
}

// Handles mouse movement events from GLUT.
// Passive mouse movement is define as mouse movement without a mouse button is pressed.
// Called every loop. Parameters are the new x, y coordinates of the mouse.
void processPassiveMouseMove(int x, int y)
{
	// Record mouse position in Input class.
	shared_context.input->setMousePos(x, y);
}

// Handles mouse button and wheel events from GLUT.
// Parameters include mouse button that fired the event (GLUT_LEFT_BUTTON, GLUT_RIGHT_BUTTON, GLUT_MIDDLE_BUTTON),
// button state (up and down), and current cursor position.
void processMouseButtons(int button, int state, int x, int y)
{
	// Detect left button press/released
	if (button == GLUT_LEFT_BUTTON) {
		// when the button is released
		if (state == GLUT_DOWN) {
			shared_context.input->setMouseLDown(true);
		}
		// else button state == GLUT_UP
		else  {
			shared_context.input->setMouseLDown(false);
		}
	}
	else if (button == GLUT_RIGHT_BUTTON) {
		// when the button is released
		if (state == GLUT_DOWN) {
			shared_context.input->setMouseRDown(true);
		}
		// else button state == GLUT_UP
		else {
			shared_context.input->setMouseRDown(false);
		}
	}
	else if (button == GLUT_MIDDLE_BUTTON) {
		// when the button is released
		if (state == GLUT_DOWN) {
			shared_context.input->setMouseMDown(true);
		}
		// else button state == GLUT_UP
		else {
			shared_context.input->setMouseMDown(false);
		}
	}
	else if (button == 3) // wheel scroll up
	{
		shared_context.input->setScrollUp(true);
	}
	else if (button == 4) // wheel scroll down
	{
		shared_context.input->setScrollDown(true);
	}
}

// Delete all the pointers created in the main file
void deletePointers()
{
	delete scene;
	scene = nullptr;

	delete shared_context.input;
	shared_context.input = nullptr;
	delete shared_context.window_width;
	shared_context.window_width = nullptr;
	delete  shared_context.window_height;
	shared_context.window_height = nullptr;
	delete  shared_context.wireframe_mode;
	shared_context.wireframe_mode = nullptr;
	delete shared_context.game_focused;
	shared_context.game_focused = nullptr;
	delete shared_context.first_mouse_click;
	shared_context.first_mouse_click = nullptr;
}

// Main entery point for application.
// Initialises GLUT and application window.
// Registers callback functions for handling GLUT input events
// Registers callback functions for window resizing and rendering.
// Initialises Input and Scene class, prior to starting Main Loop.
int main(int argc, char **argv) 
{
	shared_context.window_width = new int(1280);
	shared_context.window_height = new int(720);
	shared_context.wireframe_mode = new bool(false); // by default the project is not in wireframe mode
	shared_context.game_focused = new bool(false); // by default the game has not the focus (mouse not jailed, etc)
	shared_context.first_mouse_click = new bool(true); // it is updated to false in camera.cpp after the user has clicked, and it is set to true again in scene when the game lost the focus

	// Init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_STENCIL);
	//glutInitWindowPosition(100, 100);
	
	glutInitWindowSize(*shared_context.window_width, *shared_context.window_height);
	glutCreateWindow("CMP203 Coursework - 1902654");
	
	// Register callback functions for change in size and rendering.
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

	// Register Input callback functions.
	glutKeyboardFunc(processNormalKeys);
	glutKeyboardUpFunc(processNormalKeysUp);
	glutSpecialFunc(NULL);
	// Special keys not processed, as currently not required.
	//glutSpecialFunc(processSpecialKeys);
	//glutSpecialUpFunc(processSpecialKeysUp);
	
	// Mouse callbacks
	glutMotionFunc(processActiveMouseMove);
	glutPassiveMotionFunc(processPassiveMouseMove);
	glutMouseFunc(processMouseButtons);
	//glutMouseWheelFunc(processMouseWheel); // for glutfree

	// Position mouse in centre of windows before main loop (window not resized yet)
	glutWarpPointer(*shared_context.window_width/2.0f, *shared_context.window_height / 2.0f);
	// Hide mouse cursor
	//glutSetCursor(GLUT_CURSOR_NONE);

	// Initialise input and scene objects.
	shared_context.input = new Input();
	scene = new Scene(&shared_context);
	
	// Enter GLUT event processing cycle
	glutMainLoop();

	// delete pointers created
	deletePointers();

	return 1;
}