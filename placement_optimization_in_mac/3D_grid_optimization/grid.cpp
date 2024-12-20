#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>
#include <fstream>
#include <cstdio>
#include <sstream>
#include <map>
#include <cmath>

const int MAX_ITERATIONS = 1000000;   // 最大反復回数
const int WEIGHT_THRESHOLD = 0;      // エッジの重みの閾値
const int MIN_IMPROVEMENT = 1;        // これ以下の改善量は「わずか」とみなす
const int MAX_NO_IMPROVEMENT = 10000;  // わずかな改善がこの回数続けば打ち切り

struct Qubit {
    int id;
    int x, y, z;  
    int totalEdgeWeight;
};

struct Edge {
    int qubit1, qubit2;
    int weight; 
};

int calculateEnergy(const std::vector<Qubit>& qubits, const std::vector<Edge>& edges) {
    int energy = 0;
    for (const auto& edge : edges) {
        if (edge.weight <= WEIGHT_THRESHOLD) continue;
        const Qubit& q1 = qubits[edge.qubit1];
        const Qubit& q2 = qubits[edge.qubit2];
        int distance = std::abs(q1.x - q2.x) + std::abs(q1.y - q2.y) + std::abs(q1.z - q2.z);
        energy += (int)(std::pow(edge.weight, 1) * std::pow(distance, 2));
    }
    return energy;
}

