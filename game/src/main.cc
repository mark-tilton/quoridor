#include <iostream>
#include <fstream>
#include <chrono>
#include <atomic>
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

#include "random_player.h"
#include "shortest_path_player.h"
#include "game.h"
#include "windowed_game_runner.h"
#include "ranking.h"

using namespace std;
using namespace std::chrono;
using namespace rapidjson;

void CreateSerializedGameData();
void TestGamePerformance();
void TestPlayerPool();
void TestPlayers();
void TestActions();
void TestBoardStateCopy();
void TestBoardStateMatrices();
void TestBoardStateGetWallPoints();

int main(int argc, char *argv[]) {
    //WindowedGameRunner().Run();
    TestGamePerformance();
    //TestPlayerPool();
    //TestPlayers();
    //TestActions();
    //TestBoardStateCopy();
    //TestBoardStateMatrices();
    //TestBoardStateGetWallPoints();

    return 0;
}

void CreateSerializedGameData() {
    const int GAME_COUNT = 1000;
    auto game = Game(new ShortestPathPlayer(), new ShortestPathPlayer());

    StringBuffer sb;
    Writer<StringBuffer> writer(sb);
    writer.StartArray();

    for(int i = 0; i < GAME_COUNT; i++) {
        game.Play();
        game.Serialize(writer);
        game.Reset();
    }

    writer.EndArray();
    ofstream file;
    file.open ("games.json");
    file << sb.GetString();
    file.close();
}

void TestGamePerformance() {
    srand(time(nullptr));
    const auto GAME_COUNT = 10000;
    for(auto depth = 1; depth < 2; depth++) {
        auto start = high_resolution_clock::now();
		atomic<int> turn_count = 0;
		atomic<int> red_wins = 0;
		#pragma omp parallel for
		for (auto i = 0; i < 8; i++) {
			auto game = Game(new RandomPlayer(), new RandomPlayer(), false, false);
			for (auto i = 0; i < GAME_COUNT; i++) {
				game.Play();
				turn_count += game.GetTurnCount();
				red_wins += game.GetWinner();
				game.Reset();
				//cout << "." << flush;
			}
		}
        auto end = high_resolution_clock::now();
        const auto milliseconds = chrono::duration_cast<chrono::milliseconds>(end-start).count();
        const auto total_games = GAME_COUNT * 8;
        const auto blue_wins = total_games - red_wins;

		cout << endl;
        cout << "Depth: " << depth << endl;
        cout << "Elapsed ms:            " << milliseconds << endl;
        cout << "Games per second:      " << static_cast<double>(total_games) / milliseconds * 1000.0 << endl;
        cout << "Avg. turns per game:   " << static_cast<double>(turn_count) / total_games << endl;
        cout << "Avg. turns per second: " << static_cast<double>(turn_count) / milliseconds * 1000.0 << endl;
		cout << "Blue wins:             " << blue_wins << " (" << (blue_wins) / static_cast<double>(total_games) * 100.0 << "%)" << endl;
		cout << "Red wins:              " << red_wins << " (" << red_wins / static_cast<double>(total_games) * 100.0 << "%)" << endl;
        cout << endl;
    }
}

void TestPlayerPool() {
    srand(time(nullptr));
    auto players = vector<Player*>();
    players.emplace_back(new RandomPlayer(1.0));
    players.emplace_back(new RandomPlayer(0.9));
    players.emplace_back(new RandomPlayer(0.8));
    players.emplace_back(new RandomPlayer(0.7));
    players.emplace_back(new RandomPlayer(0.6));
    players.emplace_back(new RandomPlayer(0.5));
    players.emplace_back(new RandomPlayer(0.4));
    players.emplace_back(new RandomPlayer(0.3));
    players.emplace_back(new RandomPlayer(0.2));
    players.emplace_back(new RandomPlayer(0.1));
    players.emplace_back(new RandomPlayer(0.0));
    // players.emplace_back(new MinimaxPlayer(1, 1.0));
    // players.emplace_back(new MinimaxPlayer(1, 0.5));
    // players.emplace_back(new MinimaxPlayer(2, 1.0));
    // players.emplace_back(new MinimaxPlayer(2, 0.5));
    // players.emplace_back(new MinimaxPlayer(3, 1.0));
    // players.emplace_back(new MinimaxPlayer(3, 0.5));

    auto mmr = vector<float>(players.size(), 1200);
    auto game_count = vector<int>(players.size(), 0);

    const int GAME_COUNT = 10000;
    for(int i = 0; i < GAME_COUNT; i++) {
        int player_1_index = 0;
        int player_2_index = 0;
        while (player_1_index == player_2_index) {
            player_1_index = rand() % players.size();
            player_2_index = rand() % players.size();
        }
        auto player_1 = players[player_1_index];
        auto player_2 = players[player_2_index];
        auto game = Game(player_1, player_2, false, false);
        game.Play();
        AdjustElo(mmr[player_1_index], mmr[player_2_index], 1, game.GetWinner() == 0);
        game_count[player_1_index] += 1;
        game_count[player_2_index] += 1;
        //cout << "." << flush;
    }

    cout << endl;
    for(auto i = 0; i < players.size(); i++) {
        cout << "Player " << i << " | ELO: " << mmr[i] << " | Games: " << game_count[i] << endl;
    }
}

