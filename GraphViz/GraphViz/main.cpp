/*
 * GraphViz assignment from Stanford CS106L
 * @author Keith Schwartz
 * @author Avery Wang
 * @author Zeyu Li
 */
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include <chrono>
#include "SimpleGraph.h"
#include "constants.h"

#include <QtGui>
#include <QWidget>
#include <QApplication>

constexpr double kpi = 3.14159265358979323;
constexpr double kattract = 0.001;
constexpr double krepel = 0.001;

void Welcome();
void PromptUserForFile(std::ifstream& file);
int PromptUserForMicroseconds();
void InitializeGraph(SimpleGraph& userGraph, int numNodes);
void addEdges(SimpleGraph& userGraph, const std::vector<std::pair<int, int>>& edges);
void RunForceDirectedAlgorithm(SimpleGraph& graph, int microseconds);

// Main method
int main(int argc, char** argv) {
    QApplication app(argc, argv);

    Welcome();

    MyWidget& myWidget = MyWidget::getInstance();
    myWidget.resize(kWindowWidth, kWindowHeight);
    myWidget.show();
    qRegisterMetaType<SimpleGraph>();

    SimpleGraph userGraph;
    InitGraphVisualizer(userGraph);

    std::string YesOrNo = "Yes";
    while (YesOrNo == "Yes") {
        std::ifstream file;
        PromptUserForFile(file);
        const int time = PromptUserForMicroseconds();
        int numNodes;
        std::vector<std::pair<int, int>> edges;
        file >> numNodes;
        while (!file.eof()) {
            int start, end;
            file >> start >> end;
            edges.emplace_back(start, end);
        }
        InitializeGraph(userGraph, numNodes);
        addEdges(userGraph, edges);

        RunForceDirectedAlgorithm(userGraph, time);

        std::cout << "Want to try another file? (Yes/No): ";
        std::cin >> YesOrNo;
        if (YesOrNo == "Yes") {
            userGraph.nodes.clear();
            userGraph.edges.clear();
        }
    }

    return QApplication::exec();
}

/* Prints a message to the console welcoming the user and
 * describing the program. */
void Welcome() {
    std::cout << "Welcome to CS106L GraphViz!" << std::endl;
    std::cout << "This program uses a force-directed graph layout algorithm" << std::endl;
    std::cout << "to render sleek, snazzy pictures of various graphs." << std::endl;
    std::cout << std::endl;
}

/*
 * Prompt the user to type in a file name; if fails, reprompt the user
 * until a valid file name is entered.
 * @param file the ifstream to open the file
 */
void PromptUserForFile(std::ifstream& file) {
    while (true) {
        std::cout << "Please enter the name of file you want to visualize: ";
        std::string filename;
        getline(std::cin, filename);
        file.open(filename);
        if (!file.fail()) {
            break;
        }
        std::cout << "Sorry, I can't open that file. Please try again." << std::endl;
        file.clear();
    }
}

/*
 * Prompt the user for the number of microseconds to run the algorithm,
 * which should be a positive integer. If the input is invalid, reprompt.
 * @return the number of microseconds to run the algorithm
 */
int PromptUserForMicroseconds() {
    int microseconds;
    while (true) {
        std::cout << "Please enter the number of microseconds to run the algorithm: ";
        std::cin >> microseconds;
        if (std::cin.fail() || microseconds <= 0) {
            std::cout << "Please enter a positive integer." << std::endl;
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }
        else {
            break;
        }
    }
    return microseconds;
}

/*
 * Initialize the graph by reading the number of nodes and place
 * the nodes in a circle.
 * @param numNodes the number of nodes in the graph
 */
void InitializeGraph(SimpleGraph& userGraph, const int numNodes)
{
    for (int i = 0; i < numNodes; i++) {
        const double angle = 2 * kpi * i / numNodes;
        Node n{};
        n.x = kCircleRadius * cos(angle) + static_cast<double>(kWindowWidth) / 2;
        n.y = kCircleRadius * sin(angle) + static_cast<double>(kWindowHeight) / 2;
        userGraph.nodes.push_back(n);
    }
}

/*
 * Add edges to the graph.
 * @param userGraph the graph to add edges to
 * @param edges the edges to add
 */
void addEdges(SimpleGraph& userGraph, const std::vector<std::pair<int, int>>& edges) {
    for (const auto& [fst, snd] : edges) {
        Edge e{};
        e.start = fst;
        e.end = snd;
        userGraph.edges.push_back(e);
    }
    DrawGraph(userGraph);
}

/*
 * Compute the attractive forces and repulsive forces between nodes.
 * Pay attention to the time elapsed and break the loop if the time
 * exceeds the given microseconds.
 * @param userGraph the graph to visualize
 * @param microseconds the number of microseconds to run the algorithm
 */
void RunForceDirectedAlgorithm(SimpleGraph& graph, int microseconds) {
    const auto start = std::chrono::high_resolution_clock::now();
    while (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count() < microseconds) {
        // Calculate forces and update node positions
        std::vector<double> deltaX(graph.nodes.size(), 0.0);
        std::vector<double> deltaY(graph.nodes.size(), 0.0);

        // Attractive forces
        for (const auto& edge : graph.edges) {
            const Node& node1 = graph.nodes[edge.start];
            const Node& node2 = graph.nodes[edge.end];

            const double distSq = (node2.y - node1.y) * (node2.y - node1.y) + (node2.x - node1.x) * (node2.x - node1.x);
            const double Fattract = kattract * distSq;
            const double theta = atan2(node2.y - node1.y, node2.x - node1.x);

            deltaX[edge.start] += Fattract * cos(theta);
            deltaY[edge.start] += Fattract * sin(theta);
            deltaX[edge.end] -= Fattract * cos(theta);
            deltaY[edge.end] -= Fattract * sin(theta);
        }

        // Repulsive forces
        for (size_t i = 0; i < graph.nodes.size(); ++i) {
            for (size_t j = i + 1; j < graph.nodes.size(); ++j) {
                const Node& node1 = graph.nodes[i];
                const Node& node2 = graph.nodes[j];

                const double dist = sqrt((node2.y - node1.y) * (node2.y - node1.y) + (node2.x - node1.x) * (node2.x - node1.x));
                const double Frepel = krepel / dist;
                const double theta = atan2(node2.y - node1.y, node2.x - node1.x);

                deltaX[i] -= Frepel * cos(theta);
                deltaY[i] -= Frepel * sin(theta);
                deltaX[j] += Frepel * cos(theta);
                deltaY[j] += Frepel * sin(theta);
            }
        }

        // Update node positions
        for (size_t i = 0; i < graph.nodes.size(); ++i) {
            graph.nodes[i].x += deltaX[i];
            graph.nodes[i].y += deltaY[i];
        }

        DrawGraph(graph);

        // Process events and add a small delay
        QCoreApplication::processEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}