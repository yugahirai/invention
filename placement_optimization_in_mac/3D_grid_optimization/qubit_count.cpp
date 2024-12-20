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

        if (gate == "MAGIC_MOVE" || gate == "MAGIC_MZZ") {
            std::string edge = "MAGIC_NODE-" + qubit1;

            // ペアの出現回数をカウント
            edgeCountMap[edge]++;

            // ノードをセットに追加
            nodes.insert("MAGIC_NODE");
        }
    }

    file.close();
}

// ノードとエッジをgraph.txtに書き出す関数（zが0～3に収まる3次元座標対応）
void writeGraphToFile(const std::string& filename, const std::set<std::string>& nodes, const std::unordered_map<std::string, int>& edgeCountMap) {
    std::ofstream file(filename);

    if (!file) {
        std::cerr << "Error: Could not open file for writing: " << filename << std::endl;
        return;
    }

    // ノードベクトル化
    std::vector<std::string> nodeList(nodes.begin(), nodes.end());
    int N = (int)nodeList.size();

    // z座標0～3(計4層)に収まるようにする
    // 4層でNノードを配置するには、各層あたり約N/4個。
    // 各層D×Dノード配置するとして 4*D^2 >= N を満たすDを求める。
    // D >= sqrt(N/4) → D = ceil(sqrt((N+3)/4.0)) としておく（+3は余裕分）
    int D = (int)std::ceil(std::sqrt((N + 3) / 4.0));

    // ノードの出力 ("Node"ヘッダの後に x,y,z座標も出力)
    file << "Node\n";
    for (int i = 0; i < N; i++) {
        int z = i / (D * D);
        int layerIndex = i % (D * D);
        int y = (layerIndex / D);
        int x = (layerIndex % D);

        // zが0～3の範囲に収まっているかチェック（理論上満たされるはず）
        // if (z > 3) ... // 必要ならエラーチェック

        file << nodeList[i] << " " << x << " " << y << " " << z << "\n";
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
    std::cout << "Graph data with 3D coordinates (z in [0,3]) saved to " << filename << std::endl;
}

int main() {
    std::unordered_map<std::string, int> edgeCountMap;
    std::set<std::string> nodes;

    // ファイルからqubitのペアをカウント
    countQubitPairs("circuit/result_SELECT_7_Heisenberg2D_cylinder_0.5_0.5_3.raw", edgeCountMap, nodes);

    // カウントされたノードとエッジをgraph.txtに書き出し（z=0~3に抑える）
    writeGraphToFile("graph.txt", nodes, edgeCountMap);

    return 0;
}
