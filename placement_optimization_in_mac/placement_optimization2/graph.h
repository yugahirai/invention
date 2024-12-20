#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>  // std::min, std::max

class Node {
private:
    std::string id;
    double x, y; // 2次元座標
    bool has_edges;  // ノードがエッジを持つかどうか

public:
    Node(const std::string& id, double x = 0, double y = 0, bool has_edges = false) 
        : id(id), x(x), y(y), has_edges(has_edges) {}
    
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
            nodes[id] = new Node(id);
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
        // 1. 力学モデルでエッジを持つノードの座標を計算
        for (int iter = 0; iter < 1000; ++iter) { // 多くの反復で座標を調整
            for (const auto& edge : edges) {
                Node* u = edge.get_node1_ptr();
                Node* v = edge.get_node2_ptr();

                double dx = v->get_x() - u->get_x();
                double dy = v->get_y() - u->get_y();
                double distance = std::sqrt(dx * dx + dy * dy);

                // 重みが大きいほどノード同士を近づける
                double force = (edge.get_weight()*0.000001 - distance) * 0.001;
                double ux = force * (dx / distance);
                double uy = force * (dy / distance);

                // ノードの座標を更新
                u->set_coordinates(u->get_x() + ux, u->get_y() + uy);
                v->set_coordinates(v->get_x() - ux, v->get_y() - uy);
            }
        }

        // 2. 収束後にノードを最も近い2Dグリッドの点に移動（エッジを持つノード）
        snapToGrid();

        // 3. エッジを持たないノードをグリッドの外側に配置
        addNonConnectedNodes();
    }

    // エッジを持つノードを最も近いグリッド座標にスナップする
    void snapToGrid() {
        int grid_size = std::ceil(std::sqrt(nodes.size()));
        std::vector<std::vector<bool>> grid(grid_size, std::vector<bool>(grid_size, false));

        for (auto& pair : nodes) {
            Node* node = pair.second;
            if (!node->has_edges_connected()) {
                continue; // エッジを持たないノードは無視
            }

            // 近いグリッド座標を計算
            int closest_x = std::round(node->get_x());
            int closest_y = std::round(node->get_y());

            // グリッドの範囲内に収める
            closest_x = std::min(std::max(closest_x, 0), grid_size - 1);
            closest_y = std::min(std::max(closest_y, 0), grid_size - 1);

            // グリッド上の空いている場所にスナップ
            while (grid[closest_y][closest_x]) {
                closest_x = (closest_x + 1) % grid_size;
                if (closest_x == 0) {
                    closest_y = (closest_y + 1) % grid_size;
                }
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
            if (node->has_edges_connected()) {
                continue;  // エッジを持つノードはすでに配置済み
            }

            // 空いているグリッド座標を探して配置
            while (grid[outer_ring_start_y][outer_ring_start_x]) {
                outer_ring_start_x++;
                if (outer_ring_start_x >= grid_size) {
                    outer_ring_start_x = 0;
                    outer_ring_start_y++;
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
        file << "    node [shape=circle, width=1.0, height=1.0, fixedsize=true, fontsize=5];\n";  // 文字を小さく設定

        // エッジの出力
        for (const auto& edge : edges) {
            file << "    " << edge.get_node1() << " -- " << edge.get_node2() 
                 << " [label=\"" << edge.get_weight() << "\"];" << "\n";
        }

        // ノードの位置を固定して出力
        for (const auto& pair : nodes) {
            file << "    " << pair.second->get_id() << " [pos=\"" << pair.second->get_x()
                 << "," << pair.second->get_y() << "!\"];" << "\n";
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
