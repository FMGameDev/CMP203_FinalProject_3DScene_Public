#include "Scene.h"

// Scene constructor, initilises OpenGL
Scene::Scene(SharedContext* shared_context)
	: shared_context_(shared_context)
{
	initialiseOpenGL();

	// Other OpenGL / render setting should be applied here.
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_COLOR_MATERIAL);

	// Set the position of the mouse in the middle of the window

	// camera
	camera_mgr_ = new CameraManager(shared_context_);

	// light manager
	light_mgr_ = new LightManager(shared_context_);

	// create textures
	initialiseTextures();

	// create material
	//initialiseMaterials();

	// create meshes
	initialiseMeshes();
}

Scene::~Scene()
{
	// delete all the pointers created in this class
	delete camera_mgr_;
	camera_mgr_ = nullptr;

	delete light_mgr_ ;
	light_mgr_ = nullptr;

	for (std::pair<TextureName, Texture*> texture : textures)
	{
		delete texture.second;
		texture.second = nullptr;
	}
}

void Scene::handleInput(float dt)
{
	// Change if the game has the focus or not
	if (shared_context_->input->isMouseLDown())
	{
		shared_context_->input->setMouseLDown(false);

		// activate or deactivate jailed mouse depending on the current state
		if (*shared_context_->game_focused == true)
		{
			// set jailed mouse flag to flag, so the camera will stop fixing it on the middle of the screen
			*shared_context_->game_focused = false;
			*shared_context_->first_mouse_click = true;

			// Show mouse cursor
			glutSetCursor(GLUT_CURSOR_INHERIT); // default cursor
		}
		else
		{
			// set jailed mouse flag to true, so in the camera function will be fixed in the middle screen
			*shared_context_->game_focused = true;

			// hide mouse cursor
			glutSetCursor(GLUT_CURSOR_NONE);
		}
	}

	// if the game has the focus then handle the inputs
	if (*shared_context_->game_focused == true)
	{

		// Handle the input for the camera
		camera_mgr_->handleInput(dt);

		// Handle the input for light
		light_mgr_->handleInput(dt);

		// change if show wireframe line or filled geometries
		if (shared_context_->input->isKeyDown((int)'m'))
		{
			shared_context_->input->setKeyUp((int)'m');

			// activate or deactivate wireframe depending on the current state
			if (*shared_context_->wireframe_mode == true)
				*shared_context_->wireframe_mode = false;
			else
				*shared_context_->wireframe_mode = true;
		}
		else if (shared_context_->input->isKeyDown((int)'p'))
		{
			shared_context_->input->setKeyUp((int)'p');
			// activate or deactivate paused depending on the current state
			if (paused)
				paused = false;
			else
				paused = true;

		}
	}	
}

void Scene::update(float dt)
{
	if (!paused)
	{
		// update the camera
		camera_mgr_->update(dt);

		// update the lights
		light_mgr_->update(dt);

		// update all the meshes
		for (pair<MeshesType, BaseMesh*> mesh : my_geometry_)
		{
			mesh.second->update(dt);
		}
		for (pair<MeshesType, Model*> model : models_)
		{
			model.second->update(dt);
		}
		// update the mirror worlds
		for (pair<MeshesType, MeshMirrorWorld*> mirror_world : mirror_worlds_)
		{
			mirror_world.second->update(dt);
		}
	}

	// Calculate FPS for output
	calculateFPS();
}

