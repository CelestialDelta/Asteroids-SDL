#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "../include/clock.h"
#include "../include/game.h"
#include "../include/model.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

#define PLAYER_SCALE 15
#define PLAYER_ACCELERATION 10

int32_t main(int32_t argc, char** argv)
{
	std::bitset<ARG_BUFFER> args;
	for(size_t i = 1; i < argc; i++)
	{
		std::string arg = argv[i];
		int32_t a = parse_arg(arg);
		if(a != BAD_ARG)
		{
			args[a] = 1;
		}
	}
	
	/*for(size_t i = 0; i < args.size(); i++)
	{
		if(args[i])
		{
			std::cout << i << std::endl;
		}
	}*/
	
	Model model("../res/test.txt");
	
	Game game;
	if(game.init(args))
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


















