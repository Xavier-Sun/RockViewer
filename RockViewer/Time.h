#ifndef TIME_H
#define TIME_H

static class Time
{
public:
	static double GetDeltaTime() { return deltaTime; }
	static double GetCurrentTime() { return currentTime; }

	static void Update();

private:
	static double deltaTime;
	static double currentTime;
	static double lastTime;
};

#endif