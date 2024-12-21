def main():
    input_file = "placement.txt"
    allocates = []

    # 1回目の読み込み：全ALLOCATE行を取得し、x,yのmin,maxを求める
    min_x = float('inf')
    max_x = float('-inf')
    min_y = float('inf')
    max_y = float('-inf')

    with open(input_file, 'r', encoding='utf-8') as f:
        for line in f:
            line = line.strip()
            if line.startswith("ALLOCATE"):
                parts = line.split()
                name = parts[1]
                x = int(parts[2])
                y = int(parts[3])
                allocates.append((name, x, y))

                if x < min_x:
                    min_x = x
                if x > max_x:
                    max_x = x
                if y < min_y:
                    min_y = y
                if y > max_y:
                    max_y = y

    # 中心点および半辺長を計算
    width = (max_x - min_x + 1)
    height = (max_y - min_y + 1)
    
    half_width = width // 2
    half_height = height // 2

    mid_x = min_x + half_width
    mid_y = min_y + half_height

    # 出力ファイル
    output_file = "placement_3d.txt"

    with open(input_file, 'r', encoding='utf-8') as fin, open(output_file, 'w', encoding='utf-8') as fout:
        for line in fin:
            line = line.strip()

            # ALLOCATE行の座標を変換して出力
            if line.startswith("ALLOCATE"):
                parts = line.split()
                name = parts[1]
                x = int(parts[2])
                y = int(parts[3])

                # 象限判定と新座標計算 (4分割)
                if x < mid_x and y < mid_y:
                    z = 0
                    x_new = x - min_x
                    y_new = y - min_y
                elif x >= mid_x and y < mid_y:
                    z = 1
                    x_new = x - mid_x
                    y_new = y - min_y
                elif x < mid_x and y >= mid_y:
                    z = 2
                    x_new = x - min_x
                    y_new = y - mid_y
                else:  # x >= mid_x and y >= mid_y
                    z = 3
                    x_new = x - mid_x
                    y_new = y - mid_y

                # X座標オフセット +3、Y座標オフセット +1
                x_new += 3
                y_new += 1

                fout.write(f"ALLOCATE {name} {x_new} {y_new} {z}\n")
            
            # MAGIC行については z=0 を付加して出力
            elif line.startswith("MAGIC"):
                # 例: "MAGIC magic0 0 0" -> "MAGIC magic0 0 0 0"
                fout.write(line + " 0\n")
            
            # それ以外の行はそのまま出力
            else:
                fout.write(line + "\n")

    print("3Dへの変換が完了しました。'placement_3d.txt'に出力しました。")


if __name__ == "__main__":
    main()