void Scene::render() {

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // ==> CLEAN BUFFER each frame

	// Reset transformations
	glLoadIdentity();

	// Get current camera position and vectors
	Vector3 camera_pos = camera_mgr_->getCurrentCameraPosition();
	Vector3 look_at = camera_mgr_->getCurrentCameraLookAt();
	Vector3 up = camera_mgr_->getCurrentCameraUp();

	// Set the camera
	gluLookAt(camera_pos.x, camera_pos.y, camera_pos.z, // position (eye)
		look_at.x, look_at.y, look_at.z, // look at (center)
		up.x, up.y, up.z); // up


	// Render geometry/scene here -------------------------------------

	/* Render lights */
	light_mgr_->render();

	/* Generate shadow matrix */
	float shadow_matrix[16]; // shadow
	int i = 0; // it is used for changing the stencil test value dinamicallly

	for (pair<MeshesType, MeshPlane*> floor_wall : floor_and_walls_)
	{
		Shadow::generateShadowMatrix(shadow_matrix, light_mgr_->getLightPosition(GL_LIGHT0).data(), floor_wall.second->getPQRVertices().data());

		/* Render floor seting it stencil buffer */
		glEnable(GL_STENCIL_TEST); // enable stencil
		glStencilFunc(GL_ALWAYS, 2+i, 0xffffffff);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		floor_wall.second->render();

		// Now, only render where stencil is set above 2 (ie, 3 where the top floor is).
		// Update stencil with 2 where the shadow gets drawn so we don't redraw (and accidently reblend) the shadow.
		glStencilFunc(GL_LESS, 1+i, 0xffffffff);
		glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);


		/* Render shadow */

		// disable depth test, lighting and texture
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);

		// Apply shadow's colour
		//glColor3f(0.1f, 0.1f, 0.1f); 

		glPushMatrix();

			// apply shadow matrix transform
			glMultMatrixf((GLfloat*)shadow_matrix);

			// create the shadows
			for (pair<MeshesType, BaseMesh*> mesh : my_geometry_)
			{
				mesh.second->render(true);
			}
			for (pair<MeshesType, Model*> model : models_)
			{
				model.second->render(true);
			}

		glPopMatrix();

		// reset colour
		//glColor3f(1.0f, 1.0f, 1.0f);

		// enable depth test, lighting and texture
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);

		glDisable(GL_STENCIL_TEST); // disable stencil

		i++; // for next stencil
	}

	// render actual meshes/models with their texture
	for (pair<MeshesType, BaseMesh*> mesh : my_geometry_)
	{
		mesh.second->render();
	}
	for (pair<MeshesType, Model*> model : models_)
	{
		model.second->render();
	}

	// render the mirror worlds
	for (pair<MeshesType, MeshMirrorWorld*> mirror_world : mirror_worlds_)
	{
		mirror_world.second->render();
	}


	// End render geometry --------------------------------------

	// Render text, should be last object rendered.
	renderTextOutput();

	// Swap buffers, after all objects are rendered.
	glutSwapBuffers();
}


void Scene::initialiseOpenGL()
{
	//OpenGL settings
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.39f, 0.58f, 93.0f, 1.0f);			// Cornflour Blue Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glClearStencil(0);									// Clear stencil buffer
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // How textures are applied
}

void Scene::initialiseTextures()
{
	// For my meshes
	textures[TextureName::kDonut] = new Texture("gfx/donut.png");
	textures[TextureName::kEarth] = new Texture( "gfx/earth.png");
	textures[TextureName::kDiceMap] = new Texture("gfx/dicemap.png", TextureCoordsType::kMapped);
	textures[TextureName::kDarkGrayWood] = new Texture("gfx/dark_gray_wood.jpg");
	textures[TextureName::kDarkGrayWood]->setWrapST(GL_REPEAT, GL_REPEAT);
	textures[TextureName::kMetal] = new Texture("gfx/metal.jpg");
	textures[TextureName::kMetal]->setWrapST(GL_REPEAT, GL_REPEAT);

	// For models
	textures[TextureName::kSword] = new Texture("gfx/sword_texture.jpg", TextureCoordsType::kDefault, true);
	textures[TextureName::kBronzeSword] = new Texture("gfx/sword_bronze_texture.jpg", TextureCoordsType::kDefault, true);
	textures[TextureName::kSpaceship] = new Texture("gfx/spaceship.jpg",TextureCoordsType::kDefault, true);
	textures[TextureName::kSpaceship2] = new Texture("gfx/spaceship2_texture.png");
}

