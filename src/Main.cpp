#include "Game.h"

static constexpr auto Scale = 1;
static constexpr auto FontSize = 8;
static constexpr auto ScreenWidth = 145;
static constexpr auto ScreenHeight = 75;

int main(int argc, char** argv)
{
	Game game;
	game.Construct(ScreenWidth * FontSize, ScreenHeight * FontSize, Scale, Scale);
	game.Start();

	return 0;
}
