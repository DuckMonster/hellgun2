#include "context.h"
#include "core/engine.h"
#include "core/input.h"
#include <GLFW/glfw3.h>
#include <stdio.h>

Context context;

namespace
{
	GLFWwindow* window = NULL;
}

void handle_key_event(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_REPEAT)
		return;

/*
	if (action == GLFW_PRESS)
		printf("Key: %X\n", scancode);
		*/

	// Not a supported key...
	if (scancode > (int)Key::MAX)
		return;

	input->set_key_state((Key)scancode, action == GLFW_PRESS);
}

void handle_mouse_btn_event(GLFWwindow* window, int button, int action, int mods)
{
	input->set_mouse_state((Mouse_Btn)button, action == GLFW_PRESS);
}

void handle_mouse_move_event(GLFWwindow* window, double xpos, double ypos)
{
	input->set_mouse_position(Vec2((float)xpos, (float)ypos));
}

void handle_mouse_scroll_event(GLFWwindow* window, double xscroll, double yscroll)
{
	input->add_mouse_wheel((i32)yscroll);
}

void Context::open(const char* title, u32 in_width, u32 in_height)
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	window = glfwCreateWindow(in_width, in_height, title, NULL, NULL);
	width = in_width;
	height = in_height;

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetKeyCallback(window, handle_key_event);
	glfwSetMouseButtonCallback(window, handle_mouse_btn_event);
	glfwSetCursorPosCallback(window, handle_mouse_move_event);
	glfwSetScrollCallback(window, handle_mouse_scroll_event);
}

bool Context::is_open()
{
	return !glfwWindowShouldClose(window);
}

void Context::close()
{
	glfwSetWindowShouldClose(window, true);
}

void Context::update()
{
	glfwSwapBuffers(window);

	frame_num++;
	input->new_frame();
	glfwPollEvents();
}

void Context::lock_cursor()
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
void Context::unlock_cursor()
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}