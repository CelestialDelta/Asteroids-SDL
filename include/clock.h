#pragma once

#include <chrono>

typedef std::chrono::steady_clock SteadyClock;
typedef std::chrono::time_point<SteadyClock> TimePoint;
typedef std::chrono::duration<float, std::milli> Milliseconds;

#define MS_PER_UPDATE 1000

class Clock
{
public:
	Clock();
	
	void start();
	void stop();
	float tick();
	
	bool is_ticking() const;
	float dt() const;
private:
	TimePoint _start;
	TimePoint _paused;
	float _dt;
	bool _ticking;
};

class Timer
{
public:
	//time in seconds
	Timer(float time);
	
	void start();
	void stop();
	void reset();
	float tick();
	
	void set_timer(float time);
	
	bool is_ticking() const;
	float time_left() const;
private:
	TimePoint _start;
	TimePoint _paused;
	float _passed;
	float _time;
	bool _ticking;
};
