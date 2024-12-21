
# ターミナルをPDFに設定（フォントとサイズを指定）
set terminal pdfcairo enhanced font '/Users/hiraiyuuware/Library/Fonts/latinmodern-math.otf,30' size 12,6

# 出力ファイルの指定
set output 'distance_3D_optimized_1.pdf'

# タイトルを削除
unset title

# 軸ラベルの設定
set xlabel 'Instruction Number' font '/Users/hiraiyuuware/Library/Fonts/latinmodern-math.otf,45'
set ylabel 'Distance' font '/Users/hiraiyuuware/Library/Fonts/latinmodern-math.otf,45'

# グリッドを有効化
set grid

# 凡例の設定
set key font '/Users/hiraiyuuware/Library/Fonts/latinmodern-math.otf,45'

# 平均値を関数として定義
y(x) = 6.2762837934487195

# 軸の範囲を固定
set xrange [1:13982]
set yrange [0:80]

# データのプロット
plot 'distance_3D_optimized_1.txt' using (column(0)+1):1 with lines lt rgb '#4B0082' lw 2 title 'Distance', \
     y(x) with lines lt rgb 'red' lw 2 title 'Average'
