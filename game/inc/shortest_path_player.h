#ifndef QUORIDORBOT_SHORTESTPATHPLAYER_H_
#define QUORIDORBOT_SHORTESTPATHPLAYER_H_

#include "player.h"

class ShortestPathPlayer : public Player {
public:
    ShortestPathPlayer();

    virtual Action TakeAction(const BoardState& board_state) override;

private:
    static Vectori GetBestMove(const BoardState& board_state, int player_index, const Matrix& distance_matrix);
};

#endif