void Scene::initialiseMeshes()
{
	/* Models */

	// spaceship (it works as a model) moving along z-axis
	models_[MeshesType::kSpaceship] = new Model("models/spaceship.obj");
	models_[MeshesType::kSpaceship]->setSharedContext(shared_context_);
	models_[MeshesType::kSpaceship]->setTexture(textures[TextureName::kSpaceship]);
	models_[MeshesType::kSpaceship]->setTranslation({ 1.0f, 0.13f, -2.0f });
	models_[MeshesType::kSpaceship]->setScale({ 0.90f, 0.90f, 0.90f });
	models_[MeshesType::kSpaceship]->setIsMoving({false, false, true });
	models_[MeshesType::kSpaceship]->setAxisLimits(AxisLimits{ {1.0f, 0.13f,-2.0f}, {1.0f, 0.13f,-22.0f} });
	models_[MeshesType::kSpaceship]->setDirection({0.0f,0.0f,-1.0f});
	models_[MeshesType::kSpaceship]->setSpeed(2.5f);
	camera_mgr_->linkObjToCamera(7, models_[MeshesType::kSpaceship], {0.0f, 0.3f, 0.0f}); // link the camera to the object setting the translation to set it in the eye object

	// spaceship 2 (it works as a model) moving along x-axis
	models_[MeshesType::kSpaceship2] = new Model("models/spaceship2.obj");
	models_[MeshesType::kSpaceship2]->setSharedContext(shared_context_);
	models_[MeshesType::kSpaceship2]->setTexture(textures[TextureName::kSpaceship2]);
	models_[MeshesType::kSpaceship2]->setTranslation({ 2.0f, 0.5f, -1.0f });
	models_[MeshesType::kSpaceship2]->setScale({ 0.1f, 0.1f, 0.1f });
	models_[MeshesType::kSpaceship2]->setRotationAngles({ 0.0f, 90.0f, 0.0f });
	models_[MeshesType::kSpaceship2]->setIsMoving({ true, false, false });
	models_[MeshesType::kSpaceship2]->setAxisLimits(AxisLimits{ {18.0f, 0.8f, -1.6f}, {2.0f, 0.8f, -1.6f} });
	models_[MeshesType::kSpaceship2]->setDirection({ 1.0f, 0.0f, 0.0f });
	models_[MeshesType::kSpaceship2]->setSpeed(2.5f);
	camera_mgr_->linkObjToCamera(8, models_[MeshesType::kSpaceship2], { 0.0f, 0.2f, 0.0f }); // link the camera to the object setting the translation to set it in the eye object

	// sword
	models_[MeshesType::kSword] = new Model("models/sword.obj");
	models_[MeshesType::kSword]->setSharedContext(shared_context_);
	models_[MeshesType::kSword]->setTexture(textures[TextureName::kSword]);
	models_[MeshesType::kSword]->setTranslation({ 10.0f, 2.30f, -10.0f });
	models_[MeshesType::kSword]->setScale({ 0.4f, 0.4f, 0.4f });
	models_[MeshesType::kSword]->setIsRotating({ false, true, false });
	models_[MeshesType::kSword]->setSpeed(15.0f);


	/* My Geometry */
	// To see how the world would be if it was a... =)

	// sphere
	my_geometry_[MeshesType::kSphere] = new MeshSphere(0.8, 90, 90);
	my_geometry_[MeshesType::kSphere]->setSharedContext(shared_context_);
	my_geometry_[MeshesType::kSphere]->setTexture(textures[TextureName::kEarth]);
	my_geometry_[MeshesType::kSphere]->setTranslation({ 3.0f, 1.6f, -21.0f });
	my_geometry_[MeshesType::kSphere]->setRotationAngles({ 0.0f, 0.0f, 23.5f }); // set the rotation of a earth
	my_geometry_[MeshesType::kSphere]->setIsRotating(false, true, false);
	my_geometry_[MeshesType::kSphere]->setSpeed(-15.0f);

	// cone
	my_geometry_[MeshesType::kCone] = new MeshCone(2, 0, 4, 200, 200, false, true);
	my_geometry_[MeshesType::kCone]->setSharedContext(shared_context_);
	my_geometry_[MeshesType::kCone]->setTexture(textures[TextureName::kEarth]);
	my_geometry_[MeshesType::kCone]->setScale({0.3f, 0.3f, 0.3f});
	my_geometry_[MeshesType::kCone]->setTranslation({ 6.5f, 1.0f, -21.0f });
	my_geometry_[MeshesType::kCone]->setIsRotating(false, true, false);
	my_geometry_[MeshesType::kCone]->setSpeed(+15.0f);

	// cylinder
	my_geometry_[MeshesType::kCylinder] = new MeshCone(2, 2, 4, 100, 100, true, true);
	my_geometry_[MeshesType::kCylinder]->setSharedContext(shared_context_);
	my_geometry_[MeshesType::kCylinder]->setTexture(textures[TextureName::kEarth]);
	my_geometry_[MeshesType::kCylinder]->setScale({ 0.3f, 0.3f, 0.3f });
	my_geometry_[MeshesType::kCylinder]->setTranslation({ 10.05f, 1.0f, -21.0f });
	my_geometry_[MeshesType::kCylinder]->setIsRotating(false, true, false);
	my_geometry_[MeshesType::kCylinder]->setSpeed(-15.0f);

	// pyramid
	my_geometry_[MeshesType::kPyramid] = new MeshCone(2, 0, 4, 400, 3, false, true);
	my_geometry_[MeshesType::kPyramid]->setSharedContext(shared_context_);
	my_geometry_[MeshesType::kPyramid]->setTexture(textures[TextureName::kEarth]);
	my_geometry_[MeshesType::kPyramid]->setScale({ 0.3f, 0.3f, 0.3f });
	my_geometry_[MeshesType::kPyramid]->setTranslation({ 13.5f, 1.0f, -21.0f });
	my_geometry_[MeshesType::kPyramid]->setIsRotating(false, true, false);
	my_geometry_[MeshesType::kPyramid]->setSpeed(+15.0f);

	// pentagonal
	my_geometry_[MeshesType::kPentagonal] = new MeshCone(2, 2, 4, 100, 5, true, true);
	my_geometry_[MeshesType::kPentagonal]->setSharedContext(shared_context_);
	my_geometry_[MeshesType::kPentagonal]->setTexture(textures[TextureName::kEarth]);
	my_geometry_[MeshesType::kPentagonal]->setScale({ 0.3f, 0.3f, 0.3f });
	my_geometry_[MeshesType::kPentagonal]->setTranslation({ 17.0f, 1.0f, -21.0f });
	my_geometry_[MeshesType::kPentagonal]->setIsRotating(false, true, false);
	my_geometry_[MeshesType::kPentagonal]->setSpeed(-15.0f);

	// hexagonal
	my_geometry_[MeshesType::kHexagonal] = new MeshCone(2, 2, 4, 100, 6, true, true);
	my_geometry_[MeshesType::kHexagonal]->setSharedContext(shared_context_);
	my_geometry_[MeshesType::kHexagonal]->setTexture(textures[TextureName::kEarth]);
	my_geometry_[MeshesType::kHexagonal]->setScale({ 0.3f, 0.3f, 0.3f });
	my_geometry_[MeshesType::kHexagonal]->setTranslation({ 17.0f, 1.0f, -17.0f });
	my_geometry_[MeshesType::kHexagonal]->setIsRotating(false, true, false);
	my_geometry_[MeshesType::kHexagonal]->setSpeed(+15.0f);

	// octagonal
	my_geometry_[MeshesType::kOctagonal] = new MeshCone(2, 2, 4, 100, 8, true, true);
	my_geometry_[MeshesType::kOctagonal]->setSharedContext(shared_context_);
	my_geometry_[MeshesType::kOctagonal]->setTexture(textures[TextureName::kEarth]);
	my_geometry_[MeshesType::kOctagonal]->setScale({ 0.3f, 0.3f, 0.3f });
	my_geometry_[MeshesType::kOctagonal]->setTranslation({ 17.0f, 1.0f, -13.0f });
	my_geometry_[MeshesType::kOctagonal]->setIsRotating(false, true, false);
	my_geometry_[MeshesType::kOctagonal]->setSpeed(-15.0f);

	// cube
	my_geometry_[MeshesType::kCube] = new MeshCube(4,false,RectangleBehaviourType::kUnit); // set it as a unit for the dice
	my_geometry_[MeshesType::kCube]->setSharedContext(shared_context_);
	my_geometry_[MeshesType::kCube]->setTexture(textures[TextureName::kDiceMap]);
	my_geometry_[MeshesType::kCube]->setScale({ 0.25f, 0.25f, 0.25f });
	my_geometry_[MeshesType::kCube]->setTranslation({ 17.0f, 1.0f, -9.0f });
	my_geometry_[MeshesType::kCube]->setIsRotating(false, true, false);
	my_geometry_[MeshesType::kCube]->setSpeed(+15.0f);

	// torus
	my_geometry_[MeshesType::kTorus] = new MeshTorus(1, 2, 100, 200);
	my_geometry_[MeshesType::kTorus]->setSharedContext(shared_context_);
	my_geometry_[MeshesType::kTorus]->setTexture(textures[TextureName::kDonut]);
	my_geometry_[MeshesType::kTorus]->setScale({ 0.3f, 0.3f, 0.3f });
	my_geometry_[MeshesType::kTorus]->setTranslation({ 17.0f, 1.6f, -5.0f });
	my_geometry_[MeshesType::kTorus]->setIsRotating(false, true, false);
	my_geometry_[MeshesType::kTorus]->setSpeed(+15.0f);


	/* Floor and walls where it will be printed the shadows */
	// floor
	floor_and_walls_[MeshesType::kFloor] = new MeshPlane(Facing::kUp, 24, 20);
	floor_and_walls_[MeshesType::kFloor]->setSharedContext(shared_context_);
	floor_and_walls_[MeshesType::kFloor]->setTexture(textures[TextureName::kDarkGrayWood]);

	// back wall
	floor_and_walls_[MeshesType::kWallBack] = new MeshPlane(Facing::kBackward, 10, 20);
	floor_and_walls_[MeshesType::kWallBack]->setSharedContext(shared_context_);
	floor_and_walls_[MeshesType::kWallBack]->setTranslation({ 0.0f, 0.0f, -24 }); // translate it down and forward (back)
	floor_and_walls_[MeshesType::kWallBack]->setTexture(textures[TextureName::kMetal]);

	// right wall
	floor_and_walls_[MeshesType::kWallRight] = new MeshPlane(Facing::kLeft, 10, 24);
	floor_and_walls_[MeshesType::kWallRight]->setSharedContext(shared_context_);
	floor_and_walls_[MeshesType::kWallRight]->setTranslation({ +20.0f, +10.0f, 0.0f }); // translate it down and forward (back)
	floor_and_walls_[MeshesType::kWallRight]->setTexture(textures[TextureName::kMetal]);


	/* Mirror Worlds */

	// plane mirror world
	mirror_worlds_[MeshesType::kPlaneMirror] = new MeshMirrorWorld();
	mirror_worlds_[MeshesType::kPlaneMirror]->initPlaneMirror(Facing::kRight, 10, 24); // create a rectangle mirror (plane)
	mirror_worlds_[MeshesType::kPlaneMirror]->setSharedContext(shared_context_);
	mirror_worlds_[MeshesType::kPlaneMirror]->setTranslation({ 0.0f, +10.0f, -24.0f }); // translate it down and forward (back)
	mirror_worlds_[MeshesType::kPlaneMirror]->setColour({ 0.8f, 0.8f, 1.0f, 0.3f }); // imitating a pane glass
	mirror_worlds_[MeshesType::kPlaneMirror]->createReflection(models_[MeshesType::kSpaceship]);
	mirror_worlds_[MeshesType::kPlaneMirror]->createReflection(models_[MeshesType::kSpaceship2], true);

	// disc mirror world
	mirror_worlds_[MeshesType::kDiscMirror] = new MeshMirrorWorld();
	mirror_worlds_[MeshesType::kDiscMirror]->initDiscMirror(Facing::kForward, 3, 200); // create a rectangle mirror (plane)
	mirror_worlds_[MeshesType::kDiscMirror]->setSharedContext(shared_context_);
	mirror_worlds_[MeshesType::kDiscMirror]->setTranslation({ 10.0f, 3.0f, 0.0f }); // translate it down and forward (back)
	mirror_worlds_[MeshesType::kDiscMirror]->setColour({ 0.8f, 0.8f, 1.0f, 0.3f }); // imitating a pane glass
	mirror_worlds_[MeshesType::kDiscMirror]->createReflection(models_[MeshesType::kSword],false, textures[TextureName::kBronzeSword]); // set another texture for the object reflected
	mirror_worlds_[MeshesType::kDiscMirror]->createReflection(models_[MeshesType::kSpaceship2]);

}

