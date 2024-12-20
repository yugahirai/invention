#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

const int GRID_SIZE = 10; // グリッドの大きさ

// グリッドを可視化して出力する関数
void printGrid(const std::vector<std::vector<int>>& grid, int closest_x, int closest_y) {
    for (int y = 0; y < GRID_SIZE; ++y) {
        for (int x = 0; x < GRID_SIZE; ++x) {
            if (x == closest_x && y == closest_y) {
                std::cout << "P ";  // 選ばれた点
            } else if (grid[y][x] == 1) {
                std::cout << "# ";  // 埋まっている場所
            } else {
                std::cout << ". ";  // 空いている場所
            }
        }
        std::cout << std::endl;
    }
}

// テストするグリッドと初期位置を設定
void test() {
    // 空のグリッドを作成 (0: 空き、1: 埋まっている)
    std::vector<std::vector<int>> grid(GRID_SIZE, std::vector<int>(GRID_SIZE, 0));

    // いくつかのセルを埋める
    grid[5][5] = 1; // 原点に近いセルを埋める
grid[4][5] = 1;
grid[5][4] = 1;
grid[5][6] = 1;
grid[4][7] = 1;
grid[6][4] = 1;

// さらに多くの障害物を追加
grid[3][5] = 1;
grid[5][3] = 1;
grid[6][5] = 1;
grid[5][7] = 1;
grid[6][6] = 1;
grid[7][4] = 1;
grid[4][4] = 1;
grid[4][6] = 1;
grid[3][4] = 1;
grid[7][5] = 1;
grid[6][3] = 1;
grid[3][6] = 1;
grid[2][5] = 1;
grid[5][2] = 1;


    // closest_x と closest_y を設定
    int closest_x = 5;
    int closest_y = 5;

    // グリッドの初期状態を表示
    std::cout << "Initial grid state:" << std::endl;
    printGrid(grid, closest_x, closest_y);

    // スナップのロジック
    bool found = false;
    int distance = 0;

    while (grid[closest_y][closest_x] && !found) {
        std::vector<std::pair<int, int>> points;

        for (int x = -distance; x <= distance; ++x) {
            int y = distance - std::abs(x);
            points.push_back({x, y});
            if (y != 0) {
                points.push_back({x, -y});
            }
        }

        for (int i = 0; i < points.size(); ++i) {
            std::srand(static_cast<unsigned int>(std::time(0))); // 乱数の初期化
            int random_index = std::rand() % points.size(); // ランダムインデックス

            std::pair<int, int> random_point = points[random_index];
            points.erase(points.begin() + random_index);

            // ランダムな点に移動可能なら
            if (!grid[closest_y + random_point.second][closest_x + random_point.first]) {
                closest_x = closest_x + random_point.first;
                closest_y = closest_y + random_point.second;
                found = true;  // フラグを立てて `while` を終了
                break;
            }
        }
        distance++;
    }

    // 結果を出力
    std::cout << "Closest available position: (" << closest_x << ", " << closest_y << ")" << std::endl;

    // 最終的なグリッドの状態を表示
    std::cout << "Final grid state with selected point:" << std::endl;
    printGrid(grid, closest_x, closest_y);
}

int main() {
    test();
    return 0;
}
