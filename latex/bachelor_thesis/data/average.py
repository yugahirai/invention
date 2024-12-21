import os

# 処理対象のファイル名リスト
file_names = [
    "distance_2D.txt",
    "distance_2D_1.txt",
    "distance_2D_optimized.txt",
    "distance_2D_optimized_1.txt",
    "distance_3D.txt",
    "distance_3D_1.txt",
    "distance_3D_optimized.txt",
    "distance_3D_optimized_1.txt",
]

# 結果を格納する辞書
mean_values = {}

# 各ファイルを処理
for fname in file_names:
    # ファイルが存在するかチェック
    if not os.path.isfile(fname):
        print(f"ファイルが見つかりません: {fname}")
        continue

    try:
        with open(fname, 'r') as f:
            # ファイルの全行を読み込み
            lines = f.readlines()
        
        # 各行を整数に変換し、リストに格納
        y_values = []
        for line in lines:
            stripped_line = line.strip()
            if stripped_line:  # 空行をスキップ
                try:
                    y = int(stripped_line)
                    y_values.append(y)
                except ValueError:
                    print(f"無効なデータが含まれています（{fname}）: '{stripped_line}'")
        
        # y値が存在する場合、平均を計算
        if y_values:
            mean = sum(y_values) / len(y_values)
            mean_values[fname] = mean
        else:
            print(f"データがありません: {fname}")
    
    except Exception as e:
        print(f"ファイルの処理中にエラーが発生しました（{fname}）: {e}")

# 結果の表示
print("\n各ファイルのy値の平均:")
for fname, mean in mean_values.items():
    print(f"{fname}: {mean:.2f}")
