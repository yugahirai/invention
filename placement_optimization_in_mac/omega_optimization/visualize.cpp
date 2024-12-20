#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

// グリッドのサイズ（適宜変更）
const int WIDTH = 50;  // 横方向の最大サイズ
const int HEIGHT = 50; // 縦方向の最大サイズ

int main() {
    // グリッドを初期化（. で埋める）
    std::vector<std::vector<char>> grid(HEIGHT, std::vector<char>(WIDTH, '.'));

    std::ifstream infile("placement.txt");

    if (!infile.is_open()) {
        std::cerr << "Failed to open placement.txt" << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string command, node_name;
        int x, y;

        if (!(iss >> command >> node_name >> x >> y)) {
            continue; // 読み込みエラーはスキップ
        }

        // グリッド内に座標が収まる場合のみ 'Q' を配置
        if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
            grid[y][x] = 'Q';  // yが行、xが列に対応
        }
    }

    infile.close();

    // グリッドを表示
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            std::cout << grid[y][x] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
