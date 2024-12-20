#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

int main() {
    std::ifstream infile("final_graph.txt");
    std::ofstream outfile("placement.txt");

    if (!infile.is_open()) {
        std::cerr << "Failed to open final_graph.txt" << std::endl;
        return 1;
    }

    if (!outfile.is_open()) {
        std::cerr << "Failed to create placement.txt" << std::endl;
        return 1;
    }

    std::string line;
    bool readingNodes = false;

    while (std::getline(infile, line)) {
        // "Node"行が来たらノードセクション開始
        if (line == "Node") {
            readingNodes = true;
            continue;
        }

        // "edge"行または空行が来たらノードセクション終了
        if (line == "edge" || (readingNodes && line.empty())) {
            readingNodes = false;
        }

        // ノードセクション内の行を処理
        if (readingNodes && !line.empty()) {
            std::istringstream iss(line);
            std::string node_name;
            int x, y, z;
            if (!(iss >> node_name >> x >> y >> z)) {
                continue; // パース失敗時スキップ
            }

            if (node_name == "MAGIC_NODE" && x == -1) {
                // MAGIC_NODEの場合は20個配置。座標は(0,i,0)固定。
                for (int i = 0; i < 20; ++i) {
                    outfile << "MAGIC magic" << i << " " << 0 << " " << i << " " << 0 << std::endl;
                }
            } else {
                // 通常ノードの場合、xとyを指定の式で変換
                int new_x = 3 + x * 3 / 2;  // x座標を調整
                int new_y = 1 + y * 3 / 2;  // y座標を調整
                // zはそのまま
                int new_z = z;

                outfile << "ALLOCATE " << node_name << " " << new_x << " " << new_y << " " << new_z << std::endl;
            }
        }
    }

    infile.close();
    outfile.close();

    std::cout << "placement.txt has been created successfully." << std::endl;
    return 0;
}
