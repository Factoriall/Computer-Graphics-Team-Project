#include "settings.h"	// 전역 상수
#include "cgmath.h"		// slee's simple math library
#include "cgut.h"		// slee's OpenGL utility
#include "camera.h"		// camera header file
#include "trackball.h"	// trackball for develop
#include "floor.h"		// floor
#include "plate.h"		// plate
#include "pointer.h"	// pointer
#include "sphere.h"		// sphere
#include "wall.h"		// wall
#include "intro.h"		// intro 기능
#include "sound.h"		// sound
#include "physics.h"	// physics
#include "text.h"		// text
#include "storm.h"		// 히오스 배너
#include "particle.h"
#include "space.h"		// 배경 

//*************************************
// global constants
static const char*	window_name = "Team Project - Funny Game!";
static const char*	vert_shader_path = "../bin/shaders/teamproject.vert";
static const char*  frag_shader_path = "../bin/shaders/teamproject.frag";
static const char*	str_play_time		 = "Play Time  : ";
static const char*	str_number_of_jump   = "Jump Count : ";

//*************************************
// window objects
GLFWwindow*	window = nullptr;
ivec2		window_size = cg_default_window_size(); // initial window size

//*************************************
// OpenGL objects
GLuint		program	= 0;				// ID holder for GPU program
trackball	tb_dev, tb_play;			// trackball for devlopment

//*************************************
// 설정용 변수
bool		sound_on = true;			// 사운드 효과 on - off
bool		is_debug_mode = false;		// 디버깅모드 on - off
int			collision_type = 0;			// 충돌 타입
float		game_speed = 1.0f;			// 게임의 전체적인 속도 조절 (기본값 x1.0)
/*	game_mod 게임의 상태
	0 - 초기화 & 인트로
	1 - 게임 중
	2 - 폭풍속으로
	3 - 폭발 이펙트
	4 - 아웃트로 (엔딩 크레딧)
*/
int			game_mod = 0;

// 게임 정보 표시용 변수
int			number_of_jump = 0;			// 점프 횟수 카운트 변수
int			frame = 0;					// 프레임 카운트 변수
int			fps = 0;					// Frame Per Second, 초당 프레임 수
float		fps_count_time = 0.0f;		// fps 측정 기준 시간
float		play_time = 0;
float		end = 0.0f;					// 게임 4단계 돌입 직후 시간저장

// 시간계산 관련 변수
float		t;							// 현재 시간
float		last_t;						// 이전 시간
float		del_t;						// del_t = t - last_t

bool		input_click = false;
int			last_colide = 0;			// 최근에 밟은 발판 확인 용
std::string	status;						// 현재 상태 표시
vec3		status_color = vec3(0.0f);
float		blink_text;					// 깜빡이는 텍스트
float		set_status_time;			// 스태이터스 자동 소멸


void set_status(int type) {
	set_status_time = 2.0f;
	if (type == 5) {
		status = std::string("SLIPPERY !!");
		status_color = vec3(0.1f, 0.1f, 1);
	}
	else if (type == 6) {
		status = std::string("STICKY !!");
		status_color = vec3(0.1f, 1, 0.1f);
	}
	else if (type == 7) {
		status = std::string("POWERFUL !!");
		status_color = vec3(1.0f, 0.1f, 0.1f);
	}
}
void reset_status() {
	status = std::string("-");
	status_color = vec3(0);
}
void automatic_reset_status(float del_t) {
	set_status_time -= del_t;
	if (set_status_time < 0) {
		reset_status();
	}
}
void update_fps() {
	// 대략 1초마다 fps 값을 갱신한다.
	if (play_time - fps_count_time > 1.0f) {
		fps_count_time += 1.0f;
		fps = frame;
		frame = 0;
	}
}
void update_camera() {
	// 카메라 matrix 계산
	if (game_mod == 1) cam_for_play.update(sphere.center);
	else if(game_mod == 2) cam_for_play.update();

	// update projection matrix
	cam_now->aspect_ratio = window_size.x / float(window_size.y);
	cam_now->projection_matrix = mat4::perspective(cam_now->fovy, cam_now->aspect_ratio, cam_now->dNear, cam_now->dFar);
}
void update_time() {
	// 시간정보 갱신
	t = float(glfwGetTime());  // now time
	del_t = min(t - last_t, 0.1f) * game_speed;
	last_t = t;
}
void game_reset() {
	reset_camera();
	//reset_floor();
	//reset_wall();
	reset_intro_state();
	//reset_plate();
	reset_sphere();
	reset_storm();
	reset_status();

	number_of_jump = 0;			// 점프 횟수 카운트 변수
	frame = 0;					// 프레임 카운트 변수
	fps = 0;					// Frame Per Second, 초당 프레임 수
	fps_count_time = 0.0f;		// fps 측정 기준 시간
	play_time = 0;
	end = 0.0f;

	game_mod = 0;
	if (sound_on) engine->stopAllSounds();
	if (sound_on) engine->play2D(sound_intro_src, false);
}

