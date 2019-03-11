#ifndef QUORIDORBOT_MCTS_NODE_H_
#define QUORIDORBOT_MCTS_NODE_H_

#include "player.h"
#include <vector>
#include <optional>

class MctsNode {

public:
	// Constructors
	MctsNode(const BoardState board_state, const std::optional<Action> action, MctsNode* parent);

	// Accessors
	MctsNode& operator[] (int i);
	std::vector<MctsNode>::iterator begin();
	std::vector<MctsNode>::iterator end();
	const BoardState& GetBoardState() const;
	const std::optional<Action>& GetAction() const;
	MctsNode* GetParent() const;
	int GetChildCount() const;
	int GetVisitCount() const;
	double GetScore() const;

	// Mutators
	std::vector<MctsNode> children_;
	void Expand(MctsNode child);
	void Visit(double score);

	template <typename Writer>
	void Serialize(Writer& writer) const {
		if (visit_count_ == 1) return;

		writer.StartObject();

		if (action_.has_value()) {
			writer.Key("action");
			action_.value().Serialize(writer);
		}

		writer.Key("score");
		writer.Double(score_);

		writer.Key("visitor_count");
		writer.Int(visit_count_);

		writer.Key("children");
		writer.StartArray();
		for (auto child : children_) {
			child.Serialize(writer);
		}
		writer.EndArray();

		writer.EndObject();
	}

private:
	const std::optional<Action> action_;
	double score_ = 0;
	int visit_count_ = 0;
	MctsNode* parent_;
	const BoardState board_state_;

};

#endif