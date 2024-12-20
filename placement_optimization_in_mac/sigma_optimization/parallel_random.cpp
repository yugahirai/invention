#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <vector>

// 定数の定義
const int NUM_QUBITS = 1000;      // Qubitの数
const int NUM_GATES = 10000;      // ゲートの数
const std::string GATE_TYPE = "CX"; // ゲートの種類（必要に応じて変更可能）
const std::string OUTPUT_FILE = "gate.raw"; // 出力ファイル名

int main() {
    // 各qubitに対する重みを設定
    // ここでは qubit_0 から qubit_99 の重みを1000.0に設定し、qubit_100 から qubit_999 の重みを1.0に設定
    std::vector<double> qubit_weights(NUM_QUBITS, 1.0); // 初期重みを1.0に設定

    // 非常に高い重みを設定するqubitの範囲
    for(int i = 0; i < 100; ++i){
        qubit_weights[i] = 1000.0; // qubit_0 から qubit_99 の重みを1000.0に設定
    }

    // ランダム数生成器の設定
    std::random_device rd;
    std::mt19937 gen(rd());

    // 重みに基づく分布を作成
    std::discrete_distribution<> dis(qubit_weights.begin(), qubit_weights.end());

    // 出力ファイルを開く
    std::ofstream ofs(OUTPUT_FILE);
    if (!ofs.is_open()) {
        std::cerr << "ファイル " << OUTPUT_FILE << " を開くことができませんでした。\n";
        return 1;
    }

    // ゲートの生成と書き込み
    for (int i = 0; i < NUM_GATES; ++i) {
        int control = dis(gen);
        int target = dis(gen);

        // 同じqubitが選ばれないように再選択
        while (target == control) {
            target = dis(gen);
        }

        ofs << GATE_TYPE << " qubit_" << control << " qubit_" << target << "\n";
    }

    ofs.close();
    std::cout << NUM_GATES << "個のゲートが" << OUTPUT_FILE << "に出力されました。\n";

    return 0;
}