void TestPlayers() {
    auto player = RandomPlayer(0.75);
    player.SetIndex(0);
    auto bs = BoardState();
    for(int i = 0; i < 10; i++) {
        auto action = player.TakeAction(bs);
        cout << action << endl;
    }
}

void TestActions() {
    auto moveAction = Action(Vectori(3, 5));
    cout << moveAction << endl;
    const auto block_action = Action(Vectori(3, 5), WallOrientation::VERTICAL);
    cout << block_action << endl;
}

void TestBoardStateCopy() {
    auto original = BoardState();
    original.SetWall(Vectori(4, 1), WallOrientation::HORIZONTAL);
    original.SetPlayerPosition(1, Vectori(4, 1));
    original.SetPlayerWallCount(1, 3);
    cout << "Original" << endl;
    cout << "Original Wall Count: " << original.GetPlayerWallCount(1) << endl;
    cout << original << endl << endl;

    auto copy = BoardState(original);
    cout << "Copy" << endl;
    cout << "Copy Wall Count: " << copy.GetPlayerWallCount(1) << endl;
    cout << copy << endl << endl;

    original.SetWall(Vectori(4, 2), WallOrientation::HORIZONTAL);
    original.SetPlayerPosition(1, Vectori(4, 2));
    original.SetPlayerWallCount(1, 2);
    copy.SetWall(Vectori(4, 3), WallOrientation::HORIZONTAL);
    copy.SetPlayerPosition(1, Vectori(4, 3));
    copy.SetPlayerWallCount(1, 1);

    cout << "Modified Original" << endl;
    cout << "Modified Original Wall Count: " << original.GetPlayerWallCount(1) << endl;
    cout << original << endl << endl;

    cout << "Modified Copy" << endl;
    cout << "Modified Copy Wall Count: " << copy.GetPlayerWallCount(1) << endl;
    cout << copy << endl << endl;
}

void TestBoardStateMatrices() {
    auto bs = BoardState();
    bs.SetWall(Vectori(3, 4), WallOrientation::HORIZONTAL);
    auto dist_matrix = bs.GetDistanceMatrix(1);
    auto dev_marix = bs.CalculateDeviationMatrix(dist_matrix, Vectori(3, 7));
    cout << dist_matrix << endl;
    cout << dev_marix << endl;
}

void TestBoardStateGetWallPoints() {
    auto bs = BoardState();
    Vectori points[2];
    bs.GetWallPoints(Vectori(2, 2), Vectori(0, 1), points[0], points[1]);
    std::cout << Vectori(2, 2) << " + " << Vectori(0, 1) << " = " << points[0] << points[1] << std::endl;
    bs.GetWallPoints(Vectori(2, 2), Vectori(0, -1), points[0], points[1]);
    std::cout << Vectori(2, 2) << " + " << Vectori(0, -1) << " = " << points[0] << points[1] << std::endl;
    bs.GetWallPoints(Vectori(2, 2), Vectori(1, 0), points[0], points[1]);
    std::cout << Vectori(2, 2) << " + " << Vectori(1, 0) << " = " << points[0] << points[1] << std::endl;
    bs.GetWallPoints(Vectori(2, 2), Vectori(-1, 0), points[0], points[1]);
    std::cout << Vectori(2, 2) << " + " << Vectori(-1, 0) << " = " << points[0] << points[1] << std::endl;


    bs.GetWallPoints(Vectori(2, 2), Vectori(0, 2), points[0], points[1]);
    std::cout << Vectori(2, 2) << " + " << Vectori(0, 2) << " = " << points[0] << points[1] << std::endl;
    bs.GetWallPoints(Vectori(2, 2), Vectori(0, -2), points[0], points[1]);
    std::cout << Vectori(2, 2) << " + " << Vectori(0, -2) << " = " << points[0] << points[1] << std::endl;
    bs.GetWallPoints(Vectori(2, 2), Vectori(2, 0), points[0], points[1]);
    std::cout << Vectori(2, 2) << " + " << Vectori(2, 0) << " = " << points[0] << points[1] << std::endl;
    bs.GetWallPoints(Vectori(2, 2), Vectori(-2, 0), points[0], points[1]);
    std::cout << Vectori(2, 2) << " + " << Vectori(-2, 0) << " = " << points[0] << points[1] << std::endl;
}