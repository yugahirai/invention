#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>  // for rand() and srand()
#include <ctime>    // for time()

void generateRandomGraph(const std::string& filename, int numNodes, int numEdges) {
    std::ofstream file(filename);

    if (!file) {
        std::cerr << "Error: Could not open file for writing: " << filename << std::endl;
        return;
    }

    // ランダムシードを設定
    std::srand(std::time(0));

    // ノードの出力
    file << "Node\n";
    for (int i = 1; i <= numNodes; ++i) {
        file << i << "\n";
    }

    // エッジの出力
    file << "\nedge\n";
    for (int i = 1; i <= numEdges; ++i) {
        int node1 = std::rand() % numNodes + 1;  // 1からnumNodesまでのランダムなノードID
        int node2 = std::rand() % numNodes + 1;
        int weight = std::rand() % 10 + 1;       // 1から10までのランダムな重み

        // 同じノード間にエッジを作らない
        if (node1 != node2) {
            file << i << " " << node1 << " " << node2 << " " << weight << "\n";
        } else {
            --i;  // 同じノードが選ばれた場合はやり直し
        }
    }

    file.close();
    std::cout << "Random graph generated and saved to " << filename << std::endl;
}

int main() {
    // ノード数とエッジ数を指定
    int numNodes; // ノードの数
    int numEdges; // エッジの数

    std::cout << "Enter the number of nodes: ";
    std::cin >> numNodes;
    std::cout << "Enter the number of edges: ";
    std::cin >> numEdges;

    // ランダムなグラフを生成してgraph.txtに保存
    generateRandomGraph("graph.txt", numNodes, numEdges);

    return 0;
}