void Scene::initialiseMaterials()
{
	// Components to create the materials

	// no material component (black)
	Colour4 no_material_colour = { 0.0f, 0.0f, 0.0f, 1.0f };

	// material ambient component
	Colour4 mat_ambient_colour = { 0.8f, 0.8f, 0.2f, 1.0f };

	// material diffuse components
	Colour4 mat_diffuse_red_colour = { 1.0f, 0.0f, 0.0f, 1.0f };
	Colour4 mat_diffuse_grey_colour = { 0.32f, 0.32f, 0.32f, 1.0f };
	Colour4 mat_diffuse_blue_colour = { 0.0f, 0.0f, 1.0f, 0.2f };

	// material specular components
	Colour4 mat_specular_colour = { 1.0f, 1.0f, 1.0f, 1.0f };

	// material emission components
	Colour4 emission_colour = { 0.3f, 0.2f, 0.2f, 0.0f };

	// material shininess components
	GLfloat no_shininess = 0.0f;
	GLfloat low_shininess = 50.0f;
	GLfloat high_shininess = 100.0f;

	metal_material_ = new Material(GL_FRONT, mat_diffuse_grey_colour, mat_diffuse_grey_colour, mat_specular_colour, emission_colour, high_shininess); // metal efect - high shininess
}

// Handles the resize of the window. If the window changes size the perspective matrix requires re-calculation to match new window size.
void Scene::resize(int w, int h) 
{
	// update the window and height windows size
	//*shared_context_->window_width = w;
	//*shared_context_->window_height = h;

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	float ratio = (float)w / (float)h; // it should match and be base on windows size
	//fov = 45.0f; // view angle
	nearPlane = 0.1f;
	farPlane = 100.0f;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// setting up the frustum manually
	//glFrustum(left, right, bottom, top, zNear, zFar);

	// Set the correct perspective (create the frustum, based on the results we want)
	gluPerspective(camera_mgr_->getCurrentCameraFov(), ratio, nearPlane, farPlane);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

// Calculates FPS
void Scene::calculateFPS()
{
	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);

	if (time - timebase > 1000) {
		sprintf_s(fps, " FPS: %4.2f", frame*1000.0 / (time - timebase));
		timebase = time;
		frame = 0;
	}
}

