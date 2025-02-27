#include "windowed_game_runner.h"
#include <ctime>

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
    if (TTF_Init() == -1) 
    { 
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError()); 
        return;
    }
    font_ = TTF_OpenFont("Hack-Regular.ttf", 20);

    game_ = new Game(new MctsPlayer(3000, 20), new MinimaxPlayer(3, 1));
    StartNewGame();
    Draw(board_state_);
    SDL_RenderPresent(renderer_);

    for(int i = 0; i < SDL_NUM_SCANCODES; i++) {
        keys_[i] = false;
        keys_allow_repeat_[i] = false;
    }    
    keys_allow_repeat_[SDL_SCANCODE_LEFT] = true;
    keys_allow_repeat_[SDL_SCANCODE_RIGHT] = true;

    DebugMatrixModeLabelMap[DebugMatrixMode::DMM_NONE] = "NONE";
    DebugMatrixModeLabelMap[DebugMatrixMode::DMM_DISTANCE] = "DISTANCE";
    DebugMatrixModeLabelMap[DebugMatrixMode::DMM_DEVIATION] = "DEVIATION";
    DebugMatrixTextModeLabelMap[DebugMatrixTextMode::DMTM_OFF] = "OFF";
    DebugMatrixTextModeLabelMap[DebugMatrixTextMode::DMTM_BOTHPLAYERS] = "BOTH_PLAYERS";
    DebugMatrixTextModeLabelMap[DebugMatrixTextMode::DMTM_CURRENTPLAYER] = "CURRENT_PLAYER";
}

WindowedGameRunner::~WindowedGameRunner() {
    delete game_; 

    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
    renderer_ = nullptr;
    window_ = nullptr;
}

void WindowedGameRunner::StartNewGame() {
    game_->Reset();
    current_turn_ = 0;
    wall_player_matrix_ = Matrix(8, 8);
    const auto seed = time(NULL);
    srand(seed);
    cout << "New Game" << " (" << seed << ")" << endl;
    UpdateTurn();
}

void WindowedGameRunner::HandleInput() {
    if (keys_[SDL_SCANCODE_SPACE]) {
        keys_[SDL_SCANCODE_SPACE] = false;
        is_paused_ = !is_paused_;
        cout << "Is Paused = " << boolalpha << is_paused_ << endl;
    }
    if (keys_[SDL_SCANCODE_B]) {
        keys_[SDL_SCANCODE_B] = false;
        current_turn_ = 0;
    }    
    if (keys_[SDL_SCANCODE_E]) {
        keys_[SDL_SCANCODE_E] = false;
        auto_play_next_game_ = !auto_play_next_game_;
        cout << "Auto Play Next Game = " << boolalpha << auto_play_next_game_ << endl;
    }
    if (keys_[SDL_SCANCODE_N]) {
        keys_[SDL_SCANCODE_N] = false;
        StartNewGame();
    }
    if (keys_[SDL_SCANCODE_S]) {
        keys_[SDL_SCANCODE_S] = false;
        cout << "STATUS" << endl;
        cout << "  Turn = " << current_turn_ << endl;
        cout << "  Is Paused = " << boolalpha << is_paused_ << endl;
        cout << "  Auto Play Next Game = " << boolalpha << auto_play_next_game_ << endl;
        cout << "  FPS = " << max_fps_ << endl;
        cout << "  Debug Matrix Player: " << debug_matrix_player_ + 1 << endl;
        cout << "  Debug Matrix Mode: " << DebugMatrixModeLabelMap[debug_matrix_mode_] << endl;
        cout << "  Debug Matrix Text Mode: " << DebugMatrixTextModeLabelMap[debug_matrix_text_mode_] << endl;
    }
    if (keys_[SDL_SCANCODE_D]) {
        keys_[SDL_SCANCODE_D] = false;
        debug_matrix_mode_ = static_cast<DebugMatrixMode>((debug_matrix_mode_ + 1) % 3);
        cout << "Debug Matrix Mode: " << DebugMatrixModeLabelMap[debug_matrix_mode_] << endl;
    }
    if (keys_[SDL_SCANCODE_P]) {
        keys_[SDL_SCANCODE_P] = false;
        debug_matrix_player_ = (debug_matrix_player_ + 1) % 2;
        cout << "Debug Matrix Player: " << debug_matrix_player_ + 1 << endl;
    }
    if (keys_[SDL_SCANCODE_T]) {
        keys_[SDL_SCANCODE_T] = false;
        debug_matrix_text_mode_ = static_cast<DebugMatrixTextMode>((debug_matrix_text_mode_ + 1) % 3);
        cout << "Debug Matrix Text Mode: " << DebugMatrixTextModeLabelMap[debug_matrix_text_mode_] << endl;
    }
    if (keys_[SDL_SCANCODE_RIGHT]) {
        keys_[SDL_SCANCODE_RIGHT] = false;
        if (is_paused_ && (current_turn_ < game_->GetTurnCount() || game_->GetWinner() == -1)) {
            current_turn_ = last_turn_ + 1;
        }
    }
    if (keys_[SDL_SCANCODE_LEFT] && is_paused_) {
        keys_[SDL_SCANCODE_LEFT] = false;
        if(is_paused_ && current_turn_ > 0) {
            current_turn_ = last_turn_ - 1;
        }
    }
    if (keys_[SDL_SCANCODE_1] || keys_[SDL_SCANCODE_KP_1]) {
        keys_[SDL_SCANCODE_1] = false;
        keys_[SDL_SCANCODE_KP_1] = false;
        max_fps_ = 2;
        cout << "FPS = " << max_fps_ << endl;
    }
    if (keys_[SDL_SCANCODE_2] || keys_[SDL_SCANCODE_KP_2]) {
        keys_[SDL_SCANCODE_2] = false;
        keys_[SDL_SCANCODE_KP_2] = false;
        max_fps_ = 10;
        cout << "FPS = " << max_fps_ << endl;
    }
    if (keys_[SDL_SCANCODE_3] || keys_[SDL_SCANCODE_KP_3]) {
        keys_[SDL_SCANCODE_3] = false;
        keys_[SDL_SCANCODE_KP_3] = false;
        max_fps_ = 25;
        cout << "FPS = " << max_fps_ << endl;
    }
}

