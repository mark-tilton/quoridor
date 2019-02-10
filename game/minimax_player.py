from core.action import *
from player import Player
from core.vector2 import Vector2
from core.action import *
from core.board_state import BoardState


class MinimaxPlayer(Player):

    def __init__(self, index):
        super().__init__(index)
        self.branch_depth = 1

    def take_action(self, opponent, board_state):
        current_node = BoardStateNode(board_state, self.index, None)
        nodes_to_process = []
        child_nodes = []
        child_nodes.append(current_node)
        for i in range(self.branch_depth):
            nodes_to_process = child_nodes
            child_nodes = []
            for node in nodes_to_process:
                node.build_children()
                child_nodes.extend(node.children)

        current_node.calculate_score(False)

        return current_node.best_child.action



    #def score_board(self, board_state, player_1, player_2):
    #    player_1_dist_matrix = calculateDistanceMatrix(player_1.goal_row)
    #    player_2_dist_matrix = calculateDistanceMatrix(player_2.goal_row)
    #    player_1_dist = player_1_dist_matrix[player_1.pos.x, player_1.pos.y]
    #    player_2_dist = player_2_dist_matrix[player_2.pos.x, player_2.pos.y]
    #    score = player_2_dist - player_1_dist


class BoardStateNode():

    def __init__(self, board_state, player_index, action):
        self.board_state = board_state
        self.action = action
        self.player_index = player_index
        self.opp_index = (player_index + 1) % 2
        a_pos = board_state.player_positions[0]
        b_pos = board_state.player_positions[1]
        self.player_pos = a_pos if player_index == 0 else b_pos
        self.opp_pos = b_pos if player_index == 0 else a_pos
        self.player_goal = 8 if player_index == 0 else 0
        self.opp_goal = 0 if player_index == 0 else 8
        self.player_walls = board_state.player_walls[player_index]
        self.opp_walls = board_state.player_walls[self.opp_index]
        self.initialize_distance_matrices()
        self.children = []
        self.score = None

    def build_children(self):
        # If we are already at the end, there are no valid moves.
        player_dist = BoardStateNode.get_matrix_pos(self.player_dist, self.player_pos)
        if player_dist == 0:        
            return

        # Add all the valid movement positions to the valid moves.
        valid_actions = []
        valid_moves = self.board_state.get_valid_moves(self.player_pos, self.opp_pos)
        for move in valid_moves:
            valid_actions.append(create_move(move))

        if self.player_walls > 0:
            deviation_matrix = self.board_state.get_deviation_matrix(self.opp_dist, self.opp_pos)
            # Get a subset of wall placements we want to consider.
            # For each column
            for x in range(8):
                # For each row
                for y in range(8):
                    pos = Vector2(x, y)
                    # For each orientation
                    for o in range(1, 3):
                        # If this is a valid place to put a wall.
                        if self.board_state.is_valid_wall(pos, o):
                            # Figure out which cells this blocks.
                            blocked_paths = BoardState.get_blocked_paths(pos, o)
                            blocks_shortest_path = False
                            # Each wall blocks 2 paths, since they are 2 cells wide.
                            for i in range(2):
                                # Get the points in front and behind the wall.
                                point_a = blocked_paths[i][0]
                                point_b = blocked_paths[i][1]
                                if (self.board_state.is_cell_index_in_bounds(point_a) 
                                    and self.board_state.is_cell_index_in_bounds(point_b)):
                                    # If the cells behind and in front of the wall are in the shortest path,
                                    # we know this wall blocks along the shortest path.
                                    blocks_shortest_path = blocks_shortest_path or (
                                        deviation_matrix[int(point_a.x), int(point_a.y)] == 0
                                        and deviation_matrix[int(point_b.x), int(point_b.y)] == 0)
                            if blocks_shortest_path:
                                valid_actions.append(create_block(pos, o))

        # Create a board state for each of the valid actions.
        for action in valid_actions:
            board_state = self.board_state.apply_action(action, self.player_index)
            child_node = BoardStateNode(board_state, self.opp_index, action)
            if child_node.is_valid:
                self.children.append(child_node)

    def calculate_score(self, minimize):
        if len(self.children) == 0:
            # When the board has no children calculate the distances from the end for each player.
            opp_dist = self.opp_dist[self.opp_pos.x, self.opp_pos.y]
            player_dist = self.player_dist[self.player_pos.x, self.player_pos.y]
            self.score = (opp_dist - player_dist)
        else:
            min_max_score = None

            for child in self.children:
                child.calculate_score(not minimize)

            # On our opponents turns we are trying to minimize the score of the boards.
            # We assume that the opponent is going to make the worst possible move
            # for us.
            if minimize:
                # Since we don't know for sure what the oppoent will end up doing,
                # we take the 5 worst moves (for us) and average their value.
                sorted_children = sorted(self.children, key=lambda x: x.score)

                # Take the top 5 and average them.
                count =  min(5, len(sorted_children))
                min_max_score = 0
                for i in range(count):
                    min_max_score += sorted_children[i].score

                min_max_score = min_max_score / count
                self.best_child = sorted_children[0]
            else:
                # On our turn we can assume we will make the best move so
                # we just find the maximum score.
                for child in self.children:
                    if min_max_score == None or child.score > min_max_score:
                        min_max_score = child.score
                        self.best_child = child
            
            self.score = min_max_score

        return self.score

    def initialize_distance_matrices(self):
        self.player_dist = self.board_state.get_distance_matrix_from_row(self.player_goal)
        self.opp_dist = self.board_state.get_distance_matrix_from_row(self.opp_goal)
        self.is_valid = (self.get_matrix_pos(self.player_dist, self.player_pos) != None 
            and self.get_matrix_pos(self.opp_dist, self.opp_pos) != None)

    @staticmethod
    def get_matrix_pos(matrix, pos):
        return matrix[pos.x, pos.y]