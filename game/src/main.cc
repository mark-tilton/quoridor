#include <iostream>
#include <fstream>
#include <chrono>
#include "random_player.h"
#include "shortest_path_player.h"
#include "game.h"
#include "windowed_game_runner.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

using namespace std;
using namespace std::chrono;
using namespace rapidjson;

void CreateSerializedGameData();
void TestGamePerformance();
void TestPlayers();
void TestActions();
void TestBoardStateCopy();
void TestBoardStateMatrices();
void TestBoardStateGetWallPoints();
 
int main(int argc, char *argv[]) {
    auto runner = WindowedGameRunner();
    runner.Run();
    
    //TestGame();
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
        game.Play(false);
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
    const auto GAME_COUNT = 10;
    for(auto depth = 1; depth < 5; depth++) {
        auto game = Game(new MinimaxPlayer(depth), new ShortestPathPlayer());
        auto start = high_resolution_clock::now();
        long turn_count = 0;
        long winner_count = 0;
        for(auto i = 0; i < GAME_COUNT; i++) {
            game.Play(false);
            turn_count += game.GetTurnCount();
            winner_count += game.GetWinner();
            game.Reset();
        }
        auto end = high_resolution_clock::now();
        const auto milliseconds = chrono::duration_cast<chrono::milliseconds>(end-start).count();
        cout << "Depth: " << depth << endl;
        cout << milliseconds << endl;											 // Elapsed Time
        cout << static_cast<double>(GAME_COUNT) / milliseconds * 1000.0 << endl; // Games per second
        cout << static_cast<double>(turn_count) / GAME_COUNT << endl;            // Avg. turns per game
        cout << static_cast<double>(turn_count) / milliseconds * 1000.0 << endl; // Avg. turns per second
        cout << GAME_COUNT - winner_count << endl;							     // Player 1 Win Count
        cout << winner_count << endl;											 // Player 2 Win Count
        cout << endl;                                    
    }    
}

void TestPlayers() {
    auto player = RandomPlayer();
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
    auto blockAction = Action(Vectori(3, 5), WallOrientation::VERTICAL);
    cout << blockAction << endl;
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