void WindowedGameRunner::Update() {
	const auto ticks = SDL_GetTicks();
	const auto ticks_per_frame = 1000 / max_fps_;
    if (ticks - last_update_ < ticks_per_frame) {
        return;
    }
    last_update_ = ticks;

    if (!is_paused_ && 
        (current_turn_ < game_->GetTurnCount() || 
            (current_turn_ == game_->GetTurnCount() && 
             game_->GetWinner() == -1))) {
        current_turn_++;
    }

    if (current_turn_ != last_turn_) {
        if (current_turn_ > game_->GetTurnCount() && game_->GetWinner() == -1) {
            cout << "Taking Turn " << current_turn_ << "..." << flush;
            game_->TakeTurn();
            cout << "\r" << flush;
        }
        UpdateTurn();        
    }

    if (!is_paused_ && current_turn_ == game_->GetTurnCount() && game_->GetWinner() != -1) {
        if (auto_play_next_game_) {
            StartNewGame();
        } else {
            is_paused_ = true;
            cout << "Is Paused = " << boolalpha << is_paused_ << endl;
        }
    }
}

void WindowedGameRunner::UpdateTurn() {
    if (current_turn_ == 0) {
        board_state_ = game_->GetInitialBoardState();
        cout << "Turn " << current_turn_ << endl;
    } 
    else {
        const auto& turn = game_->GetTurn(current_turn_ - 1);
        board_state_ = turn.GetBoardState();
        const auto& action = turn.GetAction();
        cout << "Turn " << current_turn_ << " (" << action << ")" << endl;
        if(action.GetType() == ActionType::BLOCK) {
            wall_player_matrix_[action.GetBlockPosition()] = turn.GetPlayerIndex();
        }
    }

    distance_matrices_[0] = board_state_.GetDistanceMatrix(0);
    distance_matrices_[1] = board_state_.GetDistanceMatrix(1);
    deviation_matrices_[0] = board_state_.CalculateDeviationMatrix(distance_matrices_[0], board_state_.GetPlayerPosition(0), 81);
    deviation_matrices_[1] = board_state_.CalculateDeviationMatrix(distance_matrices_[1], board_state_.GetPlayerPosition(1), 81);      

    last_turn_ = current_turn_;
}

