#ifndef QUORIDORBOT_MCTS_PLAYER_H_
#define QUORIDORBOT_MCTS_PLAYER_H_

#include "player.h"

class MctsPlayer : public Player {

public:
	MctsPlayer(long long time_out);
	virtual Action TakeAction(const BoardState& board_state) override;

private:
	long long time_out_;

};

#endif