#pragma once

#include "common.h"
#include "position.h"

enum class PathConnectionType{
    VERTICAL,
    HORIZONTAL,
    BOTH
};

enum class NodeStatus : int{
    FREE,
    USED,
    START,
    GOAL
};

enum class PathConnectionFrom{
    FROM_LEFT,
    FROM_RIGHT,
    FROM_UP,
    FROM_DOWN,
    FROM_FRONT,
    FROM_BACK
};

inline std::string NodeStatusToString(NodeStatus status) {
    switch (status) {
        case NodeStatus::FREE: return "FREE";
        case NodeStatus::USED: return "USED";
        case NodeStatus::START: return "START";
        case NodeStatus::GOAL: return "GOAL";
        default: return "UNKNOWN";
    }
}

struct Node{
    int cost = 0;
    PathConnectionType con_type = PathConnectionType::BOTH;
    NodeStatus status = NodeStatus::FREE;
    PathConnectionFrom from_type = PathConnectionFrom::FROM_LEFT;
};

class SearchField{
private:
    vector<vector<vector<Node>>> field;
    const int m_width;
    const int m_height;
    const int m_depth;
    const int INF = 1 << 30;

    Node& get(const Position3D& pos){
        return field[pos.z][pos.y][pos.x];
    }

    bool is_bounded(const Position3D& pos){
        return 0 <= pos.x && pos.x < m_width && 0 <= pos.y && pos.y < m_height && 0 <= pos.z && pos.z < m_depth;
    }

public:
    SearchField(int width, int height, int depth) : field(depth, vector<vector<Node>>(height, vector<Node>(width))), m_width(width), m_height(height), m_depth(depth){
        //cout << "search_field_size " << field.size() << " " << field[0].size() << " " << field[0][0].size() << endl;
    };

    void set_status(const Position3D& pos, NodeStatus status, PathConnectionType con_type){
        field[pos.z][pos.y][pos.x].status = status;
        field[pos.z][pos.y][pos.x].con_type = con_type;
    }

