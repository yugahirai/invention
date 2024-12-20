#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>  // std::min, std::max
#include <cstdlib> 
#include <ctime>

int stepFunction(double x) {
    return (x >= 0) ? 1 : -1;
}

int g = 11;

class Node {
private:
    std::string id;
    double x, y; // 2次元座標
    bool has_edges;  // ノードがエッジを持つかどうか
    bool is_magic;

public:
    Node(const std::string& id, double x = (std::rand() %  g), double y = (std::rand() %  g), bool has_edges = false, bool is_magic = false) 
        : id(id), x(x), y(y),has_edges(has_edges), is_magic(is_magic){};

    std::string get_id() const { return id; }
    double get_x() const { return x; }
    double get_y() const { return y; }
    bool has_edges_connected() const { return has_edges; }
    void set_coordinates(double new_x, double new_y) {
        x = new_x;
        y = new_y;
    }
    void set_has_edges(bool value) {
        has_edges = value;
    }

    bool get_is_magic() const{
        return is_magic;
    }
};

class Edge {
private:
    int id;
    Node* node1;
    Node* node2;
    int weight;

public:
    Edge(int id, Node* node1, Node* node2, int weight)
        : id(id), node1(node1), node2(node2), weight(weight) {
            node1->set_has_edges(true);
            node2->set_has_edges(true);
        }

    std::string get_node1() const { return node1->get_id(); }
    std::string get_node2() const { return node2->get_id(); }
    int get_weight() const { return weight; }
    Node* get_node1_ptr() const { return node1; }
    Node* get_node2_ptr() const { return node2; }
};

class Graph {
private:
    std::unordered_map<std::string, Node*> nodes;
    std::vector<Edge> edges;

public:
    // ノードを追加
    void addNode(const std::string& id) {
        if (nodes.find(id) == nodes.end()) {
            if (id == "MAGIC_NODE"){
                nodes[id] = new Node(id,-1,0, false, true);
            }else{
            nodes[id] = new Node(id);
            }
        }
    }

    // エッジを追加
    void addEdge(int edge_id, const std::string& id1, const std::string& id2, int weight) {
        Node* node1 = getNode(id1);
        Node* node2 = getNode(id2);

        if (node1 && node2) {
            edges.emplace_back(edge_id, node1, node2, weight);
        }
    }

    // ノードを取得
    Node* getNode(const std::string& id) {
        if (nodes.find(id) != nodes.end()) {
            return nodes[id];
        }
        return nullptr;
    }

    // グラフ情報を表示
    void display() const {
        for (const auto& edge : edges) {
            std::cout << "Edge from Node " << edge.get_node1()
                      << " to Node " << edge.get_node2()
                      << " with weight " << edge.get_weight() << "\n";
        }
    }

    // 力学モデルでノードの位置を調整（エッジを持たないノードは無視）
    void layoutGraph() {
        const double epsilon = 1e-3;
        double total_movement = epsilon + 0.1;
        // 1. 力学モデルでエッジを持つノードの座標を計算
        while (total_movement > epsilon) { // 多くの反復で座標を調整
            total_movement = 0.0;
            for (const auto& edge : edges) {
                Node* u = edge.get_node1_ptr();
                Node* v = edge.get_node2_ptr();
                //std::cout << edge.get_node2() <<"(" << v->get_x() << ", " << v->get_y() << ")" << std::endl;
                //std::cout << edge.get_node1() <<"(" << u->get_x() << ", " << u->get_y() << ")" << std::endl;
                double ux = u->get_x();
                double uy = u->get_y();
                double vx = v->get_x();
                double vy = v->get_y();
                bool u_magic = u->get_is_magic();
                bool v_magic = v->get_is_magic();

                double dx = vx - ux;
                double dy = vy - uy;
                double distance = std::sqrt(dx * dx + dy * dy);

                // 重みが大きいほどノード同士を近づける
                double force = pow(edge.get_weight(),2)*pow(distance, 2)*0.000001*epsilon;
                double dp_x = force*(dx/distance);
                double dp_y = force*(dy/distance);
                double min_distance = 0.5;
                // 距離が最小距離未満の場合、修正を行う
                if (std::sqrt(pow(dx-std::abs(dp_x),2) + pow(dy-std::abs(dp_y), 2))<min_distance) {
                    dp_x = (distance-min_distance)*cos(atan(dy/dx))/2;
                    dp_y = (distance-min_distance)*sin(atan(dy/dx))/2;
                    if (u_magic){
                        v->set_coordinates(std::max(0.0, vx - 2*dp_x), std::max(0.0, vy - 2*dp_y)); 
                    } else if (v_magic){
                        u->set_coordinates(std::max(0.0, ux - 2*dp_x), std::max(0.0, uy - 2*dp_y)); 
                    }else{
                        u->set_coordinates(std::max(0.0, ux + dp_x), std::max(0.0, uy + dp_y));
                        v->set_coordinates(std::max(0.0, vx - dp_x), std::max(0.0, vy - dp_y)); 
                    }
                }else{
                    // ノードの座標を更新
                    if (u_magic){
                        v->set_coordinates(std::max(0.0, vx - 2*dp_x), std::max(0.0, vy - 2*dp_y)); 
                    } else if (v_magic){
                        u->set_coordinates(std::max(0.0, ux - 2*dp_x), std::max(0.0, uy - 2*dp_y)); 
                    }else{
                        u->set_coordinates(std::max(0.0, ux + dp_x), std::max(0.0, uy + dp_y));
                        v->set_coordinates(std::max(0.0, vx - dp_x), std::max(0.0, vy - dp_y)); 
                    }

                    double movement_u = std::sqrt(dp_x * dp_x + dp_y * dp_y);
                    double movement_v = std::sqrt(dp_x * dp_x + dp_y * dp_y);

                    total_movement += movement_u + movement_v; 
                }
            }
            std::cout << total_movement << std::endl;
        }

        // 2. 収束後にノードを最も近い2Dグリッドの点に移動（エッジを持つノード）
        snapToGrid();

        // 3. エッジを持たないノードをグリッドの外側に配置
        addNonConnectedNodes();
    }

