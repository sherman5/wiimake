#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QObject>
#include <QWidget>
#include <QTabWidget>
#include <QComboBox>

#include "../MainProgram/Global.h"
#include "PathInput.h"

/* main window for the GUI */
class MainWindow : public QWidget
{
    Q_OBJECT

private:

    /* tabs with all main function of program */
    QTabWidget* mTabs;

    /* file paths */
    PathInput* mIsoPath;
    PathInput* mConfigFile;

    /* drop down menu to select game in config file */
    QComboBox* mGameTitle;

public:

    /* constructor */
    MainWindow();

    /* get arguments */
    Arguments getArgs();

public slots:
    
    /* get list of game titles from config file */
    void getGameTitles();
};
 
#endif
