#include "cgmath.h"		// slee's simple math library
#include "cgut.h"		// slee's OpenGL utility
#include "camera.h"		// camera header file
#include "trackball.h"	// trackball for develop
#include "floor.h"		// floor
#include "plate.h"		// plate
#include "pointer.h"	// pointer
#include "sphere.h"		// sphere
#include "wall.h"		// wall
#include "intro.h"		// intro
#include "sound.h"		// sound
#include "physics.h"	// physics

//*************************************
// global constants
static const char*	window_name = "Team Project - Funny Game!";
static const char*	vert_shader_path = "../bin/shaders/teamproject.vert";
static const char*  frag_shader_path = "../bin/shaders/teamproject.frag";

//*************************************
// window objects
GLFWwindow*	window = nullptr;
ivec2		window_size = cg_default_window_size(); // initial window size

//*************************************
// OpenGL objects
GLuint		program	= 0;				// ID holder for GPU program
trackball	tb_dev, tb_play;			// trackball for devlopment

//*************************************
// global variables
int			frame = 0;		// index of rendering frames
float		t = 0.0f;
bool		is_debug_mode = false;
int			collision_type = 0;
float		debug_move_speed = 0.06f;
vec2		m0 = vec2(0);
bool		sound_on = false;

//*************************************
void update()
{
	cam_for_play.update(sphere.center);

	// update projection matrix
	cam_now->aspect_ratio = window_size.x/float(window_size.y);
	cam_now->projection_matrix = mat4::perspective(cam_now->fovy, cam_now->aspect_ratio, cam_now->dNear, cam_now->dFar );
	
	// 시간 비례 업데이트 구현할 것
	t = float(glfwGetTime());

	// 공 충돌계산과 충돌물체 간의 사운드 재생
	if ((collision_type = sphere.collision(floors, walls, plates)) && sound_on && sphere.is_moving) {
		if (collision_type == 1) {
			// collide with sample
			engine->play2D(sound_src, false);
		}
		else if (collision_type == 2) {
			// collide with floor
			engine->play2D(sound_floor_src, false);
		}
		else if (collision_type == 3) {
			// collide with wall
			engine->play2D(sound_wall_src, false);
		}
		else if (collision_type == 4) {
			// collide with plate
			engine->play2D(sound_plate_src, false);
		}
	}

	// 포인터를 구에 고정
	pointer.center = sphere.center;
	
	// update uniform variables in vertex/fragment shaders
	GLint uloc;
	uloc = glGetUniformLocation( program, "view_matrix" );			if(uloc>-1) glUniformMatrix4fv( uloc, 1, GL_TRUE, cam_now->view_matrix );		// update the view matrix (covered later in viewing lecture)
	uloc = glGetUniformLocation( program, "projection_matrix" );	if(uloc>-1) glUniformMatrix4fv( uloc, 1, GL_TRUE, cam_now->projection_matrix );	// update the projection matrix (covered later in viewing lecture)
}

void render()
{
	// clear screen (with background color) and clear depth buffer
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	// notify GL that we use our own program
	glUseProgram( program );

	render_wall(program, walls);
	render_floor(program, floors);
	render_plate(program, plates);
	render_sphere(program, sphere, t);
	render_introBoard(program, introBoard);
	if(!sphere.is_moving) render_pointer(program, pointer);
	// pointer.angle += 0.02f;

	// swap front and back buffers, and display to screen
	glfwSwapBuffers( window );
}

void reshape( GLFWwindow* window, int width, int height )
{
	// set current viewport in pixels (win_x, win_y, win_width, win_height)
	// viewport: the window area that are affected by rendering 
	window_size = ivec2(width,height);
	glViewport( 0, 0, width, height );
}

void print_help()
{
	printf( "[help]\n" );
	printf( "- press ESC or 'q' to terminate the program\n" );
	printf( "- press F1 or 'h' to see help\n" );
	printf("- press F2 to see how to play\n");
	printf("- press 'TAB' to switch debug mode\n");
	printf("- press 'Z' to reset camera (debug mode only)\n");
	printf( "\n" );
}