void outputFinalGraph(const std::string& filename, const std::vector<Qubit>& qubits, const std::vector<Edge>& edges, const std::vector<std::string>& nodeIDToName) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << filename << " for writing." << std::endl;
        return;
    }

    file << "Node\n";
    for (const auto& q : qubits) {
        file << nodeIDToName[q.id] << " " << q.x << " " << q.y << " " << q.z << "\n";
    }

    file << "\nedge\n";
    int edgeID = 1;
    for (const auto& e : edges) {
        if (e.weight <= WEIGHT_THRESHOLD) continue;
        file << edgeID << " " << nodeIDToName[e.qubit1] << " " << nodeIDToName[e.qubit2] << " " << e.weight << "\n";
        edgeID++;
    }

    file.close();
    std::cout << "Final graph data saved to " << filename << std::endl;
}

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::ifstream graphFile("graph.txt");
    if (!graphFile) {
        std::cerr << "Failed to open graph.txt" << std::endl;
        return 1;
    }

    std::vector<Qubit> qubits;
    std::vector<Edge> edges;
    std::map<std::string, int> nodeNameToID;
    std::vector<std::string> nodeIDToName;

    bool readingNodes = false;
    bool readingEdges = false;
    int currentNodeID = 0;

    std::string line;
    while (std::getline(graphFile, line)) {
        if (line == "Node") {
            readingNodes = true;
            readingEdges = false;
            continue;
        } else if (line == "edge") {
            readingNodes = false;
            readingEdges = true;
            continue;
        }

        if (readingNodes && !line.empty()) {
            std::istringstream iss(line);
            std::string nodeName;
            int x, y, z;
            if (!(iss >> nodeName >> x >> y >> z)) {
                continue;
            }

            if (nodeNameToID.find(nodeName) == nodeNameToID.end()) {
                nodeNameToID[nodeName] = currentNodeID++;
                nodeIDToName.push_back(nodeName);
            }
            int nodeID = nodeNameToID[nodeName];

            Qubit q;
            q.id = nodeID;
            q.x = x;
            q.y = y;
            q.z = z;
            q.totalEdgeWeight = 0;
            qubits.push_back(q);

        } else if (readingEdges && !line.empty()) {
            std::istringstream iss(line);
            std::string edgeIDStr, node1Name, node2Name;
            int weight;
            if (!(iss >> edgeIDStr >> node1Name >> node2Name >> weight)) {
                continue;
            }

            if (nodeNameToID.find(node1Name) == nodeNameToID.end()) {
                nodeNameToID[node1Name] = currentNodeID++;
                nodeIDToName.push_back(node1Name);
                Qubit q;
                q.id = nodeNameToID[node1Name];
                q.x = 0; q.y = 0; q.z = 0; 
                q.totalEdgeWeight = 0;
                qubits.push_back(q);
            }

            if (nodeNameToID.find(node2Name) == nodeNameToID.end()) {
                nodeNameToID[node2Name] = currentNodeID++;
                nodeIDToName.push_back(node2Name);
                Qubit q;
                q.id = nodeNameToID[node2Name];
                q.x = 0; q.y = 0; q.z = 0; 
                q.totalEdgeWeight = 0;
                qubits.push_back(q);
            }

            int node1ID = nodeNameToID[node1Name];
            int node2ID = nodeNameToID[node2Name];

            Edge e;
            e.qubit1 = node1ID;
            e.qubit2 = node2ID;
            e.weight = weight;
            edges.push_back(e);
        }
    }

    graphFile.close();

    {
        std::vector<Qubit> sortedQubits(currentNodeID);
        for (auto &q : qubits) {
            if (q.id < (int)sortedQubits.size()) {
                sortedQubits[q.id] = q;
            }
        }
        qubits = std::move(sortedQubits);
    }

    int qubitCount = (int)qubits.size();

    int magicNodeID = -1;
    if (nodeNameToID.find("MAGIC_NODE") != nodeNameToID.end()) {
        magicNodeID = nodeNameToID["MAGIC_NODE"];
        qubits[magicNodeID].x = -1;
        qubits[magicNodeID].y = 0;
        qubits[magicNodeID].z = 0;
    } else {
        std::cerr << "MAGIC_NODEが見つかりませんでした。" << std::endl;
        return 1;
    }

    for (const auto& edge : edges) {
        if (edge.weight <= WEIGHT_THRESHOLD) continue;
        qubits[edge.qubit1].totalEdgeWeight += edge.weight;
        qubits[edge.qubit2].totalEdgeWeight += edge.weight;
    }

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

    int minX = qubits[0].x, maxX = qubits[0].x;
    int minY = qubits[0].y, maxY = qubits[0].y;
    int minZ = qubits[0].z, maxZ = qubits[0].z;
    for (auto &q : qubits) {
        minX = std::min(minX, q.x);
        maxX = std::max(maxX, q.x);
        minY = std::min(minY, q.y);
        maxY = std::max(maxY, q.y);
        minZ = std::min(minZ, q.z);
        maxZ = std::max(maxZ, q.z);
    }

    // z座標は0から3までしか取れないようにするため、近傍探索時に制限をかける
    int range = 20;
    int qubitIndexPosition = 0;

    int small_improvement_count = 0;
    for (int iteration = 0; iteration < MAX_ITERATIONS; ++iteration) {
        int idx1 = qubitIndices[qubitIndexPosition];
        qubitIndexPosition = (qubitIndexPosition + 1) % qubitIndices.size();

        int x1 = qubits[idx1].x;
        int y1 = qubits[idx1].y;
        int z1 = qubits[idx1].z;

        std::vector<std::tuple<int,int,int>> neighbor_positions;
        for (int dx = -range; dx <= range; ++dx) {
            for (int dy = -range; dy <= range; ++dy) {
                for (int dz = -range; dz <= range; ++dz) {
                    if (dx == 0 && dy == 0 && dz == 0) continue;
                    if (std::abs(dx) + std::abs(dy) + std::abs(dz) > range) continue;

                    int new_x = x1 + dx;
                    int new_y = y1 + dy;
                    int new_z = z1 + dz;

                    // zが0〜3以外は無視
                    if (new_z < 0 || new_z > 3) continue;

                    if (new_x >= minX && new_x <= maxX &&
                        new_y >= minY && new_y <= maxY &&
                        new_z >= 0 && new_z <= 3) {
                        neighbor_positions.emplace_back(new_x, new_y, new_z);
                    }
                }
            }
        }

        if (neighbor_positions.empty()) continue;

        std::uniform_int_distribution<> neighbor_dist(0, (int)neighbor_positions.size() - 1);
        auto [x2, y2, z2] = neighbor_positions[neighbor_dist(gen)];

        int idx2 = -1;
        for (int i = 0; i < qubitCount; ++i) {
            if (qubits[i].x == x2 && qubits[i].y == y2 && qubits[i].z == z2) {
                idx2 = i;
                break;
            }
        }

        if (idx2 == -1) continue;
        if (idx1 == magicNodeID || idx2 == magicNodeID) continue;

        // スワップ
        std::swap(qubits[idx1].x, qubits[idx2].x);
        std::swap(qubits[idx1].y, qubits[idx2].y);
        std::swap(qubits[idx1].z, qubits[idx2].z);

        int newEnergy = calculateEnergy(qubits, edges);
        int improvement = currentEnergy - newEnergy;

        if (newEnergy < currentEnergy) {
            currentEnergy = newEnergy;
            if (improvement < MIN_IMPROVEMENT) {
                small_improvement_count++;
            } else {
                small_improvement_count = 0;
            }
            std::cout << currentEnergy << std::endl;
        } else {
            // 戻す
            std::swap(qubits[idx1].x, qubits[idx2].x);
            std::swap(qubits[idx1].y, qubits[idx2].y);
            std::swap(qubits[idx1].z, qubits[idx2].z);
            small_improvement_count++;
        }

        if (small_improvement_count > MAX_NO_IMPROVEMENT) {
            std::cout << "十分小さい更新量が続いたため、早期終了します。" << std::endl;
            break;
        }
    }

    std::cout << "最終エネルギー: " << currentEnergy << std::endl;
    outputFinalGraph("final_graph.txt", qubits, edges, nodeIDToName);

    return 0;
}
