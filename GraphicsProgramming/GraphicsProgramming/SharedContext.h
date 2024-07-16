// Shared Context 
// It contains a structure with all the pointers to the objects which are necessary in all the game
// These pointers will be pointing to the objects which must be declared in some static class which will be active during all the game
// In this case these objects has been created in the main.cpp
// @author Francisco Diaz (@FMGameDev)

#pragma once
#include "Input.h"

// All the components that are shared between the game scenes
// They must been initialise in the main.cpp
struct SharedContext
{
	// constructor
	SharedContext() : input(nullptr), wireframe_mode(nullptr), game_focused(false), window_width(nullptr), window_height(nullptr){}

	// components
	Input* input; // input component
	bool* wireframe_mode; // if it is active then all the objets will be rendered in wireframe mode
	bool* game_focused; // control if the game itself has the focus (it has been clicked) so the player can use the mouse and keyboard inputs to interact with the game.
	bool* first_mouse_click; // to control the camera doesn't rotate when the user click for first time after recuperating the focus the game
	int* window_width; // window x value
	int* window_height; // window y value

};
