// Input class
// Stores current keyboard and mouse state include, pressed keys, mouse button pressed and mouse position.
// @author Paul Robertson
// It has been added two scroll detections (up and down) and middle button (click detection)
// Modified by Francisco Diaz

#ifndef INPUT_H
#define INPUT_H

class Input
{
	// Mouse struct stores mouse related data include cursor
	// x, y coordinates and left/right button pressed state.
	struct Mouse
	{
		int x,y; // mouse position
		bool left, right, middle; // buttons (click detection)
		bool scroll_up, scroll_down; // wheel
	};

public:
	Input();
	// Getters and setters for keys (keyboard)
	void setKeyDown(unsigned char key);
	void setKeyUp(unsigned char key);
	bool isKeyDown(int key);

	// getters and setters for mouse buttons, wheel and position.
	// pos
	void setMouseX(int lx);
	void setMouseY(int ly);
	void setMousePos(int x, int y);
	int getMouseX();
	int getMouseY();
	// button click
	void setMouseLDown(bool down);
	bool isMouseLDown();
	void setMouseRDown(bool down);
	bool isMouseRDown();
	void setMouseMDown(bool down);
	bool isMouseMDown();
	// wheel
	void setScrollUp(bool up);
	bool isScrollUp();
	void setScrollDown(bool down);
	bool isScrollDown();

private:
	// Boolean array, element per key
	// Mouse struct object.
	bool keys[256]{ false }; // keyboard
	Mouse mouse;

};

#endif