    pair<bool, vector<Position3D>> bfs(){
        queue<Position3D> bfs_queue;
        vector<Position3D> goal_list;
        for (int z = 0; z < (signed)field.size(); ++z){
            for (int y = 0; y < (signed)field[z].size(); ++y){
                for (int x = 0; x < (signed)field[z][y].size(); ++x){
                    auto status = field[z][y][x].status;
                    Position3D pos(x, y, z);
                    //cout << "(" << pos.x << "," << pos.y << "," << pos.z << "):" << NodeStatusToString(status) << std::endl;
                    if (status == NodeStatus::START){
                        //cout << "START" << "(" << pos.x << "," <<  pos.y << "," << pos.z << ")" << endl;
                        if (!bfs_queue.empty()){
                            throw runtime_error("cannot append two start nodes." + std::to_string(bfs_queue.front().x) + " " + std::to_string(bfs_queue.front().y) + " " + std::to_string(bfs_queue.front().z));
                        }
                        bfs_queue.push(pos);
                        get(pos).cost = 0;
                    }
                    else if (status == NodeStatus::GOAL){
                        //cout << "GOAL" << "(" << pos.x << "," <<  pos.y << "," << pos.z << ")" << endl;
                        goal_list.push_back(pos);
                        get(pos).cost = INF;
                    }
                    else{
                        get(pos).cost = INF;
                    }
                }
            }
        }

        while (!bfs_queue.empty()){
            auto pos = bfs_queue.front();
            bfs_queue.pop();
            const Node& node = get(pos);

            vector<pair<Position3D, PathConnectionFrom>> candidate;
            if (node.status == NodeStatus::START){ //経路はz方向から始まらない。
                if (node.con_type != PathConnectionType::HORIZONTAL){
                    candidate.push_back(make_pair(Position3D(pos.x, pos.y + 1, pos.z), PathConnectionFrom::FROM_UP));
                    candidate.push_back(make_pair(Position3D(pos.x, pos.y - 1, pos.z), PathConnectionFrom::FROM_DOWN));
                }
                if (node.con_type != PathConnectionType::VERTICAL) {
                    candidate.push_back(make_pair(Position3D(pos.x + 1, pos.y, pos.z), PathConnectionFrom::FROM_LEFT));
                    candidate.push_back(make_pair(Position3D(pos.x - 1, pos.y, pos.z), PathConnectionFrom::FROM_RIGHT));
                }
            }
            else{
                if (node.con_type == PathConnectionType::VERTICAL || node.con_type == PathConnectionType::BOTH){
                    candidate.push_back(make_pair(Position3D(pos.x, pos.y + 1, pos.z), PathConnectionFrom::FROM_UP));
                    candidate.push_back(make_pair(Position3D(pos.x, pos.y - 1, pos.z), PathConnectionFrom::FROM_DOWN));
                    candidate.push_back(make_pair(Position3D(pos.x, pos.y, (pos.z + 3) % 4), PathConnectionFrom::FROM_FRONT));
                    candidate.push_back(make_pair(Position3D(pos.x, pos.y, (pos.z + 1) % 4), PathConnectionFrom::FROM_BACK));
                }
                if (node.con_type == PathConnectionType::HORIZONTAL || node.con_type == PathConnectionType::BOTH) {
                    candidate.push_back(make_pair(Position3D(pos.x + 1, pos.y, pos.z), PathConnectionFrom::FROM_LEFT));
                    candidate.push_back(make_pair(Position3D(pos.x - 1, pos.y, pos.z), PathConnectionFrom::FROM_RIGHT));
                    candidate.push_back(make_pair(Position3D(pos.x, pos.y, (pos.z + 3) % 4), PathConnectionFrom::FROM_FRONT));
                    candidate.push_back(make_pair(Position3D(pos.x, pos.y, (pos.z + 1) % 4), PathConnectionFrom::FROM_BACK));
                }
            }
            
            for (const auto& neighbor : candidate){
                auto next_pos = neighbor.first;
                auto next_from = neighbor.second;

                if (!is_bounded(next_pos)) continue;
                Node& next = get(next_pos);

                if (next.status == NodeStatus::USED) continue;
                if (next.cost <= node.cost + 1) continue;
                if (next.status == NodeStatus::GOAL && next.con_type != PathConnectionType::BOTH){
                    if (next.con_type == PathConnectionType::HORIZONTAL && (next_from == PathConnectionFrom::FROM_UP || next_from == PathConnectionFrom::FROM_DOWN)) continue;
                    if (next.con_type == PathConnectionType::VERTICAL && (next_from == PathConnectionFrom::FROM_LEFT || next_from == PathConnectionFrom::FROM_RIGHT)) continue;

                    //　最後の平面方向の動きを見つける。
                    int x = pos.x;
                    int y = pos.y;
                    int z = pos.z;
                    PathConnectionFrom last_2D_move = node.from_type;
                    while (last_2D_move == PathConnectionFrom::FROM_FRONT || last_2D_move == PathConnectionFrom::FROM_BACK){
                        Position3D current_pos(x, y, z);
                        auto current_node = get(current_pos);
                        last_2D_move = current_node.from_type;
                        if (current_node.from_type == PathConnectionFrom::FROM_FRONT) z = (z + 1) % 4;
                        else if (current_node.from_type == PathConnectionFrom::FROM_BACK) z = (z + 3) % 4;
                    }
                    if (next.con_type == PathConnectionType::HORIZONTAL && 
                    ((next_from == PathConnectionFrom::FROM_FRONT && last_2D_move == PathConnectionFrom::FROM_UP) ||
                     (next_from == PathConnectionFrom::FROM_FRONT &&last_2D_move == PathConnectionFrom::FROM_DOWN) ||
                     (next_from == PathConnectionFrom::FROM_BACK &&last_2D_move == PathConnectionFrom::FROM_UP) ||
                     (next_from == PathConnectionFrom::FROM_BACK &&last_2D_move == PathConnectionFrom::FROM_DOWN))) continue;

                    if (next.con_type == PathConnectionType::VERTICAL && 
                    ((next_from == PathConnectionFrom::FROM_FRONT && last_2D_move == PathConnectionFrom::FROM_LEFT) ||
                     (next_from == PathConnectionFrom::FROM_FRONT &&last_2D_move == PathConnectionFrom::FROM_RIGHT) ||
                     (next_from == PathConnectionFrom::FROM_BACK &&last_2D_move == PathConnectionFrom::FROM_LEFT) ||
                     (next_from == PathConnectionFrom::FROM_BACK &&last_2D_move == PathConnectionFrom::FROM_RIGHT))) continue;
                }

                next.cost = node.cost + 1;
                next.from_type = next_from;

                if (next.status != NodeStatus::GOAL){
                    bfs_queue.push(next_pos);
                }
            }
        }

        bool result = true;
        for (const auto& pos : goal_list){
            if (get(pos).cost == INF) {
                //cout << "infinity" << endl;
                result = false;
            }
        }
        if (!result){
            vector<Position3D> dummy;
            return make_pair(false, dummy);
        }

        vector<Position3D> path;
        for (const auto& pos : goal_list){
            int x = pos.x;
            int y = pos.y;
            int z = pos.z;
            while (true){
                Position3D current_pos(x, y, z);
                auto node = get(current_pos);
                if (node.status == NodeStatus::START) break;
                if (node.status != NodeStatus::GOAL){
                    path.push_back(current_pos);
                }
                if (node.status == NodeStatus::USED){
                    throw runtime_error("invalid node reached. 1");
                }
                if (node.from_type == PathConnectionFrom::FROM_DOWN) y += 1;
                else if (node.from_type == PathConnectionFrom::FROM_UP) y -= 1;
                else if (node.from_type == PathConnectionFrom::FROM_LEFT) x -= 1;
                else if (node.from_type == PathConnectionFrom::FROM_RIGHT) x += 1;
                else if (node.from_type == PathConnectionFrom::FROM_FRONT) z = (z + 1) % 4;
                else if (node.from_type == PathConnectionFrom::FROM_BACK) z = (z + 3) % 4;

                if (!is_bounded(Position3D(x, y, z))){
                    cout << "invalid node at " << "(" << x << "," <<  y << "," << z << ")" << endl;
                    throw runtime_error("invalid node reached. 2");
                }
            }
        }

        //cout << "path_size: " << path.size() << endl;
        //for (int i = 0; i < path.size(); ++i){
            //cout << "path" << "(" << path[i].x << "," << path[i].y << "," << path[i].z << ")";
        //}
        //cout << endl;
        auto ite = unique(path.begin(), path.end());
        path.erase(ite,path.end());

        return make_pair(true, path);
    }
};