#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>
#include <fstream>
#include <cstdio> // system関数を使用するため
#include <sstream>
#include <map>
#include <cmath> // std::ceil, std::sqrt

const int MAX_ITERATIONS = 1000000;   // 最大反復回数
const int SCALE = 100;                 // スケーリングファクター
const int WEIGHT_THRESHOLD = 0;        // エッジの重みの閾値（n）

struct Qubit {
    int id;                    // 量子ビットの識別子
    int x, y;                  // グリッド上の位置
    int totalEdgeWeight;       // ノードが持つエッジの重みの総和
};

struct Edge {
    int qubit1, qubit2;
    int weight; // ゲートの回数を重みとする
};

int calculateEnergy(const std::vector<Qubit>& qubits, const std::vector<Edge>& edges) {
    int energy = 0;
    for (const auto& edge : edges) {
        if (edge.weight <= WEIGHT_THRESHOLD) continue; // 閾値以下のエッジを無視

        const Qubit& q1 = qubits[edge.qubit1];
        const Qubit& q2 = qubits[edge.qubit2];
        int distance = abs(q1.x - q2.x) + abs(q1.y - q2.y);
        energy += pow(edge.weight, 1) * pow(distance, 2);
    }
    return energy;
}

void outputGraphviz(const std::string& filename, const std::vector<Qubit>& qubits, const std::vector<Edge>& edges, const std::vector<std::string>& nodeIDToName) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << filename << " for writing." << std::endl;
        return;
    }

    file << "graph G {\n";
    file << "  node [shape=circle, width=1.4, height=1.4, fixedsize=true, fontsize=10];\n";
    file << "  graph [splines=true, overlap=false];\n";
    file << "  edge [penwidth=0.5];\n";

    // ノードの位置を設定（ノード名を識別子として使用）
    for (const auto& qubit : qubits) {
        file << "  " << nodeIDToName[qubit.id] << " [pos=\"" << qubit.x * SCALE << "," << qubit.y * SCALE << "!\", label=\"" << nodeIDToName[qubit.id] << "\"];\n";
    }

    // エッジを追加（ノード名を識別子として使用）
    for (const auto& edge : edges) {
        if (edge.weight <= WEIGHT_THRESHOLD) continue; // 閾値以下のエッジを無視
        file << "  " << nodeIDToName[edge.qubit1] << " -- " << nodeIDToName[edge.qubit2] << " [label=\"" << edge.weight << "\", fontsize=8];\n";
    }

    file << "}\n";
    file.close();
}

void generatePDF(const std::string& dotFilename, const std::string& pdfFilename) {
    std::string command = "neato -n2 -Tpdf \"" + dotFilename + "\" -o \"" + pdfFilename + "\"";
    int result = system(command.c_str());
    if (result != 0) {
        std::cerr << "コマンドの実行に失敗しました: " << command << std::endl;
    }
}

