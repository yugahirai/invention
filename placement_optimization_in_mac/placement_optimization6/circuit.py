import cirq
import numpy as np
import matplotlib.pyplot as plt
from cirq.contrib.svg import SVGCircuit
from IPython.display import display

def quantum_random_walk(num_steps):
    """
    1次元の量子ランダムウォークを実行し、回路を表示する関数。

    :param num_steps: ランダムウォークのステップ数。
    """
    # 位置（ポジション）とコイン（方向）用の量子ビットを用意
    position_qubits = [cirq.LineQubit(i) for i in range(num_steps)]
    coin_qubit = cirq.LineQubit(num_steps)

    # 回路の構築
    circuit = cirq.Circuit()

    # 初期状態の設定：位置0、コインは|0⟩
    # コインにアダマールゲートを適用してスーパー・ポジションを作成
    circuit.append(cirq.H(coin_qubit))

    # ランダムウォークのステップを実行
    for step in range(num_steps):
        # コイン投げ（アダマールゲート）
        circuit.append(cirq.H(coin_qubit))

        # シフトオペレーション
        for i, qubit in enumerate(position_qubits):
            # 位置をエンコードするための制御ゲートを適用
            circuit.append(cirq.CNOT(coin_qubit, qubit))

    # 測定
    circuit.append(cirq.measure(*position_qubits, key='position'))

    # シミュレーションの実行
    simulator = cirq.Simulator()
    result = simulator.run(circuit, repetitions=1000)
    measurements = result.histogram(key='position')

    # 結果のプロット
    positions = list(measurements.keys())
    frequencies = list(measurements.values())
    probabilities = [freq / 1000 for freq in frequencies]

    plt.bar(positions, probabilities)
    plt.xlabel('Position')
    plt.ylabel('Probability')
    plt.title('Quantum Random Walk after {} steps'.format(num_steps))
    plt.show()

    # 回路図の表示（Jupyter Notebook）
    display(SVGCircuit(circuit))

# 使用例
num_steps = 5  # ランダムウォークのステップ数
quantum_random_walk(num_steps)