#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <string>

class Node {
private:
    std::string id;

public:
    Node(const std::string& id) : id(id) {}
    std::string get_id() const {
        return id;
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
        : id(id), node1(node1), node2(node2), weight(weight) {}

    std::string get_node1() const {
        return node1->get_id();
    }

    std::string get_node2() const {
        return node2->get_id();
    }

    int get_weight() const {
        return weight;
    }
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

    // グラフをGraphvizのDOT形式で出力
    void exportToDOT(const std::string& filename) {
        std::ofstream file(filename);
        file << "graph G {\n";
        for (const auto& edge : edges) {
            file << "    " << edge.get_node1() << " -- "
                 << edge.get_node2() << " [label=\"" << edge.get_weight() << "\"];\n";
        }
        file << "}\n";
        file.close();
    }

    ~Graph() {
        for (auto& pair : nodes) {
            delete pair.second;
        }
    }
};
