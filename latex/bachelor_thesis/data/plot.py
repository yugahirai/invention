import subprocess
import os

# ユーザーから入力ファイル名を受け取る
data_file = input("読み込むファイル名を入力してください: ")

# ファイルの存在確認
if not os.path.isfile(data_file):
    raise FileNotFoundError(f"データファイル '{data_file}' が存在しません。")

# 出力ファイル名を設定（拡張子を.pdfに変更）
base_name = os.path.splitext(data_file)[0]
output_pdf = base_name + '.pdf'

# フォントファイルのパスを指定（システムに応じて変更してください）
# 例:
# Windows: 'C:/Windows/Fonts/LatinModernMath-Regular.otf'
# macOS: '/Library/Fonts/LatinModernMath-Regular.otf'
# Linux: '/usr/share/fonts/opentype/public/lm/lm-math/LatinModernMath-Regular.otf'
font_path = '/Users/hiraiyuuware/Library/Fonts/latinmodern-math.otf'  # macOSのパスを使用

# フォントファイルの存在確認
if not os.path.isfile(font_path):
    raise FileNotFoundError(f"フォントファイル '{font_path}' が存在しません。")

# データの読み込みと平均値の計算
with open(data_file, 'r') as file:
    data = [float(line.strip()) for line in file if line.strip()]

average = sum(data) / len(data) if data else 0

# データの数（x軸の最大値）
x_max = len(data)

# y軸の最小値と最大値
y_min = min(data) if data else 0
y_max = max(data) if data else 1

# 軸の範囲を設定（必要に応じて調整してください）
# ここではx軸を1からx_maxまで、y軸をy_minとy_maxに設定
# 少し余裕を持たせる場合は、y_min - delta, y_max + delta
delta = (y_max - y_min) * 0.05 if y_max != y_min else 1  # 5%の余裕
y_lower = y_min - delta if y_min - delta > 0 else 0
y_upper = y_max + delta

# GNUplotスクリプトの作成
gnuplot_script = f"""
# ターミナルをPDFに設定（フォントとサイズを指定）
set terminal pdfcairo enhanced font '{font_path},30' size 12,6

# 出力ファイルの指定
set output '{output_pdf}'

# タイトルを削除
unset title

# 軸ラベルの設定
set xlabel 'Instruction Number' font '{font_path},45'
set ylabel 'Distance' font '{font_path},45'

# グリッドを有効化
set grid

# 凡例の設定
set key font '{font_path},45'

# 平均値を関数として定義
y(x) = {average}

# 軸の範囲を固定
set xrange [1:{x_max}]
set yrange [{y_lower}:80]

# データのプロット
plot '{data_file}' using (column(0)+1):1 with lines lt rgb '#4B0082' lw 2 title 'Distance', \\
     y(x) with lines lt rgb 'red' lw 2 title 'Average'
"""

# GNUplotスクリプトをファイルに書き込む
with open('plot_distance.gp', 'w') as gp_file:
    gp_file.write(gnuplot_script)

# GNUplotスクリプトの実行
try:
    subprocess.run(['gnuplot', 'plot_distance.gp'], check=True)
    print(f"プロットが正常に生成され、'{output_pdf}' に保存されました。")
except subprocess.CalledProcessError as e:
    print("GNUplotの実行中にエラーが発生しました:")
    print(e)

# オプション: 一時的なGNUplotスクリプトファイルを削除
# os.remove('plot_distance.gp')
