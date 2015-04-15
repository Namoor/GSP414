#pragma once

#include <time.h>

class Timer
{
public:
	Timer();

	// Liefert die Zeit, die seit dem letzten Aufruf von GetDeltaTime vergangen ist
	float GetDeltaTime();

private:
	clock_t m_LastTimeStamp;
};