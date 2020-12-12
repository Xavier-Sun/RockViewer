#include "Input.h"

#include "Window.h"

double Input::lastCursorX = 0.0;
double Input::lastCursorY = 0.0;
double Input::cursorX = 0.0;
double Input::cursorY = 0.0;
double Input::scrollX = 0.0;
double Input::scrollY = 0.0;

bool Input::GetKey(KeyCode key)
{
	switch (key)
	{
	case KeyCode::A:
		return glfwGetKey(Window::GetInstance().GetGLFWwindow(), GLFW_KEY_A) == GLFW_PRESS;
		break;
	case KeyCode::B:
		return glfwGetKey(Window::GetInstance().GetGLFWwindow(), GLFW_KEY_B) == GLFW_PRESS;
		break;
	case KeyCode::C:
		return glfwGetKey(Window::GetInstance().GetGLFWwindow(), GLFW_KEY_C) == GLFW_PRESS;
		break;
	case KeyCode::D:
		return glfwGetKey(Window::GetInstance().GetGLFWwindow(), GLFW_KEY_D) == GLFW_PRESS;
		break;
	case KeyCode::E:
		return glfwGetKey(Window::GetInstance().GetGLFWwindow(), GLFW_KEY_E) == GLFW_PRESS;
		break;
	case KeyCode::F:
		return glfwGetKey(Window::GetInstance().GetGLFWwindow(), GLFW_KEY_F) == GLFW_PRESS;
		break;
	case KeyCode::G:
		return glfwGetKey(Window::GetInstance().GetGLFWwindow(), GLFW_KEY_G) == GLFW_PRESS;
		break;
	case KeyCode::H:
		return glfwGetKey(Window::GetInstance().GetGLFWwindow(), GLFW_KEY_H) == GLFW_PRESS;
		break;
	case KeyCode::I:
		return glfwGetKey(Window::GetInstance().GetGLFWwindow(), GLFW_KEY_I) == GLFW_PRESS;
		break;
	case KeyCode::J:
		return glfwGetKey(Window::GetInstance().GetGLFWwindow(), GLFW_KEY_J) == GLFW_PRESS;
		break;
	case KeyCode::K:
		return glfwGetKey(Window::GetInstance().GetGLFWwindow(), GLFW_KEY_K) == GLFW_PRESS;
		break;
	case KeyCode::L:
		return glfwGetKey(Window::GetInstance().GetGLFWwindow(), GLFW_KEY_L) == GLFW_PRESS;
		break;
	case KeyCode::M:
		return glfwGetKey(Window::GetInstance().GetGLFWwindow(), GLFW_KEY_M) == GLFW_PRESS;
		break;
	case KeyCode::N:
		return glfwGetKey(Window::GetInstance().GetGLFWwindow(), GLFW_KEY_N) == GLFW_PRESS;
		break;
	case KeyCode::O:
		return glfwGetKey(Window::GetInstance().GetGLFWwindow(), GLFW_KEY_O) == GLFW_PRESS;
		break;
	case KeyCode::P:
		return glfwGetKey(Window::GetInstance().GetGLFWwindow(), GLFW_KEY_P) == GLFW_PRESS;
		break;
	case KeyCode::Q:
		return glfwGetKey(Window::GetInstance().GetGLFWwindow(), GLFW_KEY_Q) == GLFW_PRESS;
		break;
	case KeyCode::R:
		return glfwGetKey(Window::GetInstance().GetGLFWwindow(), GLFW_KEY_R) == GLFW_PRESS;
		break;
	case KeyCode::S:
		return glfwGetKey(Window::GetInstance().GetGLFWwindow(), GLFW_KEY_S) == GLFW_PRESS;
		break;
	case KeyCode::T:
		return glfwGetKey(Window::GetInstance().GetGLFWwindow(), GLFW_KEY_T) == GLFW_PRESS;
		break;
	case KeyCode::U:
		return glfwGetKey(Window::GetInstance().GetGLFWwindow(), GLFW_KEY_U) == GLFW_PRESS;
		break;
	case KeyCode::V:
		return glfwGetKey(Window::GetInstance().GetGLFWwindow(), GLFW_KEY_V) == GLFW_PRESS;
		break;
	case KeyCode::W:
		return glfwGetKey(Window::GetInstance().GetGLFWwindow(), GLFW_KEY_W) == GLFW_PRESS;
		break;
	case KeyCode::X:
		return glfwGetKey(Window::GetInstance().GetGLFWwindow(), GLFW_KEY_X) == GLFW_PRESS;
		break;
	case KeyCode::Y:
		return glfwGetKey(Window::GetInstance().GetGLFWwindow(), GLFW_KEY_Y) == GLFW_PRESS;
		break;
	case KeyCode::Z:
		return glfwGetKey(Window::GetInstance().GetGLFWwindow(), GLFW_KEY_Z) == GLFW_PRESS;
		break;
	default:
		break;
	}
	return false;
}

bool Input::GetMouseButton(unsigned int button)
{
	switch (button)
	{
	case 0u:
		return glfwGetMouseButton(Window::GetInstance().GetGLFWwindow(), GLFW_MOUSE_BUTTON_1);
		break;
	case 1u:
		return glfwGetMouseButton(Window::GetInstance().GetGLFWwindow(), GLFW_MOUSE_BUTTON_2);
		break;
	default:
		break;
	}
	return false;
}

double Input::GetAxisRaw(AxisType axis)
{
	switch (axis)
	{
	case AxisType::MouseX:
		return cursorX - lastCursorX;
		break;
	case AxisType::MouseY:
		return cursorY - lastCursorY;
		break;
	case AxisType::ScrollX:
		return scrollX;
		break;
	case AxisType::ScrollY:
		return scrollY;
		break;
	default:
		break;
	}
	return 0.0;
}

void ScrollCallback(GLFWwindow* window, double x, double y)
{
	Input::scrollX = x;
	Input::scrollY = y;
}

void Input::Init()
{
	glfwSetScrollCallback(Window::GetInstance().GetGLFWwindow(), ScrollCallback);
}

void Input::Update()
{
	Input::lastCursorX = Input::cursorX;
	Input::lastCursorY = Input::cursorY;
	glfwGetCursorPos(Window::GetInstance().GetGLFWwindow(), &Input::cursorX, &Input::cursorY);
}