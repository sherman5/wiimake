#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QObject>
#include <QWidget>
#include <QTabWidget>

#include "GUI_CustomObjects.h"
#include "MemoryConfig.h"

/* main window for the GUI */
class MainWindow : public QWidget {

    Q_OBJECT

private:

    QTabWidget* m_tabs;
    PathInput* m_iso_path;

public:

    MainWindow();

    std::string GetIsoPath();

};
 
#endif
