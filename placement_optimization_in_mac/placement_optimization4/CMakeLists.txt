cmake_minimum_required(VERSION 3.10)

# プロジェクト名と使用するC++の標準バージョンを設定
project(GraphVisualization)
set(CMAKE_CXX_STANDARD 11)

# 実行ファイルを作成（main.cppとgraph.hを使用）
add_executable(graph_app main.cpp)

# Graphvizのインストール確認
find_program(GRAPHVIZ_DOT_EXECUTABLE dot)
if(NOT GRAPHVIZ_DOT_EXECUTABLE)
    message(FATAL_ERROR "Graphviz 'dot' command not found. Please install Graphviz.")
endif()

# graph_appを実行してgraph.dotを生成し、GraphvizでPNGに変換するカスタムコマンド
add_custom_command(
    TARGET graph_app
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Running graph_app to generate graph.dot"
    COMMAND ./graph_app
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Converting graph.dot to graph.png using Graphviz"
)
