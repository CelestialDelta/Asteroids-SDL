#include "../include/clock.h"

Clock::Clock()
	: _start(), _dt(), _ticking()
{
}

void Clock::start()
{
	if(!_ticking)
	{
		_ticking = true;
		_start = SteadyClock::now();
	}
}

void Clock::stop()
{
	if(_ticking)
	{
		_ticking = false;
	}
}

float Clock::tick()
{
	if(_ticking)
	{
		TimePoint t = SteadyClock::now();
		_dt = std::chrono::duration_cast<Milliseconds>(t - _start).count() / MS_PER_UPDATE;
		_start = t;
		return _dt;
	}
	return 0.0f;
}

bool Clock::is_ticking() const
{
	return _ticking;
}

float Clock::dt() const
{
	return _dt;
}

//=================================================================================================

Timer::Timer(float time)
	: _start(), _paused(), _passed(), _time(time), _ticking()
{
}

void Timer::start()
{
	if(!_ticking)
	{
		_ticking = true;
		_start = SteadyClock::now();
	}
}

void Timer::stop()
{
	if(_ticking)
	{
		_ticking = false;
	}
}

void Timer::reset()
{
	_passed = 0;
}

float Timer::tick()
{
	if(_ticking && _time >= _passed)
	{
		TimePoint t = SteadyClock::now();
		_passed = std::chrono::duration_cast<Milliseconds>(t - _start).count() / MS_PER_UPDATE;
		return _passed;
	}
	else if(_time <= _passed)
	{
		_passed = _time;
		stop();
	}
	return 0.0f;
}

void Timer::set_timer(float time)
{
	_time = time;
}

bool Timer::is_ticking() const
{
	return _ticking;
}

float Timer::time_left() const
{
	return (_time - _passed);
}
