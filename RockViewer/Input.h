#ifndef INPUT_H
#define INPUT_H

#include "Window.h"

enum class KeyCode
{
	A,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z
};

enum class AxisType
{
	MouseX,
	MouseY,
	ScrollX,
	ScrollY
};

static class Input
{
public:
	static bool GetKey(KeyCode key);
	static bool GetMouseButton(unsigned int button);
	static double GetAxisRaw(AxisType axis);

	static void Init();
	static void Update();
	
private:
	static double lastCursorX;
	static double lastCursorY;
	static double cursorX;
	static double cursorY;
	static double scrollX;
	static double scrollY;

	friend void ScrollCallback(GLFWwindow*, double, double);
};

#endif