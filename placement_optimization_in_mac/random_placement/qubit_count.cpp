#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <algorithm>  // for std::replace

// ノード（qubit）のペアをカウントする関数
void countQubitPairs(const std::string& filename, std::unordered_map<std::string, int>& edgeCountMap, std::set<std::string>& nodes) {
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string gate, qubit1, qubit2;

        // 1行からゲート情報を解析（"CX qubit1 qubit2" 形式）
        iss >> gate >> qubit1 >> qubit2;

        if (gate == "CX") {
            // ペアを標準化して順序を統一
            std::string edge = qubit1 + "-" + qubit2;

            // ペアの出現回数をカウント
            edgeCountMap[edge]++;

            // ノードをセットに追加
            nodes.insert(qubit1);
            nodes.insert(qubit2);
        }
        if (gate == "MAGIC_MOVE" || gate == "MAGIC_MZZ"){
            std::string edge = "MAGIC_NODE-" + qubit1;

            // ペアの出現回数をカウント
            edgeCountMap[edge]++;

            // ノードをセットに追加
            nodes.insert("MAGIC_NODE");
        }
    }

    file.close();
}

// ノードとエッジをgraph.txtに書き出す関数
void writeGraphToFile(const std::string& filename, const std::set<std::string>& nodes, const std::unordered_map<std::string, int>& edgeCountMap) {
    std::ofstream file(filename);

    if (!file) {
        std::cerr << "Error: Could not open file for writing: " << filename << std::endl;
        return;
    }

    // ノードの出力
    file << "Node\n";
    for (const auto& node : nodes) {
        file << node << "\n";
    }

    // エッジの出力
    file << "\nedge\n";
    int edgeId = 1;
    for (const auto& pair : edgeCountMap) {
        std::string edge = pair.first;

        // エッジの "-" をスペースに置き換える
        std::replace(edge.begin(), edge.end(), '-', ' ');

        // ファイルにエッジを書き出す
        file << edgeId << " " << edge << " " << pair.second << "\n";
        edgeId++;
    }

    file.close();
    std::cout << "Graph data saved to " << filename << std::endl;
}

int main() {
    std::unordered_map<std::string, int> edgeCountMap;
    std::set<std::string> nodes;

    // ファイルからqubitのペアをカウント
    countQubitPairs("circuit/result_SELECT_10_FermiHubbard2D_cylinder_0_0_4.raw", edgeCountMap, nodes);

    // カウントされたノードとエッジをgraph.txtに書き出し
    writeGraphToFile("graph.txt", nodes, edgeCountMap);

    return 0;
}
