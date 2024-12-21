#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <algorithm>
#include <cmath> // for std::sqrt, std::ceil

// ノード（qubit）のペアをカウントする関数
void countQubitPairs(const std::string& filename, 
                     std::unordered_map<std::string, int>& edgeCountMap, 
                     std::set<std::string>& nodes)
{
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
            edgeCountMap[edge]++;

            // ノードをセットに追加
            nodes.insert(qubit1);
            nodes.insert(qubit2);
        }

        // MAGIC_MOVE, MAGIC_MZZ の場合 "MAGIC_NODE-xxx" というエッジとしてカウント
        if (gate == "MAGIC_MOVE" || gate == "MAGIC_MZZ") {
            std::string edge = "MAGIC_NODE-" + qubit1;
            edgeCountMap[edge]++;
            nodes.insert("MAGIC_NODE");
        }
    }
    file.close();
}

// ノードとエッジをgraph.txtに書き出す関数
// (1) MAGIC_NODE は (-1,0,0) に固定
// (2) 残りのノードを 4層(z=0,1,2,3)に均等割りして配置
void writeGraphToFile(const std::string& filename,
                      const std::set<std::string>& nodes,
                      const std::unordered_map<std::string, int>& edgeCountMap)
{
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error: Could not open file for writing: " << filename << std::endl;
        return;
    }

    // すべてのノードをベクトル化
    std::vector<std::string> nodeList(nodes.begin(), nodes.end());
    int N = static_cast<int>(nodeList.size());

    // (A) MAGIC_NODE を取り除き、別で扱う
    bool hasMagicNode = false;
    {
        auto it = std::find(nodeList.begin(), nodeList.end(), "MAGIC_NODE");
        if (it != nodeList.end()) {
            hasMagicNode = true;
            nodeList.erase(it);  // レイヤリングから除外
        }
    }

    // Node出力開始
    file << "Node\n";

    // (B) 残りノード数を4層に均等割り
    N = static_cast<int>(nodeList.size());
    int base = N / 4;         // 各層に最低限入る数
    int remainder = N % 4;    // 余ったものを上から順に足していく
    int index = 0;            // nodeList 上の走査位置

    for (int layer = 0; layer < 4; ++layer) {
        // この層に割り当てるノード数
        int layerSize = base + ((layer < remainder) ? 1 : 0);

        // layerSize個のノードを置くための 2D グリッドサイズ Dk
        int Dk = static_cast<int>(std::ceil(std::sqrt(layerSize)));

        for (int i = 0; i < layerSize; ++i) {
            if (index >= N) break; // 念のため

            int x = i % Dk;
            int y = i / Dk;
            // ノード名, x, y, z
            file << nodeList[index] << " " << x << " " << y << " " << layer << "\n";

            index++;
        }
    }

    // (C) MAGIC_NODEを (-1,0,0) に固定配置
    if (hasMagicNode) {
        file << "MAGIC_NODE -1 0 0\n";
    }

    // 空行を入れて edge セクションへ
    file << "\nedge\n";
    int edgeId = 1;
    for (const auto& kv : edgeCountMap) {
        std::string edge = kv.first;
        int weight = kv.second;

        // '-' を空白に置き換える
        std::replace(edge.begin(), edge.end(), '-', ' ');

        // edgeId, node1, node2, weight
        file << edgeId << " " << edge << " " << weight << "\n";
        edgeId++;
    }

    file.close();
    std::cout << "Graph data saved to " << filename 
              << " (MAGIC_NODE at (-1,0,0), others in z=[0..3])" << std::endl;
}

int main() {
    std::unordered_map<std::string, int> edgeCountMap;
    std::set<std::string> nodes;

    // ファイルから Qubit のペアをカウント
    countQubitPairs("circuit/result_SELECT_6_Heisenberg2D_cylinder_0.5_0.5_6.raw",
                    edgeCountMap, nodes);

    // 各層のレイヤリング & MAGIC_NODE 固定
    writeGraphToFile("graph.txt", nodes, edgeCountMap);

    return 0;
}
