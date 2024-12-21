import plotly.graph_objects as go

# graph.txtのパース関数
def parse_graph_file(filename):
    nodes = []
    edges = []
    with open(filename, 'r') as f:
        lines = f.readlines()

    # Nodeとedgeセクションを分割
    node_section = []
    edge_section = []
    reading_nodes = False
    reading_edges = False

    for line in lines:
        line = line.strip()
        if line == "Node":
            reading_nodes = True
            reading_edges = False
            continue
        if line == "edge":
            reading_nodes = False
            reading_edges = True
            continue

        if reading_nodes and line != "":
            # "nodeName x y z"形式
            parts = line.split()
            node_name = parts[0]
            x, y, z = map(float, parts[1:4])
            nodes.append((node_name, x, y, z))

        if reading_edges and line != "":
            # "id node1 node2 weight"形式
            parts = line.split()
            edge_id = parts[0]
            node1 = parts[1]
            node2 = parts[2]
            weight = float(parts[3])
            edges.append((node1, node2, weight))

    return nodes, edges

# ユーザからファイル名を入力
filename = input("読み込むgraphファイル名を入力してください: ")

# グラフデータ読み込み
nodes, edges = parse_graph_file(filename)

# ノード名から座標へのマップ作成
pos = { n: (x,y,z) for (n,x,y,z) in nodes }

# エッジ描画用のデータ作成
edge_x = []
edge_y = []
edge_z = []
for n1, n2, w in edges:
    x1, y1, z1 = pos[n1]
    x2, y2, z2 = pos[n2]
    # 線分描画のためのx,y,z座標を追加
    edge_x += [x1, x2, None]
    edge_y += [y1, y2, None]
    edge_z += [z1, z2, None]

# ノード描画用
node_x = [p[1] for p in nodes]
node_y = [p[2] for p in nodes]
node_z = [p[3] for p in nodes]
node_text = [p[0] for p in nodes]

# Plotlyを用いて3D空間に可視化
fig = go.Figure()

# エッジ描画
fig.add_trace(go.Scatter3d(
    x=edge_x,
    y=edge_y,
    z=edge_z,
    mode='lines',
    line=dict(color='black', width=2),
    hoverinfo='none'
))

# ノード描画
fig.add_trace(go.Scatter3d(
    x=node_x,
    y=node_y,
    z=node_z,
    mode='markers+text',
    text=node_text,
    textposition='top center',
    marker=dict(size=5, color='red'),
    hoverinfo='text'
))

# 軸などの設定
fig.update_layout(
    scene=dict(
        xaxis_title='X',
        yaxis_title='Y',
        zaxis_title='Z'
    ),
    width=800,
    height=600,
    title='3D Graph Visualization'
)

fig.show()
