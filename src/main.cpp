/*
 * Matthew Todd Geiger
 * main.cpp
 */

// Standard includes
#include <cstdlib>

// local includes
#include <macros.hpp>

// Qt Includes
#include <QApplication>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    
    // Event loop
    return app.exec();
}