void keyboard( GLFWwindow* window, int key, int scancode, int action, int mods )
{
	if(action==GLFW_PRESS)
	{
		if(key==GLFW_KEY_ESCAPE||key==GLFW_KEY_Q)	glfwSetWindowShouldClose( window, GL_TRUE );
		else if(key==GLFW_KEY_H||key==GLFW_KEY_F1)	print_help();	
		else if (key == GLFW_KEY_TAB) {
			if (is_debug_mode) {
				cam_now = &cam_for_play;
			}
			else {
				cam_now = &cam_for_dev;
			}
			is_debug_mode = !is_debug_mode;
			printf(" > mode change : %s mode now\n", is_debug_mode ? "debug" : "play" );
		}
		else if (key == GLFW_KEY_F2) {
			printf(" > show intro\n");
			cam_now = &cam_intro;
		}
		else if (key == GLFW_KEY_SPACE && !sphere.is_moving) {//jump charge start
			if(abs(sphere.x_speed) < 0.0001f && abs(sphere.y_speed) < 0.0001f)//이중점프 방지
				jp.startTime = float(glfwGetTime());
		}
		else if (is_debug_mode || true) {
			// debug mode only input
			if(key == GLFW_KEY_Z) cam_for_dev = camera();
			else if (key == GLFW_KEY_W) {
				//sphere.center += vec3(0, debug_move_speed*10, 0);
				sphere.y_speed += 0.1f;
			}
			else if (key == GLFW_KEY_A) {
				//sphere.center += vec3(-debug_move_speed*10, 0, 0);
				sphere.x_speed -= 0.1f;
			}
			else if (key == GLFW_KEY_S) {
				//sphere.center += vec3(0, -debug_move_speed*10, 0);
				sphere.y_speed -= 0.1f;
			}
			else if (key == GLFW_KEY_D) {
				//sphere.center += vec3(debug_move_speed*10, 0, 0);
				sphere.x_speed += 0.1f;
			}
		}
	}
	else if (action == GLFW_RELEASE)//jump charge finish
	{
		if (key == GLFW_KEY_SPACE && abs(sphere.x_speed) < 0.0001f && abs(sphere.y_speed) < 0.0001f){//이중점프 방지
			jp.endTime = float(glfwGetTime());
			jp.jump = true;
		}
	}
}

void mouse( GLFWwindow* window, int button, int action, int mods )
{
	if(button==GLFW_MOUSE_BUTTON_LEFT&&action==GLFW_PRESS )
	{
		dvec2 pos; glfwGetCursorPos(window,&pos.x,&pos.y);
		printf( "> Left mouse button pressed at (%d, %d)\n", int(pos.x), int(pos.y) );
	}

	if (is_debug_mode) {
		// ------------------------------------- track ball mouse code		---------------------------------------------//							
		if ((button == GLFW_MOUSE_BUTTON_LEFT && (mods & GLFW_MOD_SHIFT)) || button == GLFW_MOUSE_BUTTON_RIGHT)
		{
			dvec2 pos; glfwGetCursorPos(window, &pos.x, &pos.y);
			vec2 npos = cursor_to_ndc(pos, window_size);
			if (action == GLFW_PRESS)			tb_dev.begin(cam_for_dev.view_matrix, npos, ZOOMING);
			else if (action == GLFW_RELEASE)	tb_dev.end();
		}
		else if ((button == GLFW_MOUSE_BUTTON_LEFT && (mods & GLFW_MOD_CONTROL)) || button == GLFW_MOUSE_BUTTON_MIDDLE)
		{
			dvec2 pos; glfwGetCursorPos(window, &pos.x, &pos.y);
			vec2 npos = cursor_to_ndc(pos, window_size);
			if (action == GLFW_PRESS)			tb_dev.begin(cam_for_dev.view_matrix, npos, PANNING);
			else if (action == GLFW_RELEASE)	tb_dev.end();
		}
		else if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
			dvec2 pos; glfwGetCursorPos(window, &pos.x, &pos.y);
			vec2 npos = cursor_to_ndc(pos, window_size);
			if (action == GLFW_PRESS)			tb_dev.begin(cam_for_dev.view_matrix, npos, ROTATING);
			else if (action == GLFW_RELEASE)	tb_dev.end();
		}
		// --------------------------------------------------------------------------------------------------------------//
	}
	else {
		dvec2 pos; 
		glfwGetCursorPos(window, &pos.x, &pos.y);

		vec2 npos = cursor_to_ndc(pos, window_size);
		//printf("npos: (%.2f, %.2f)\n", npos.x, npos.y);
		if (action == GLFW_PRESS)			tb_dev.begin(cam_for_dev.view_matrix, npos, ROTATING);
		else if (action == GLFW_RELEASE)	tb_dev.end();
		
	}
}

