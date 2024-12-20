#include "graph.h"
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <iostream>

void generatePDFFromDOT(const std::string& dotFile, const std::string& pdfFile) {
    // 'neato' を使用し、ノードの大きさを統一し、ノード間の隙間を増やす
    std::string command = "neato -Tpdf " + dotFile + " -Goverlap=false -Gsep=2.0 -Nshape=circle -Nwidth=0.5 -o " + pdfFile;
    int result = system(command.c_str());

    if (result == 0) {
        std::cout << "PDF successfully generated: " << pdfFile << std::endl;
    } else {
        std::cerr << "Error: Failed to generate PDF from DOT file." << std::endl;
    }
}

void loadGraphFromFile(const std::string& filename, Graph& graph, int weightThreshold) {
    std::ifstream file(filename);
    std::string line;
    bool readingNodes = false;
    bool readingEdges = false;

    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }
        if (line == "Node") {
            readingNodes = true;
            readingEdges = false;
            continue;
        } else if (line == "edge") {
            readingNodes = false;
            readingEdges = true;
            continue;
        }

        if (readingNodes) {
            graph.addNode(line);
        } else if (readingEdges) {
            std::istringstream iss(line);
            int edgeId, weight;
            std::string node1, node2;
            iss >> edgeId >> node1 >> node2 >> weight;

            if (weight > weightThreshold) {
                graph.addEdge(edgeId, node1, node2, weight);
            }
        }
    }

    file.close();
}

int main() {
    Graph graph;

    int weightThreshold;
    std::cout << "Enter the weight threshold: ";
    std::cin >> weightThreshold;

    loadGraphFromFile("../graph.txt", graph, weightThreshold);

    graph.layoutGraph(); // ノードのレイアウトをグリッド上に配置

    graph.display();

    std::string dotFile = "../graph.dot";
    graph.exportToDOT(dotFile);

    std::string pdfFile = "../graph.pdf";
    generatePDFFromDOT(dotFile, pdfFile);

    return 0;
}