void automatic_reset() {
	if (t-end > 12.0f) {
		game_mod = -1;
		game_reset();
	}
}
//*************************************
void update()
{
	update_time();		// 시간정보 갱신
	update_space(space, del_t);	// 움직이는 배경
	update_camera();	// 카메라 위치와 시야 갱신

	if (game_mod != 0) {
		update_fps();		// fps 갱신	
	}


	// 인트로 담당, game_mod 0 -> 1 과정을 담당함
	if (game_mod == 0) {
		game_mod = running_intro(del_t, input_click);
		input_click = false;
		if (game_mod == 1) {
			cam_now = &cam_for_play;
			if (sound_on) engine->stopAllSounds();
			if (sound_on) engine->play2D(sound_playing_src, true);
		}
	}

	// 공 충돌계산과 충돌물체 간의 사운드 재생
	if (game_mod == 1 && (collision_type = sphere.collision(floors, walls, plates, del_t))) {
		if (collision_type == 1) {
			// collide with sample
			if(sound_on) engine->play2D(sound_src, false);
		}
		else if (collision_type == 2) {
			// collide with floor
			if (sound_on) engine->play2D(sound_floor_src, false);
		}
		else if (collision_type == 3) {
			// collide with wall
			if (sound_on) engine->play2D(sound_wall_src, false);
		}
		else if (collision_type == 4) {
			// collide with normal plate
			if (sound_on) engine->play2D(sound_plate_1_src, false);
		}
		else if (collision_type == 5) {
			// collide with  ice plate
			if (sound_on) engine->play2D(sound_plate_4_src, false);
			set_status(5);
		}
		else if (collision_type == 6) {
			// collide with sticky plate
			if (sound_on) engine->play2D(sound_plate_3_src, false);
			set_status(6);
		}
		else if (collision_type == 7) {
			// collide with jump plate
			if (sound_on) engine->play2D(sound_plate_2_src, false);
			set_status(7);
		}
		last_colide = collision_type;
	}

	

	
	if (game_mod == 1) {
		// 포인터를 구에 고정
		pointer.center = sphere.center;
		automatic_reset_status(del_t);

		// 점프 발판용 추가 코딩
		if (!sphere.is_moving && last_colide == 7) {
			power = (float)basic_power * 1.6f;
			more_view_angle = 18.0f;
			cam_away.y = 2;
		}
	}

	if (game_mod == 1 && storm.in_storm(sphere.center, sphere.radius)) {
		sphere_pos = sphere.center;
		del_sphere_pos = storm.center - sphere_pos;
		cam_at = cam_for_play.at;
		del_cam_at = storm.center - cam_at;
		cam_eye = cam_for_play.eye;
		del_cam_eye = storm.center + vec3(0, 0, 3) - cam_eye;
		game_mod = 2;	// 게임 모드 변경 (충돌함수가 동작하지 않게 끔)
		if (sound_on) engine->stopAllSounds();
		if (sound_on) engine->play2D(sound_storm_src, false);
	}
	if (game_mod == 2) {
		
		// 폭풍속으로 들어가는 지구 구현
		if (storm.go_to_storm(sphere, cam_for_play, del_t)) {
			game_mod = 3;	// 최종단계 진입
			if (sound_on) engine->stopAllSounds();
			if (sound_on) engine->play2D(sound_bomb_src, false);
		}
	}
	if (game_mod == 3) {
		for (auto& p : particles) p.update();
	}
	if (game_mod == 4) {
		automatic_reset();
	}
	
	
	// update uniform variables in vertex/fragment shaders
	GLint uloc;
	uloc = glGetUniformLocation( program, "view_matrix" );			if(uloc>-1) glUniformMatrix4fv( uloc, 1, GL_TRUE, cam_now->view_matrix );		// update the view matrix (covered later in viewing lecture)
	uloc = glGetUniformLocation( program, "projection_matrix" );	if(uloc>-1) glUniformMatrix4fv( uloc, 1, GL_TRUE, cam_now->projection_matrix );	// update the projection matrix (covered later in viewing lecture)
}

