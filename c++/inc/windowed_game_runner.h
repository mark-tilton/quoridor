#ifndef QUORIDORBOT_WINDOWEDGAMERUNNER_H_
#define QUORIDORBOT_WINDOWEDGAMERUNNER_H_

#include <SDL2/SDL.h>
#include <stdio.h>
#include <iostream>

#include "game.h"
#include "random_player.h"
#include "shortest_path_player.h"

using namespace std;

class WindowedGameRunner {
public:

WindowedGameRunner();
~WindowedGameRunner();

void Initialize();
void Run();
void CleanUp();

private:
void Update();
void Draw(const BoardState& board_state);

SDL_Window* window_ = nullptr;
SDL_Renderer* renderer_ = nullptr;

Game* game_ = nullptr;

};

#endif