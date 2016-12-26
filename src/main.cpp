#include <iostream>
#include <stdint.h>
#include <vector>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "../include/clock.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

#define PLAYER_SCALE 15
#define PLAYER_ACCELERATION 10

#include "../include/game.h"

int32_t main()
{
	Game game;
	if(game.init())
	{
		Clock clock;
		clock.start();
		while(game.is_running())
		{
			clock.tick();
			float dt = clock.dt();
			
			game.handle(dt);
			game.update(dt);
			game.draw();
		}
	}

	return 0;
}


