// Compiles standard output text including FPS and current mouse position.
void Scene::renderTextOutput()
{
	//glEnable(GL_COLOR_MATERIAL);
	// Render current mouse position, frames per second and camera id is being used.
	sprintf_s(mouseText, " Mouse: %i, %i", shared_context_->input->getMouseX(), shared_context_->input->getMouseY());
	sprintf_s(cameraText, " Cam %i", camera_mgr_->getCurrentCameraId());
	displayText(-1.f, 0.96f, 1.f, 0.f, 0.f, mouseText);
	displayText(-1.f, 0.90f, 1.f, 0.f, 0.f, fps);
	displayText(-1.f, 0.84f, 1.f, 0.f, 0.f, cameraText);
	if(paused) // if it is paused then show text
		displayText(-1.f, 0.78f, 1.f, 0.f, 0.f, pausedText);
	//glDisable(GL_COLOR_MATERIAL);
}

// Renders text to screen. Must be called last in render function (before swap buffers)
void Scene::displayText(float x, float y, float r, float g, float b, char* string) {
	// Get Lenth of string
	int j = strlen(string);

	// Swap to 2D rendering
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, 5, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Orthographic lookAt (along the z-axis).
	gluLookAt(0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Set text colour and position.
	glColor3f(r, g, b);
	glRasterPos2f(x, y);
	// Render text.
	for (int i = 0; i < j; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, string[i]);
	}
	// Reset colour to white.
	glColor3f(1.f, 1.f, 1.f);

	// Swap back to 3D rendering.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(camera_mgr_->getCurrentCameraFov(), ((float)*shared_context_->window_width/(float)*shared_context_->window_height), nearPlane, farPlane);
	glMatrixMode(GL_MODELVIEW);
}