void WindowedGameRunner::Run() {
	auto quit = false;
    SDL_Event e;

    while(!quit)
    {
	    const auto start_ticks = SDL_GetTicks();

        //Handle events on queue
        while (SDL_PollEvent(&e) != 0)
        {
            switch (e.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                if (!e.key.repeat || keys_allow_repeat_[e.key.keysym.scancode]) {
                    keys_[e.key.keysym.scancode] = true;
                }
                break;
            case SDL_KEYUP:
                keys_[e.key.keysym.scancode] = false;
                break;
            default:
                break;
            }
        }

        // Clear screen  
        SDL_SetRenderDrawColor(renderer_, 216, 216, 216, 255);
        SDL_RenderClear(renderer_); 

        // Update
        HandleInput();
        Update();
        Draw(board_state_);

        // Present 
        SDL_RenderPresent(renderer_);

	    const auto end_ticks = SDL_GetTicks();
	    const auto frame_ticks = end_ticks - start_ticks;
	    const auto ticks_per_frame = 1000 / 50;
        if (frame_ticks < ticks_per_frame) {
            SDL_Delay(ticks_per_frame - frame_ticks);
        }
    }
}

void WindowedGameRunner::Draw(const BoardState& board_state) {
    int w, h;
    SDL_GetRendererOutputSize(renderer_, &w, &h);
    auto board_size = min(w, h);
    auto origin = Vectori((w - board_size) / 2, (h - board_size) / 2);
    auto cell_width = (board_size - 8) / 9.0;
    auto cell_height = (board_size - 8) / 9.0;

    // Draw debug matrix
    if (debug_matrix_mode_ != 0) {
        auto debug_matrices = debug_matrix_mode_ == 1 ? distance_matrices_ : deviation_matrices_;
        auto debug_matrix = debug_matrices[debug_matrix_player_];
        auto debug_matrix_p1 = debug_matrices[0];
        auto debug_matrix_p2 = debug_matrices[1];
        auto max_value = debug_matrix.GetMaxValue();
        SDL_Rect cell_rect;
        SDL_Color black{0, 0, 0, 255};
        SDL_Color grey{75, 75, 75, 255};
        for(int y = 0; y < 9; y++) {
            for(int x = 0; x < 9; x++) {
                auto value = debug_matrix[Vectori(x, y)];
                if (value == -1) {
                    SDL_SetRenderDrawColor(renderer_, 128, 128, 128, 255);
                } 
                else if (value == 0) {
                    SDL_SetRenderDrawColor(renderer_, 0, 255, 0, 255);
                }
                else {
                    auto scaled_color = 230 - value / (double)max_value * 175;
                    SDL_SetRenderDrawColor(renderer_, 0, scaled_color, 0, 255);
                }
                cell_rect.x = origin.x + x * cell_width + x;
                cell_rect.y = (origin.y + board_size) - (y + 1) * cell_height - y;
                cell_rect.h = cell_height;
                cell_rect.w = cell_width;
                SDL_RenderFillRect(renderer_, &cell_rect);
                
                if (debug_matrix_text_mode_ == DMTM_BOTHPLAYERS || 
                    (debug_matrix_text_mode_ == DMTM_CURRENTPLAYER && debug_matrix_player_ == 0)) {
                    // Draw p1 matrix value text
                    auto value_p1 = debug_matrix_p1[Vectori(x, y)];
                    auto text_surface_p1 = TTF_RenderText_Solid(
                        font_, 
                        to_string(value_p1).c_str(), 
                        debug_matrix_player_ == 0 ? black : grey);
                    auto text_texture_p1 = SDL_CreateTextureFromSurface(renderer_, text_surface_p1);
                    int w, h;
                    SDL_QueryTexture(text_texture_p1, NULL, NULL, &w, &h);
                    SDL_Rect text_rect_p1;
                    text_rect_p1.x = cell_rect.x + 2;
                    text_rect_p1.y = cell_rect.y + 2;
                    text_rect_p1.w = w;
                    text_rect_p1.h = h;
                    SDL_RenderCopy(renderer_, text_texture_p1, NULL, &text_rect_p1);
                    SDL_DestroyTexture(text_texture_p1);
                    SDL_FreeSurface(text_surface_p1);
                }
                
                if (debug_matrix_text_mode_ == DMTM_BOTHPLAYERS || 
                    (debug_matrix_text_mode_ == DMTM_CURRENTPLAYER && debug_matrix_player_ == 1)) {
                    // Draw p2 matrix value text
                    auto value_p2 = debug_matrix_p2[Vectori(x, y)];
                    auto text_surface_p2 = TTF_RenderText_Solid(
                        font_, 
                        to_string(value_p2).c_str(), 
                        debug_matrix_player_ == 0 ? grey : black);
                    auto text_texture_p2 = SDL_CreateTextureFromSurface(renderer_, text_surface_p2);
                    int w, h;
                    SDL_QueryTexture(text_texture_p2, NULL, NULL, &w, &h);
                    SDL_Rect text_rect_p2;
                    text_rect_p2.x = cell_rect.x + cell_width - w - 2;
                    text_rect_p2.y = cell_rect.y + cell_height - h - 2;
                    text_rect_p2.w = w;
                    text_rect_p2.h = h;
                    SDL_RenderCopy(renderer_, text_texture_p2, NULL, &text_rect_p2);
                    SDL_DestroyTexture(text_texture_p2);
                    SDL_FreeSurface(text_surface_p2);
                }
            }
        }
    }    

    // Draw grid
    SDL_SetRenderDrawColor(renderer_, 100, 100, 100, 255);
    for(int x = 1; x < 9; x++) {
        auto x_coord = origin.x + x * cell_width + x - 1;
        SDL_RenderDrawLine(renderer_, x_coord, origin.y, x_coord, origin.y + board_size);
    }
    for(int y = 1; y < 9; y++) {
        auto y_coord = origin.y + y * cell_height + y - 1;
        SDL_RenderDrawLine(renderer_, origin.x, y_coord, origin.x + board_size, y_coord);
    }

    // Draw player 1
    SDL_SetRenderDrawColor(renderer_, 0, 0, 255, 255);
    auto p1 = board_state.GetPlayerPosition(0);
    auto p1_pos = Vectori(origin.x + (p1.x + 0.5) * cell_width + (p1.x + 1), 
        origin.y + (8.5 - p1.y) * cell_height + (9 - p1.y + 1));
    SDL_Rect p1_rect;
    p1_rect.x = p1_pos.x - cell_width / 4;
    p1_rect.y = p1_pos.y - cell_height / 4;
    p1_rect.h = cell_height / 2;
    p1_rect.w = cell_width / 2;
    SDL_RenderFillRect(renderer_, &p1_rect);

    // Draw player 2
    SDL_SetRenderDrawColor(renderer_, 255, 0, 0, 255);
    auto p2 = board_state.GetPlayerPosition(1);
    auto p2_pos = Vectori(origin.x + (p2.x + 0.5) * cell_width + (p2.x + 1), 
        origin.y + (8.5 - p2.y) * cell_height + (8 - p2.y + 1));
    SDL_Rect p2_rect;
    p2_rect.x = p2_pos.x - cell_width / 4;
    p2_rect.y = p2_pos.y - cell_height / 4;
    p2_rect.h = cell_height / 2;
    p2_rect.w = cell_height / 2;
    SDL_RenderFillRect(renderer_, &p2_rect);

    // Draw walls
    for(int y = 0; y < 8; y++) {
        for(int x = 0; x < 8; x++) {
            auto orientation = board_state.GetWall(Vectori(x, y));
            if (orientation != WallOrientation::NONE) {
                auto center = Vectori(origin.x + (x + 1) * cell_width + (x + 1),
                                      origin.y + (8 - y) * cell_height + (8 - y));
                if (wall_player_matrix_.GetValue(x, y) == 0) {
                    SDL_SetRenderDrawColor(renderer_, 0, 0, 255, 255);
                }
                else {
                    SDL_SetRenderDrawColor(renderer_, 255, 0, 0, 255);
                }
                if (orientation == WallOrientation::VERTICAL) {
                    SDL_Rect rect;
                    rect.x = center.x - 3;
                    rect.y = center.y - cell_height - 2;
                    rect.h = cell_height * 2 + 3;
                    rect.w = 5;
                    SDL_RenderFillRect(renderer_, &rect);
                }
                else {
                    SDL_Rect rect;
                    rect.x = center.x - cell_width - 2;
                    rect.y = center.y - 3;
                    rect.h = 5;
                    rect.w = cell_width * 2 + 3;
                    SDL_RenderFillRect(renderer_, &rect);
                }
            }
        }
    }
}