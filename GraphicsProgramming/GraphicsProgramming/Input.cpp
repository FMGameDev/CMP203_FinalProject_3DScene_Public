// Input class
// Stores current keyboard and mouse state include, pressed keys, mouse button pressed and mouse position.
// @author Paul Robertson
// It has been added two scroll detections (up and down) and middle button (click detection)
// Modified by Francisco Diaz

#include "Input.h"

Input::Input()
{
	// set default values
	mouse.left = false;
	mouse.right = false;
	mouse.middle = false;
	mouse.scroll_up = false;
	mouse.scroll_down = false;
	mouse.x = 0;
	mouse.y = 0;
}

void Input::setKeyDown(unsigned char key)
{
	if (key >= 0)
	{
		keys[key] = true;
	}
}

void Input::setKeyUp(unsigned char key)
{
	if (key >= 0)
	{
		keys[key] = false;
	}
}

bool Input::isKeyDown(int key)
{
	if (key >= 0)
	{
		return keys[key];
	}
	return false;
}

void Input::setMouseX(int pos)
{
	mouse.x = pos;
}

void Input::setMouseY(int pos)
{
	mouse.y = pos;
}

void Input::setMousePos(int ix, int iy)
{
	mouse.x = ix;
	mouse.y = iy;
}

int Input::getMouseX()
{
	return mouse.x;
}

int Input:: getMouseY()
{
	return mouse.y;
}

void Input::setMouseLDown(bool down)
{
	mouse.left = down;
}

bool Input::isMouseLDown()
{
	return mouse.left;
}

void Input::setMouseRDown(bool down)
{
	mouse.right = down;
}
bool Input::isMouseRDown()
{
	return mouse.right;
}

void Input::setMouseMDown(bool down)
{
	mouse.middle = down;
}
bool Input::isMouseMDown()
{
	return mouse.middle;
}

void Input::setScrollUp(bool up)
{
	mouse.scroll_up = up;
}
bool Input::isScrollUp()
{
	return mouse.scroll_up;
}

void Input::setScrollDown(bool down)
{
	mouse.scroll_down = down;
}
bool Input::isScrollDown()
{
	return mouse.scroll_down;
}