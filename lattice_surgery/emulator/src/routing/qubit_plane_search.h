#pragma once

#include "common.h"
#include "position.h"

enum class PathConnectionType {
    VERTICAL,
    HORIZONTAL,
    BOTH
};

enum class NodeStatus {
    FREE,
    USED,
    START,
    GOAL,
};

enum class PathConnectionFrom {
    FROM_LEFT,
    FROM_RIGHT,
    FROM_UP,
    FROM_DOWN,
};


struct Node {
    int cost = 0;
    PathConnectionType con_type = PathConnectionType::BOTH;
    NodeStatus status = NodeStatus::FREE;
    PathConnectionFrom from_type = PathConnectionFrom::FROM_LEFT;
};

class SearchField {
private:
    vector<vector<Node>> field;
    const int m_width;
    const int m_height;
    const int INF = 1 << 30;
    Node& get(const Position2D& pos) {
        return field[pos.y][pos.x];
    }
    bool is_bounded(const Position2D& pos) {
        return 0 <= pos.x && pos.x < m_width && 0 <= pos.y && pos.y < m_height;
    }
public:
    SearchField(int width, int height) : field(height, vector<Node>(width)), m_width(width), m_height(height) {};
    void set_status(const Position2D& pos, NodeStatus status, PathConnectionType con_type) {
        field[pos.y][pos.x].status = status;
        field[pos.y][pos.x].con_type = con_type;
    }
    pair<bool, vector<Position2D>> bfs() {

        // gather start and end node
        queue<Position2D> bfs_queue;
        vector<Position2D> goal_list;
        for (int y = 0; y < (signed)field.size(); ++y) {
            for (int x = 0; x < (signed)field[y].size(); ++x) {
                auto status = field[y][x].status;
                Position2D pos(x, y);
                if (status == NodeStatus::START) {
                    if (!bfs_queue.empty()) {
                        throw std::runtime_error("cannot append two start nodes");
                    }
                    bfs_queue.push(pos);
                    get(pos).cost = 0;
                }
                else if (status == NodeStatus::GOAL) {
                    goal_list.push_back(pos);
                    get(pos).cost = INF;
                }
                else {
                    get(pos).cost = INF;
                }
            }
        }

        // perform BFS
        while (!bfs_queue.empty()) {
            auto pos = bfs_queue.front();
            bfs_queue.pop();
            const Node& node = get(pos);

            vector<pair<Position2D, PathConnectionFrom>> candidate;
            if (node.con_type != PathConnectionType::HORIZONTAL) {
                candidate.push_back(make_pair(Position2D(pos.x, pos.y + 1), PathConnectionFrom::FROM_UP));
                candidate.push_back(make_pair(Position2D(pos.x, pos.y - 1), PathConnectionFrom::FROM_DOWN));
            }
            if (node.con_type != PathConnectionType::VERTICAL) {
                candidate.push_back(make_pair(Position2D(pos.x + 1, pos.y), PathConnectionFrom::FROM_LEFT));
                candidate.push_back(make_pair(Position2D(pos.x - 1, pos.y), PathConnectionFrom::FROM_RIGHT));
            }

            for (const auto& neighbor : candidate) {
                auto next_pos = neighbor.first;
                auto next_from = neighbor.second;

                // check bounded
                if (!is_bounded(next_pos)) continue;
                Node& next = get(next_pos);

                // skip used node
                if (next.status == NodeStatus::USED) continue;

                // skip lower cost node
                if (next.cost <= node.cost + 1) continue;

                // skip if goal_node is connected from invalid direction
                if (next.status == NodeStatus::GOAL && next.con_type != PathConnectionType::BOTH) {
                    if (next.con_type == PathConnectionType::HORIZONTAL && (next_from == PathConnectionFrom::FROM_UP || next_from == PathConnectionFrom::FROM_DOWN)) continue;
                    if (next.con_type == PathConnectionType::VERTICAL && (next_from == PathConnectionFrom::FROM_LEFT || next_from == PathConnectionFrom::FROM_RIGHT)) continue;
                }

                // update node
                next.cost = node.cost + 1;
                next.from_type = next_from;

                // apppend if not goal
                if (next.status != NodeStatus::GOAL)
                    bfs_queue.push(next_pos);
            }
        }

        // check satisfied
        bool result = true;
        for (const auto& pos : goal_list) {
            if (get(pos).cost == INF) result = false;
        }
        if (!result) {
            vector<Position2D> dummy;
            return make_pair(false, dummy);
        }

        // gather paths
        vector<Position2D> path;
        for (const auto& pos : goal_list) {
            int x = pos.x;
            int y = pos.y;
            while (true) {
                Position2D current_pos(x, y);
                auto node = get(current_pos);
                if (node.status == NodeStatus::START) break;
                if (node.status != NodeStatus::GOAL) {
                    path.push_back(current_pos);
                }
                if (node.status == NodeStatus::USED) {
                    throw std::runtime_error("invalid node reached");
                }

                if (node.from_type == PathConnectionFrom::FROM_DOWN) y += 1;
                else if (node.from_type == PathConnectionFrom::FROM_UP) y -= 1;
                else if (node.from_type == PathConnectionFrom::FROM_LEFT) x -= 1;
                else if (node.from_type == PathConnectionFrom::FROM_RIGHT) x += 1;

                if (!is_bounded(Position2D(x, y))) {
                    throw std::runtime_error("invalid node reached");
                }
            }
        }

        // remove duplicated nodes
        auto ite = unique(path.begin(), path.end());
        path.erase(ite, path.end());

        return make_pair(true, path);
    }
};
