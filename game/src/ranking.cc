#include "ranking.h"
#include <cmath>

float CalculateWinChance(float rating_a, float rating_b) {
    return 1.0 * 1.0 / (1 + 1.0 * pow(10, 1.0 * (rating_a - rating_b) / 400));
}

void AdjustElo(float& rating_a, float& rating_b, float k, bool a_won) {
    auto prob_a_win = CalculateWinChance(rating_b, rating_a); // The probability of Player A winning
    auto prob_b_win = CalculateWinChance(rating_a, rating_b); // The probability of Player B winning

    if (a_won) { 
        // Player A wins
        rating_a = rating_a + k * (1 - prob_a_win);
        rating_b = rating_b + k * (0 - prob_b_win);
    }       
    else {
        // Player B wins
        rating_a = rating_a + k * (0 - prob_a_win);
        rating_b = rating_b + k * (1 - prob_b_win);
    }        
}