void render()
{
	// clear screen (with background color) and clear depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);

	render_space(program, space);
	if (game_mod == 0) {
		if(intro_descriptor_index != -1)render_introBoard(program);
		if (intro_logo) render_introLogo(program, introLogo);
	}

	if (game_mod >= 1 && game_mod < 4) {
		render_wall(program, walls);
		render_floor(program, floors);
		render_plate(program, plates);
		render_sphere(program, sphere, del_t);
		render_storm(program, storm, del_t);
		if (!sphere.is_moving) {
			render_pointer(program, pointer, jp.get_gauge(t));
		}
	}
	
	// render texts
	float dpi_scale = cg_get_dpi_scale();
	blink_text = abs(sin(t * 2.3f));
	if (game_mod == 0) {
		if(intro_next_text_ready) render_text("Click to next", window_size.x/2 -190, window_size.y / 2+160, 1.1f, vec4(1.0f, 0.0f, 0.0f, blink_text), dpi_scale);
	}
	if (game_mod == 1) {
		render_text("Statics", 20, 50, 0.5f, vec4(1.0f), dpi_scale);
		render_text((std::string("FPS : ") + std::to_string(fps)).c_str(), 20, 75, 0.5f, vec4(1.0f), dpi_scale);
		render_text((std::string(str_play_time) + std::to_string(play_time += del_t).substr(0, 4).c_str()), 20, 100, 0.5f, vec4(1.0f), dpi_scale);
		render_text((std::string(str_number_of_jump) + std::to_string(number_of_jump)).c_str(), 20, 125, 0.5f, vec4(1.0f), dpi_scale);
		render_text("Status : ", 20, 160, 0.6f, vec4(1.0f), dpi_scale);
		render_text(status, 170, 160, 0.6f, vec4(status_color, blink_text), dpi_scale);
	}
	if(game_mod == 2){
	
	}
	
	if (game_mod == 3) {
		render_text("WIN!!", window_size.x/2-120, window_size.y/2+50, 2.0f, vec4(0.0f, 0.0f, 0.0f, 1.0f), dpi_scale);
		render_text("Click to finish", window_size.x / 2 - 130, window_size.y / 2 + 130, 0.6f, vec4(1.0f, 0.0f, 0.0f, blink_text), dpi_scale);
	}
	if (game_mod == 4) {
		float dpi_scale = cg_get_dpi_scale();
		float ct = (float(glfwGetTime()) - end ) * 0.3f;
		render_text("Ending Credit", 100, GLint(ct * window_size.y / 4.0f), 1.0f, vec4(0.5f, 0.8f, 0.2f, 1.0f), dpi_scale);
		render_text("I love Computer Graphics!", 100, GLint(ct * window_size.y / 4.0f) + 25, 0.5f, vec4(0.7f, 0.4f, 0.1f, 0.8f), dpi_scale);
		render_text("Creater: Nam, Bae, Byeon", 100, GLint(ct * window_size.y / 4.0f) + 50, 0.6f, vec4(0.5f, 0.7f, 0.7f, 0.8f), dpi_scale);
		render_text("Thank you for playing!!!", 100, GLint(ct * window_size.y / 4.0f) + 75, 0.6f, vec4(1.0f, 1.0f, 1.0f, 0.8f), dpi_scale);
	}
	
	// notify GL that we use our own program
	glUseProgram(program);
	if (game_mod == 3) {
		render_particle(program, particles);
	}
	
	
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
	printf( "- press 'q' to terminate the program\n" );
	printf( "- press 'ESC' to pause the program\n");
	printf( "- press F1 or 'h' to see help\n" );
	printf("- press 'TAB' to switch debug mode\n");
	printf("- press 'Z' to reset camera (debug mode only)\n");
	printf("- press 'R' to reset game\n");
	printf( "\n" );
}

void keyboard( GLFWwindow* window, int key, int scancode, int action, int mods )
{
	if(action==GLFW_PRESS)
	{
		if(key==GLFW_KEY_Q)	glfwSetWindowShouldClose( window, GL_TRUE );
		else if(key==GLFW_KEY_H||key==GLFW_KEY_F1)	print_help();	
		else if (key == GLFW_KEY_TAB) {
			if (is_debug_mode) {
				gravity = 12.3f;
				cam_now = &cam_for_play;
			}
			else {
				gravity = 0.0f;
				cam_now = &cam_for_dev;
			}
			is_debug_mode = !is_debug_mode;
			printf(" > mode change : %s mode now\n", is_debug_mode ? "debug" : "play" );
		}
		else if (key == GLFW_KEY_ESCAPE) {
			game_speed = !game_speed;
		}
		else if (key == GLFW_KEY_F2) {
			//printf(" > show intro\n");
			//cam_now = &cam_intro;
		}
		else if (key == GLFW_KEY_R) {
			game_mod = -1;
			game_reset();
		}

		else if (key == GLFW_KEY_SPACE && !sphere.is_moving) { // jump charge start
			jp.startTime = float(glfwGetTime());
			jp.jumpping_now = true;
		}
		else if (is_debug_mode) {
			// debug mode only input
			if(key == GLFW_KEY_Z) cam_for_dev = camera();
			else if (key == GLFW_KEY_W) {
				//sphere.center += vec3(0, debug_move_speed*10, 0);
				sphere.y_speed += 1.1f;
			}
			else if (key == GLFW_KEY_A) {
				//sphere.center += vec3(-debug_move_speed*10, 0, 0);
				sphere.x_speed -= 1.1f;
			}
			else if (key == GLFW_KEY_S) {
				//sphere.center += vec3(0, -debug_move_speed*10, 0);
				sphere.y_speed -= 1.1f;
			}
			else if (key == GLFW_KEY_D) {
				//sphere.center += vec3(debug_move_speed*10, 0, 0);
				sphere.x_speed += 1.1f;
			}
		}
	}
	else if (action == GLFW_RELEASE) // jump charge finish
	{
		if (key == GLFW_KEY_SPACE && jp.jumpping_now){//이중점프 방지
			jp.endTime = float(glfwGetTime());
			jp.jumpping_now = false;
			jp.jump_action(sphere);
			reset_status();
			number_of_jump++;
		}
	}
}

