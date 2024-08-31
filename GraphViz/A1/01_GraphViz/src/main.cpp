/*
 * This is main.cpp about Graph Visualization for Stanford CS106L.
 * @author Zeyu Li
 */
#include <iostream>
#include <fstream>
#include <cmath>
#include <chrono>
#include <vector>
#include <string>
#include <limits>
#include "SimpleGraph.h"

using namespace std;

void Welcome();
int getPositiveInteger(string prompt);
void placeNodesInCircle(SimpleGraph& graph, int numNodes);
void addEdgesFromFile(SimpleGraph& graph, ifstream& infile);
void runForceDirectedLayout(SimpleGraph& graph, int microseconds);
bool getYesNoAnswer(string prompt);

const double kPi = 3.14159265358979323846;
const double kattract = 1;
const double krepel = 1;

// Main method
int main() {
    Welcome();

    SimpleGraph graph;
    InitGraphVisualizer(graph);

    string filename = "C:\\Users\\kekea\\Desktop\\Stanford-CS106L-Study-Materials\\GraphViz\\A1\\01_GraphViz\\res\\31binary-tree";
    ifstream file(filename);
    if (file.is_open()) {
        int numNodes;
        file >> numNodes;

        graph.nodes.clear();
        graph.edges.clear();

        placeNodesInCircle(graph, numNodes);
        addEdgesFromFile(graph, file);

        int microseconds = getPositiveInteger("Enter the number of microseconds to run the algorithm: ");

        runForceDirectedLayout(graph, microseconds);

        DrawGraph(graph);

        file.close();
    }
    else {
        cerr << "Unable to open file " << filename << "!" << endl;
    }
    return 0;
}

/* Prints a message to the console welcoming the user and
 * describing the program. */
void Welcome() {
    cout << "Welcome to CS106L GraphViz!" << endl;
    cout << "This program uses a force-directed graph layout algorithm" << endl;
    cout << "to render sleek, snazzy pictures of various graphs." << endl;
    cout << endl;
}

/* Prompts the user for a positive integer using the given prompt.
 * If the input is invalid, reprompts the user until a valid input
 * is received.
 * @param prompt - the prompt to display to the user
 * @return the positive integer entered by the user
 */
int getPositiveInteger(string prompt) {
    int num;

    cout << prompt;

    cin >> num;

    return num;
}

/* Prompts the user for a yes/no answer using the given prompt.
 * If the input is invalid, reprompts the user until a valid input
 * is received.
 * @param prompt - the prompt to display to the user
 * @return true if the user enters "y" or "yes", false otherwise
 */
bool getYesNoAnswer(string prompt) {
    string answer;

    do {
        cout << prompt;
        cin >> answer;

        // Convert answer to lowercase for case-insensitive comparison
        transform(answer.begin(), answer.end(), answer.begin(), ::tolower);

        if (answer == "y" || answer == "yes") {
            return true;
        } else if (answer == "n" || answer == "no") {
            return false;
        } else {
            cout << "Invalid input. Please enter 'y' or 'n'." << endl;
        }
    } while (true);
}


/* Places the nodes of the graph in a circle.
 * @param graph - the SimpleGraph to modify
 * @param numNodes - the number of nodes to place in the circle
 */
void placeNodesInCircle(SimpleGraph& graph, int numNodes) {
    graph.nodes.resize(numNodes);

    for (int k = 0; k < numNodes; ++k) {
        double angle = 2 * kPi * k / numNodes;
        graph.nodes[k].x = cos(angle);
        graph.nodes[k].y = sin(angle);
    }
}

/* Adds edges to the graph from the input file.
 * @param graph - the SimpleGraph to modify
 * @param infile - the input file stream
 */
void addEdgesFromFile(SimpleGraph& graph, ifstream& infile) {
    int numNodes;
    infile >> numNodes;

    size_t start, end;
    while (infile >> start >> end) {
        if (start >= 0 && start < graph.nodes.size() &&
            end >= 0 && end < graph.nodes.size()) {
            Edge edge;
            edge.start = start;
            edge.end = end;
            graph.edges.push_back(edge);
        } else {
            std::cerr << "Wrong node: " << start << " or " << end << std::endl;
        }
    }
}

/* Runs the force-directed layout algorithm on the graph for the specified duration.
 * @param graph - the SimpleGraph to modify
 * @param microseconds - the duration to run the algorithm in microseconds
 */
void runForceDirectedLayout(SimpleGraph& graph, int microseconds) {
    auto start = chrono::high_resolution_clock::now();

    while (true) {
        auto end = chrono::high_resolution_clock::now();
        auto elapsed = chrono::duration_cast<chrono::microseconds>(end - start);
        if (elapsed.count() > microseconds) {
            break;
        }

        // Vectors to store the delta_x and delta_y for each node
        vector<double> delta_x(graph.nodes.size(), 0.0);
        vector<double> delta_y(graph.nodes.size(), 0.0);

        // 1. Compute attractive forces for each edge
        for (const Edge& edge : graph.edges) {
            size_t i = edge.start;
            size_t j = edge.end;
            double dx = graph.nodes[j].x - graph.nodes[i].x;
            double dy = graph.nodes[j].y - graph.nodes[i].y;
            double distSq = dx * dx + dy * dy;
            double Fattract = kattract * distSq;
            double theta = atan2(dy, dx);

            delta_x[i] += Fattract * cos(theta);
            delta_y[i] += Fattract * sin(theta);
            delta_x[j] -= Fattract * cos(theta);
            delta_y[j] -= Fattract * sin(theta);
        }

        // 2. Compute repulsive forces for each pair of nodes
        for (size_t i = 0; i < graph.nodes.size(); ++i) {
            for (size_t j = i + 1; j < graph.nodes.size(); ++j) {
                double dx = graph.nodes[j].x - graph.nodes[i].x;
                double dy = graph.nodes[j].y - graph.nodes[i].y;
                double dist = sqrt(dx * dx + dy * dy);
                double Frepel = krepel / dist;
                double theta = atan2(dy, dx);

                delta_x[i] -= Frepel * cos(theta);
                delta_y[i] -= Frepel * sin(theta);
                delta_x[j] += Frepel * cos(theta);
                delta_y[j] += Frepel * sin(theta);
            }
        }

        // 3. Update node positions
        for (size_t i = 0; i < graph.nodes.size(); ++i) {
            graph.nodes[i].x += delta_x[i];
            graph.nodes[i].y += delta_y[i];
        }
    }
}
