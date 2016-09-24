#include <QApplication>
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>
#include <QLabel>
#include <QFileDialog>

#include "GUIwindow.h"
 
int main(int argc, char **argv) {

    QApplication app (argc, argv);
 
    GUIwindow* main_window = new GUIwindow();
    main_window->show();

    return app.exec();

}