void motion( GLFWwindow* window, double x, double y )
{
	if (is_debug_mode) {
		// ------------------------------------- track ball motion code		---------------------------------------------//	
		if (!tb_dev.is_tracking()) return;
		vec2 npos = cursor_to_ndc(dvec2(x, y), window_size);
		cam_for_dev.view_matrix = tb_dev.update(npos);
		// --------------------------------------------------------------------------------------------------------------//
	}
	else {
		vec2 npos = cursor_to_ndc(dvec2(x, y), window_size);
		vec4 a = cam_now->view_matrix * vec4(sphere.center, 1);
		vec2 t = vec2(npos.x - a.x, npos.y - a.y);
		float theta = atan(t.y / t.x);
		if (t.x > 0) {
			pointer.angle = theta - 0.25f*PI;
		}
		else {
			pointer.angle = theta + 0.75f*PI;
		}	
		//printf("%.2f\n", pointer.angle);
	}
}

bool user_init()
{
	
	if (sound_on && sound_init()) {
		// 각 음원의 볼륨 조정
		sound_wall_src->setDefaultVolume(0.3f);
		sound_plate_src->setDefaultVolume(0.2f);
	}
	
	

	// log hotkeys
	print_help();

	// init GL states
	glClearColor( 39/255.0f, 40/255.0f, 34/255.0f, 1.0f );	// set clear color
	glEnable( GL_CULL_FACE );								// turn on backface culling
	glEnable( GL_DEPTH_TEST );								// turn on depth tests
	glEnable(GL_TEXTURE_2D);								// enable texturing
	glActiveTexture(GL_TEXTURE0);							// notify GL the current texture slot is 0

	// load the objects we need in our project
	std::vector<vertex> unit_rect_vertices = create_rect_vertices();
	update_rect_vertex_buffer(unit_rect_vertices);
	std::vector<vertex> unit_sphere_vertices = create_sphere_vertices();
	update_sphere_vertex_buffer(unit_sphere_vertices);
	std::vector<vertex> unit_pointer_vertices = create_pointer_vertices();
	update_pointer_vertex_buffer(unit_pointer_vertices);

	// assign texture to each components.
	PlateTexture = create_texture(plate_image_path, true);
	SphereTexture = create_texture(sphere_image_path, true);
	WallTexture = create_texture(brick_image_path, true);
	FloorTexture = create_texture(floor_image_path, true);
	IntroTexture = create_texture(intro_image_path, true);
	PointerTexture = create_texture(pointer_image_path, true);

	cam_intro.eye = vec3(30.0f, 3.75f, 9.3f);
	cam_intro.at = vec3(30.0f, 3.75f, 0);
	cam_intro.update();

	return true;
}

void user_finalize()
{
	if(sound_on) engine->drop();
}

int main( int argc, char* argv[] )
{
	// create window and initialize OpenGL extensions
	if(!(window = cg_create_window( window_name, window_size.x, window_size.y ))){ glfwTerminate(); return 1; }
	if(!cg_init_extensions( window )){ glfwTerminate(); return 1; }	// version and extensions

	// initializations and validations
	if(!(program=cg_create_program( vert_shader_path, frag_shader_path ))){ glfwTerminate(); return 1; }	// create and compile shaders/program
	if(!user_init()){ printf( "Failed to user_init()\n" ); glfwTerminate(); return 1; }					// user initialization

	// register event callbacks
	glfwSetWindowSizeCallback( window, reshape );	// callback for window resizing events
    glfwSetKeyCallback( window, keyboard );			// callback for keyboard events
	glfwSetMouseButtonCallback( window, mouse );	// callback for mouse click inputs
	glfwSetCursorPosCallback( window, motion );		// callback for mouse movement

	// enters rendering/event loop
	for( frame=0; !glfwWindowShouldClose(window); frame++ )
	{
		glfwPollEvents();	// polling and processing of events
		update();			// per-frame update
		render();			// per-frame render
	}

	// normal termination
	user_finalize();
	cg_destroy_window(window);

	return 0;
}