void mouse( GLFWwindow* window, int button, int action, int mods )
{
	if(button==GLFW_MOUSE_BUTTON_LEFT&&action==GLFW_PRESS )
	{
		if(!input_click) input_click = true;
		if (game_mod == 3) {
			game_mod = 4;
			end = float(glfwGetTime());
			if (sound_on) engine->stopAllSounds();
			if (sound_on) engine->play2D(sound_outro_src, false);
		}
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
	
	if (sound_on && (sound_on = sound_init())) {
		// 각 음원의 볼륨 조정
		sound_wall_src->setDefaultVolume(0.5f);
		sound_plate_1_src->setDefaultVolume(0.6f); // 철판
		sound_plate_2_src->setDefaultVolume(0.84f);	// 폭발 발판
		sound_plate_3_src->setDefaultVolume(0.42f);
		sound_plate_4_src->setDefaultVolume(0.37f);
		sound_playing_src->setDefaultVolume(0.25f);
		sound_bomb_src->setDefaultVolume(0.6f);
		sound_storm_src->setDefaultVolume(0.8f);
		sound_intro_src->setDefaultVolume(0.4f);
		sound_outro_src->setDefaultVolume(0.4f);
	}
	
	// log hotkeys
	print_help();

	// init GL states
	glClearColor(39/255.0f, 40/255.0f, 34/255.0f, 1.0f);	// set clear color
	//glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);									// turn on backface culling
	glEnable(GL_DEPTH_TEST);								// turn on depth tests
	glEnable(GL_TEXTURE_2D);								// enable texturing
	glActiveTexture(GL_TEXTURE0);							// notify GL the current texture slot is 0
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// load the objects we need in our project
	std::vector<vertex> unit_rect_vertices = create_rect_vertices();
	update_rect_vertex_buffer(unit_rect_vertices);
	std::vector<vertex> unit_sphere_vertices = create_sphere_vertices();
	update_sphere_vertex_buffer(unit_sphere_vertices);
	std::vector<vertex> unit_pointer_vertices = create_pointer_vertices();
	update_pointer_vertex_buffer(unit_pointer_vertices);
	std::vector<vertex> unit_circle_vertices = create_circle_vertices();
	update_circle_vertex_buffer(unit_circle_vertices);
	std::vector<vertex> unit_particle_vertices = create_particle_vertices();
	update_particle_vertex_buffer(unit_particle_vertices);
	

	// assign texture to each components.
	Plate1Texture = create_texture(plate1_image_path, true);
	Plate2Texture = create_texture(plate2_image_path, true);
	Plate3Texture = create_texture(plate3_image_path, true);
	Plate4Texture = create_texture(plate4_image_path, true);

	SphereTexture = create_texture(sphere_image_path, true);
	SpaceTexture = create_texture(space_image_path, true);
	WallTexture = create_texture(brick_image_path, true);
	FloorTexture = create_texture(floor_image_path, true);
	//IntroTexture = create_texture(intro_image_path, true);
	PointerTexture = create_texture(pointer_image_path, true);
	StormTexture = create_texture(storm_image_path, true);
	ParticleTexture = create_texture(particle_image_path, false);
	LogoTexture = create_texture(logo_image_path, true);
	IntroTexture[0] = create_texture(intro_image_path[0], true);
	IntroTexture[1] = create_texture(intro_image_path[1], true);
	IntroTexture[2] = create_texture(intro_image_path[2], true);
	
	// setup freetype
	if (!init_text()) return false;

	particles.resize(particle_t::MAX_PARTICLES);

	// 시작 카메라
	cam_intro.eye = vec3(0, 0, 10.0f);
	cam_intro.at = vec3(0, 0, 0);
	cam_intro.update();


	// 게임 제어 변수 초기화
	game_mod = 0;
	intro_state = 0;
	if (sound_on) engine->play2D(sound_intro_src, false);

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