int main() {
    // ランダム数生成器の設定
    std::random_device rd;
    std::mt19937 gen(rd());

    // グラフを読み込む
    std::ifstream graphFile("graph.txt");
    if (!graphFile) {
        std::cerr << "Failed to open graph.txt" << std::endl;
        return 1;
    }

    std::map<std::string, int> nodeNameToID;
    int currentNodeID = 0;

    std::vector<Edge> edges;

    std::string line;
    while (std::getline(graphFile, line)) {
        std::istringstream iss(line);
        std::vector<std::string> tokens;
        std::string token;

        // トークンを全て読み込む
        while (iss >> token) {
            tokens.push_back(token);
        }

        std::string node1Name, node2Name;
        int weight;

        if (tokens.size() == 3) {
            // フォーマット: ノード名1 ノード名2 重み
            node1Name = tokens[0];
            node2Name = tokens[1];
            weight = std::stoi(tokens[2]);
        } else if (tokens.size() == 4) {
            // フォーマット: エッジID ノード名1 ノード名2 重み
            node1Name = tokens[1];
            node2Name = tokens[2];
            weight = std::stoi(tokens[3]);
        } else {
            // 無効な行をスキップ
            continue;
        }

        // ノード名をIDにマッピング
        if (nodeNameToID.find(node1Name) == nodeNameToID.end()) {
            nodeNameToID[node1Name] = currentNodeID++;
        }
        if (nodeNameToID.find(node2Name) == nodeNameToID.end()) {
            nodeNameToID[node2Name] = currentNodeID++;
        }

        int node1ID = nodeNameToID[node1Name];
        int node2ID = nodeNameToID[node2Name];

        Edge e;
        e.qubit1 = node1ID;
        e.qubit2 = node2ID;
        e.weight = weight;
        edges.push_back(e);
    }

    graphFile.close();

    // ノードIDから名前へのマッピングを作成
    int qubitCount = currentNodeID;
    std::vector<std::string> nodeIDToName(qubitCount);
    for (const auto& pair : nodeNameToID) {
        nodeIDToName[pair.second] = pair.first;
    }

    // 必要なグリッドサイズを計算
    int gridSize = static_cast<int>(std::ceil(std::sqrt(qubitCount)));

    // グリッドサイズを定数として設定
    const int GRID_SIZE = gridSize;

    // 量子ビットの初期化
    std::vector<Qubit> qubits;
    for (int i = 0; i < qubitCount; ++i) {
        Qubit q;
        q.id = i;
        q.x = i % GRID_SIZE;
        q.y = i / GRID_SIZE;
        q.totalEdgeWeight = 0;
        qubits.push_back(q);
    }

    // MAGIC_NODEを特定し、位置を固定
    int magicNodeID = -1;
    if (nodeNameToID.find("MAGIC_NODE") != nodeNameToID.end()) {
        magicNodeID = nodeNameToID["MAGIC_NODE"];
        qubits[magicNodeID].x = -1;
        qubits[magicNodeID].y = 0;
    } else {
        std::cerr << "MAGIC_NODEが見つかりませんでした。" << std::endl;
        return 1;
    }

    // 各量子ビットのtotalEdgeWeightを計算
    for (const auto& edge : edges) {
        if (edge.weight <= WEIGHT_THRESHOLD) continue; // 閾値以下のエッジを無視
        qubits[edge.qubit1].totalEdgeWeight += edge.weight;
        qubits[edge.qubit2].totalEdgeWeight += edge.weight;
    }

    // totalEdgeWeightが大きい順に量子ビットのインデックスを並べる（MAGIC_NODEは除外）
    std::vector<int> qubitIndices;
    for (int i = 0; i < qubitCount; ++i) {
        if (i != magicNodeID) {
            qubitIndices.push_back(i);
        }
    }

    std::sort(qubitIndices.begin(), qubitIndices.end(), [&qubits](int a, int b) {
        return qubits[a].totalEdgeWeight > qubits[b].totalEdgeWeight;
    });

    int currentEnergy = calculateEnergy(qubits, edges);
    std::cout << "初期エネルギー: " << currentEnergy << std::endl;

    // 初期状態をGraphvizで出力
    outputGraphviz("initial_state.dot", qubits, edges, nodeIDToName);
    // PDFを生成
    generatePDF("initial_state.dot", "initial_state.pdf");

    // 反復による最適化
    int qubitIndexPosition = 0;
    for (int iteration = 0; iteration < MAX_ITERATIONS; ++iteration) {
        // totalEdgeWeightが大きい順に量子ビットを選択
        int idx1 = qubitIndices[qubitIndexPosition];
        qubitIndexPosition = (qubitIndexPosition + 1) % qubitIndices.size();

        int x1 = qubits[idx1].x;
        int y1 = qubits[idx1].y;

        // idx1の隣接する位置を取得（マンハッタン距離1から3）
        std::vector<std::pair<int, int>> neighbor_positions;

        for (int dx = -20; dx <= 20; ++dx) {
            for (int dy = -20; dy <= 20; ++dy) {
                if (abs(dx) + abs(dy) > 20 || (dx == 0 && dy == 0)) continue; // マンハッタン距離が3を超える、または自身をスキップ
                int new_x = x1 + dx;
                int new_y = y1 + dy;
                // グリッド内に収まるか確認
                if (new_x >= 0 && new_x < GRID_SIZE && new_y >= 0 && new_y < GRID_SIZE) {
                    neighbor_positions.emplace_back(new_x, new_y);
                }
            }
        }

        if (neighbor_positions.empty()) continue;

        // 隣接する位置からランダムに1つを選択
        std::uniform_int_distribution<> neighbor_dist(0, neighbor_positions.size() - 1);
        auto [x2, y2] = neighbor_positions[neighbor_dist(gen)];

        // その位置にある量子ビットを見つける
        int idx2 = -1;
        for (int i = 0; i < qubitCount; ++i) {
            if (qubits[i].x == x2 && qubits[i].y == y2) {
                idx2 = i;
                break;
            }
        }

        if (idx2 == -1) continue; // 念のため

        // MAGIC_NODEとの交換を禁止
        if (idx1 == magicNodeID || idx2 == magicNodeID) {
            continue;
        }

        // 交換後の位置をシミュレーション
        std::swap(qubits[idx1].x, qubits[idx2].x);
        std::swap(qubits[idx1].y, qubits[idx2].y);

        int newEnergy = calculateEnergy(qubits, edges);
        // エネルギーを表示する場合は以下を有効にしてください
        // std::cout << newEnergy << std::endl;

        if (newEnergy < currentEnergy) {
            // エネルギーが下がった場合、交換を維持
            currentEnergy = newEnergy;
            std::cout << currentEnergy << std::endl;
            // 進捗を出力する場合は以下を有効にしてください
            // std::cout << "Iteration " << iteration << ": エネルギーが " << currentEnergy << " に下がりました。" << std::endl;
        } else {
            // エネルギーが下がらない場合、交換を元に戻す
            std::swap(qubits[idx1].x, qubits[idx2].x);
            std::swap(qubits[idx1].y, qubits[idx2].y);
        }
    }

    std::cout << "最終エネルギー: " << currentEnergy << std::endl;

    // 最終状態をGraphvizで出力
    outputGraphviz("final_state.dot", qubits, edges, nodeIDToName);
    // PDFを生成
    generatePDF("final_state.dot", "final_state.pdf");

    return 0;
}