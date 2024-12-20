#include <iostream>
#include <fstream>
#include <regex>
#include <string>

int main() {
    std::ifstream infile("final_state.dot");
    std::ofstream outfile("placement.txt");

    if (!infile.is_open()) {
        std::cerr << "Failed to open graph.dot" << std::endl;
        return 1;
    }
    
    if (!outfile.is_open()) {
        std::cerr << "Failed to create placement.txt" << std::endl;
        return 1;
    }

    std::string line;
    // MAGIC_NODEの特殊な処理用の正規表現 (label部分を緩める)
    std::regex node_regex(R"((\w+)\s+\[pos="(-?\d+),(\d+)!"\s*,?\s*label="[^"]+"\])");
    std::smatch match;

    while (std::getline(infile, line)) {
        std::cout << "Processing line: " << line << std::endl; // デバッグメッセージ
        if (std::regex_search(line, match, node_regex)) {
            std::string node_name = match[1].str();
            int x_pos = std::stoi(match[2].str())/100;
            int y_pos = std::stoi(match[3].str())/100;

            std::cout << "Matched node: " << node_name << ", x: " << x_pos << ", y: " << y_pos << std::endl; // デバッグメッセージ

            // MAGIC_NODEの検出: x座標が-1の場合に特別な処理
            if (node_name == "MAGIC_NODE" && x_pos == -1) {
                for (int i = 0; i < 28; ++i) { // MAGIC_NODEを28個生成
                    std::cout << "Writing MAGIC node: magic" << i << std::endl; // デバッグメッセージ
                    outfile << "MAGIC magic" << i << " " << 0 << " " << i << std::endl;
                }
            } else {
                // 通常のノードを処理
                std::cout << "Writing ALLOCATE for node: " << node_name << std::endl; // デバッグメッセージ
                int y_mod = y_pos % 6;
                if (y_mod == 5){
                    y_pos = y_pos + y_pos/6 + 1;
                    y_mod = y_pos % 6;
                }
                if (y_mod == 0){
                    x_pos = (x_pos+1)*4/3 + 2*(x_pos/3) + 3;
                }
                if (y_mod == 1){
                    x_pos =  (x_pos)*4/3 + 2*((x_pos + 1)/3) + 3;
                }
                if (y_mod == 2){
                    x_pos = (x_pos +2)*4/3 + 2 + 2*((x_pos + 3)/3) - 6 + 3;
                }
                if (y_mod == 3){
                    x_pos = x_pos + 4*(x_pos/2) + 2 + 3;
                }
                if (y_mod == 4){
                    x_pos =  (x_pos)*4/3 + 2*((x_pos + 1)/3) + 3;
                }
                y_pos = y_pos + 1;
                outfile << "ALLOCATE " << node_name << " " << x_pos << " " << y_pos << std::endl;
            }
        } else {
            std::cout << "No match for line: " << line << std::endl; // デバッグメッセージ
        }
    }

    infile.close();
    outfile.close();

    std::cout << "placement.txt has been created successfully." << std::endl;
    return 0;
}