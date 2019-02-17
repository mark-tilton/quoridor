#include <iostream>
#include "board_state.h"
#include "vectori.h"

using namespace std;

void TestBoardStateCopy();
void TestBoardStateMatrices();
void TestBoardStateGetWallPoints();
 
int main() {
  TestBoardStateCopy();
  //TestBoardStateMatrices();
  //TestBoardStateGetWallPoints();

  return 0;
}

void TestBoardStateCopy() {
    auto original = BoardState();
    original.SetWall(Vectori(4, 1), 2);
    original.SetPlayerPosition(1, Vectori(4, 1));
    original.SetPlayerWallCount(1, 3);
    cout << "Original" << endl;
    cout << "Original Wall Count: " << original.GetPlayerWallCount(1) << endl;
    cout << original << endl << endl;

    auto copy = BoardState(original);
    cout << "Copy" << endl;
    cout << "Copy Wall Count: " << copy.GetPlayerWallCount(1) << endl;
    cout << copy << endl << endl;

    original.SetWall(Vectori(4, 2), 2);
    original.SetPlayerPosition(1, Vectori(4, 2));
    original.SetPlayerWallCount(1, 2);
    copy.SetWall(Vectori(4, 3), 2);
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
    bs.SetWall(Vectori(3, 4), 2);
    auto dist_matrix = bs.GetDistanceMatrix(0);
    auto dev_marix = bs.GetDeviationMatrix(dist_matrix, Vectori(3, 7));
    cout << dist_matrix << endl;
    cout << dev_marix << endl;
}

void TestBoardStateGetWallPoints() {
    auto bs = BoardState();
    auto points = bs.GetWallPoints(Vectori(2, 2), Vectori(0, 1));
    std::cout << points[0] << points[1] << std::endl;
    points = bs.GetWallPoints(Vectori(2, 2), Vectori(0, -1));
    std::cout << points[0] << points[1] << std::endl;
    points = bs.GetWallPoints(Vectori(2, 2), Vectori(1, 0));
    std::cout << points[0] << points[1] << std::endl;
    points = bs.GetWallPoints(Vectori(2, 2), Vectori(-1, 0));
    std::cout << points[0] << points[1] << std::endl;
}