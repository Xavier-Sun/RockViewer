#include "Time.h"

#include <GLFW/glfw3.h>

double Time::deltaTime = 0.0;
double Time::currentTime = 0.0;
double Time::lastTime = 0.0;

void Time::Update()
{
    currentTime = glfwGetTime();
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;
}
