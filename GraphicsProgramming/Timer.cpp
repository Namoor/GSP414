#include "Timer.h"


Timer::Timer()
{
	m_LastTimeStamp = clock();
}



float Timer::GetDeltaTime()
{
	clock_t _CurrentTime = clock();

	float _Delta = (_CurrentTime - m_LastTimeStamp) / (float)CLOCKS_PER_SEC;

	m_LastTimeStamp = _CurrentTime;

	return _Delta;
}