    // エッジを持つノードを最も近いグリッド座標にスナップする
    void snapToGrid() {
        int grid_size = std::ceil(std::sqrt(nodes.size()));
        std::cout << "grid_size: " << grid_size << std::endl; 
        std::vector<std::vector<bool>> grid(grid_size, std::vector<bool>(grid_size, false));

        for (auto& pair : nodes) {
            Node* node = pair.second;
            if (!node->has_edges_connected() || node->get_is_magic()) {
                continue; // エッジを持たないノードは無視
            }

            // 近いグリッド座標を計算
            int closest_x = std::round(node->get_x());
            int closest_y = std::round(node->get_y());

            // グリッドの範囲内に収める
            closest_x = std::min(std::max(closest_x, 0), grid_size - 1);
            closest_y = std::min(std::max(closest_y, 0), grid_size - 1);

            bool found = false;
            int distance = 0;
            std::srand(static_cast<unsigned int>(std::time(0))); // 乱数の初期化
            // グリッド上の空いている場所にスナップ
            while (grid[closest_y][closest_x]&& !found) {

                // マンハッタン距離が3になる点を列挙
                std::vector<std::pair<int, int>> points;

                for (int x = -distance; x <= distance; ++x) {
                    int y = distance - std::abs(x);
                    points.push_back({x, y});
                    if (y != 0) {
                        points.push_back({x, -y});
                    }
                }
                while (!points.empty()){
                    int random_index = std::rand() % points.size(); // ランダムインデックス 
                    // ランダムに選んだ点を出力
                    std::pair<int, int> random_point = points[random_index];
                    points.erase(points.begin() + random_index);

                    int new_x = (closest_x + random_point.first)%grid_size;
                    int new_y = (closest_y + random_point.second)%grid_size;
                    //std::cout << "(" << new_x << "," << new_y << ")" << std::endl;
                    if (new_x >= 0 && new_x < grid_size && new_y >= 0 && new_y < grid_size &&
                        !grid[new_y][new_x]) {
                        closest_x = new_x;
                        closest_y = new_y;
                        found = true;  // 空き場所が見つかった
                        break;
                    }
                }
                distance++;   
                //std::cout << distance << std::endl;
            }

            // ノードをグリッド上に配置
            node->set_coordinates(closest_x, closest_y);
            grid[closest_y][closest_x] = true;
        }
    }

    // エッジを持たないノードをグリッドの外側に配置
    void addNonConnectedNodes() {
        int grid_size = std::ceil(std::sqrt(nodes.size()));
        std::vector<std::vector<bool>> grid(grid_size, std::vector<bool>(grid_size, false));

        // まずエッジを持つノードの最大のx, yを取得
        double max_x = 0, max_y = 0;
        for (auto& pair : nodes) {
            if (pair.second->has_edges_connected()) {
                max_x = std::max(max_x, pair.second->get_x());
                max_y = std::max(max_y, pair.second->get_y());
                grid[std::round(pair.second->get_y())][std::round(pair.second->get_x())] = true;
            }
        }

        // エッジを持たないノードを最大座標の外側に順次グリッド上に配置
        int outer_ring_start_x = max_x + 1;
        int outer_ring_start_y = 0;

        for (auto& pair : nodes) {
            Node* node = pair.second;
            if (node->has_edges_connected() || node->get_is_magic()) {
                continue;  // エッジを持つノードはすでに配置済み
            }

            // 空いているグリッド座標を探して配置
            while (grid[outer_ring_start_y][outer_ring_start_x]) {
                outer_ring_start_x = outer_ring_start_x + 1;
                if (outer_ring_start_x >= grid_size) {
                    outer_ring_start_x = 0;
                    outer_ring_start_y = outer_ring_start_y + 1;
                }
            }

            // グリッド上にノードを配置
            node->set_coordinates(outer_ring_start_x, outer_ring_start_y);
            grid[outer_ring_start_y][outer_ring_start_x] = true;
        }
    }    

    // グラフをGraphvizのDOT形式で出力（ノードの円の大きさを大きく、文字を小さく）
    void exportToDOT(const std::string& filename) {
        std::ofstream file(filename);
        file << "graph G {\n";
        
        // ノードの形状、サイズ、フォントサイズを統一（大きな円と小さな文字）
        file << "ratio=1;\n" << "    node [shape=circle, width=1, height=1, fixedsize=true, fontsize=7];\n";  // 文字を小さく設定

        // エッジの出力
        for (const auto& edge : edges) {
            file << "    " << edge.get_node1() << " -- " << edge.get_node2() 
                << " [label=\"" << edge.get_weight() << "\"];" << "\n";
        }

        // ノードの位置を固定し、ラベルに位置を表示して出力
        for (const auto& pair : nodes) {
            // if (!pair.second->has_edges_connected()){
            //     continue;
            // }
            double x = pair.second->get_x();
            double y = pair.second->get_y();
            std::string node_id = pair.second->get_id();

            file << "    " << node_id << " [pos=\"" << x << "," << y << "!\", "
                << "label=\"" << node_id << "\"];" << "\n";
        }

        file << "}\n";
        file.close();
    }


    // デストラクタ
    ~Graph() {
        for (auto& pair : nodes) {
            delete pair.second;
        }
    }
};
