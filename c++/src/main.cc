#include <iostream>
#include "board_state.h"
#include "vectori.h"

using namespace std;
 
int main() {
  // Vectori vec(1, 7);
  // Vectori vec1(-1, -7);
  // cout << (vec == -vec1) << endl;

  auto bs = BoardState();
  bs.SetWall(Vectori(3, 2), 2);
  auto dist_matrix = bs.GetDistanceMatrix(0);
  // cout << dist_matrix << endl;
  auto dev_marix = bs.GetDeviationMatrix(dist_matrix, Vectori(3, 7));
  cout << dist_matrix << endl;
  cout << dev_marix;
  // auto blah = BoardState::GetBlockedPaths(Vectori(3, 3), 2);

  // bs.SetWall(Vectori(3, 6), 1);
  // auto matrix = bs.GetDistanceMatrix(3);
  // cout << matrix;

  // auto points = bs.GetWallPoints(Vectori(2, 2), Vectori(0, 1));
  // std::cout << points[0] << points[1] << std::endl;
  // points = bs.GetWallPoints(Vectori(2, 2), Vectori(0, -1));
  // std::cout << points[0] << points[1] << std::endl;
  // points = bs.GetWallPoints(Vectori(2, 2), Vectori(1, 0));
  // std::cout << points[0] << points[1] << std::endl;
  // points = bs.GetWallPoints(Vectori(2, 2), Vectori(-1, 0));
  // std::cout << points[0] << points[1] << std::endl;

  //bs.SetWall(Vectori(3, 6), 7);
  // cout << bs.GetWall(Vectori(19, 10));

  return 0;
}