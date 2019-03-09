#ifndef QUORIDORBOT_RANKING_H_
#define QUORIDORBOT_RANKING_H_

float CalculateWinChance(float rating_a, float rating_b);
void AdjustElo(float& rating_a, float& rating_b, float k, bool a_won);

#endif