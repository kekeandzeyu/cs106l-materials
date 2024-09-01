/*
 * TODO: Fill out this file with a brief overview of the program.
 */
#include <iostream>
#include "SimpleGraph.h"
#include "constants.h"

#include <QtGui>
#include <QWidget>
#include <QApplication>

void Welcome();

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