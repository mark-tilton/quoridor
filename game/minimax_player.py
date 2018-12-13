from core.action import *
from player import Player


class MinimaxPlayer(Player):

    def __init__(self, goal_row, starting_pos):
        super().__init__(goal_row, starting_pos)
        self.branch_depth = 2

    def take_action(self, opponent, board_state):
        is_a = self.goal_row == 1
        w_is_a = is_a
        current_node = BoardStateNode(board_state, self, opponent)
        nodes_to_process = []
        child_nodes = []
        child_nodes.append(current_node)
        for i in range(self.branch_depth):
            nodes_to_process = child_nodes
            child_nodes = []
            for node in nodes_to_process:
                node.build_children(w_is_a)
                if node.get_children() is not None:
                    child_nodes.extend(node.get_children())
            w_is_a = not w_is_a

        current_node.getScore(isA, false)
        # currentBoard.printBestChain()

        return current_node.getBestChild().getAction().getCoord()

    def score_board(self, board_state, player_1, player_2):
        player_1_dist_matrix = calculateDistanceMatrix(player_1.goal_row)
        player_2_dist_matrix = calculateDistanceMatrix(player_2.goal_row)
        player_1_dist = player_1_dist_matrix[player_1.pos.x, player_1.pos.y]
        player_2_dist = player_2_dist_matrix[player_2.pos.x, player_2.pos.y]
        score = player_2_dist - player_1_dist


class BoardStateNode():

    def __init__(self, board_state, player, opponent):
        self.board_state = board_state

    def build_children(self):
        pass

    def calculate_score(self):
        pass
