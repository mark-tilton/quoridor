#include "windowed_game_runner.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;

WindowedGameRunner::WindowedGameRunner() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return;
	}
    window_ = SDL_CreateWindow("Quoridor Bot", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window_ == nullptr)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return;
    }
    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED); 
    if (renderer_ == nullptr) 
    { 
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return;
    }

    game_ = new Game(new ShortestPathPlayer(), new ShortestPathPlayer());
}

WindowedGameRunner::~WindowedGameRunner() {
    delete game_; 

    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
    renderer_ = nullptr;
    window_ = nullptr;
}

void WindowedGameRunner::Update() {
    game_->TakeTurn();
    Draw(game_->GetCurrentBoard());
    if (game_->GetWinner() != -1) {
        game_->Reset();
    }
}

void WindowedGameRunner::Run() {
    bool quit = false;

    SDL_Event e;

    while(!quit)
    {
        //Handle events on queue
        while (SDL_PollEvent(&e) != 0)
        {
            if(e.type == SDL_QUIT)
            {
                quit = true;
                break;
            }
        }

        // Clear screen  
        SDL_SetRenderDrawColor(renderer_, 216, 216, 216, 255);
        SDL_RenderClear(renderer_); 

        // Update
        Update();

        // Present 
        SDL_RenderPresent(renderer_);

        SDL_Delay(16);
    }
}

void WindowedGameRunner::Draw(const BoardState& board_state) {
    const int wall_thickness = 1;
    int w, h;
    SDL_GetRendererOutputSize(renderer_, &w, &h);
    auto board_size = min(w, h);
    auto origin = Vectori((w - board_size) / 2, (h - board_size) / 2);
    auto cell_width = (board_size - (wall_thickness * 10)) / 9.0;
    auto cell_height = (board_size - (wall_thickness * 10)) / 9.0;

    // Draw grid
    SDL_SetRenderDrawColor(renderer_, 200, 200, 200, 255);
    for(int x = 1; x < 9; x++) {
        auto x_coord = origin.x + x * cell_width + x * wall_thickness;
        SDL_RenderDrawLine(renderer_, x_coord, origin.y, x_coord, origin.y + board_size);
    }
    for(int y = 1; y < 9; y++) {
        auto y_coord = origin.y + y * cell_height + y * wall_thickness;
        SDL_RenderDrawLine(renderer_, origin.x, y_coord, origin.x + board_size, y_coord);
    }

    // Draw player 1
    SDL_SetRenderDrawColor(renderer_, 0, 0, 255, 255);
    auto p1 = board_state.GetPlayerPosition(0);
    auto p1_pos = Vectori(origin.x + (p1.x + 0.5) * cell_width + (p1.x + 1) * wall_thickness, 
        origin.y + (8.5 - p1.y) * cell_height + (9 - p1.y + 1) * wall_thickness);
    SDL_Rect p1_rect;
    p1_rect.x = p1_pos.x - cell_width / 4;
    p1_rect.y = p1_pos.y - cell_height / 4;
    p1_rect.h = cell_height / 2;
    p1_rect.w = cell_height / 2;
    SDL_RenderFillRect(renderer_, &p1_rect);

    // Draw player 2
    SDL_SetRenderDrawColor(renderer_, 255, 0, 0, 255);
    auto p2 = board_state.GetPlayerPosition(1);
    auto p2_pos = Vectori(origin.x + (p2.x + 0.5) * cell_width + (p2.x + 1) * wall_thickness, 
        origin.y + (8.5 - p2.y) * cell_height + (8 - p2.y + 1) * wall_thickness);
    SDL_Rect p2_rect;
    p2_rect.x = p2_pos.x - cell_width / 4;
    p2_rect.y = p2_pos.y - cell_height / 4;
    p2_rect.h = cell_height / 2;
    p2_rect.w = cell_height / 2;
    SDL_RenderFillRect(renderer_, &p2_rect);

    // Draw walls
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
    for(int y = 0; y < 8; y++) {
        for(int x = 0; x < 8; x++) {
            auto wall_scale_factor = 1;
            auto orientation = board_state.GetWall(Vectori(x, y));
            if (orientation != WallOrientation::NONE) {
                auto center = Vectori(origin.x + (x + 1) * cell_width + (x + 1) * wall_thickness,
                                      origin.y + (8 - y) * cell_height + (8 - y) * wall_thickness);
                if (orientation == WallOrientation::VERTICAL) {
                    auto p1 = Vectori(center.x, center.y - cell_height * wall_scale_factor);
                    auto p2 = Vectori(center.x, center.y + cell_height * wall_scale_factor);
                    SDL_RenderDrawLine(renderer_, p1.x, p1.y, p2.x, p2.y);
                }
                else {
                    auto p1 = Vectori(center.x - cell_width * wall_scale_factor, center.y);
                    auto p2 = Vectori(center.x + cell_width * wall_scale_factor, center.y);
                    SDL_RenderDrawLine(renderer_, p1.x, p1.y, p2.x, p2.y);
                }
            }
        }
    }
}