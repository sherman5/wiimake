#ifndef SAVE_LOAD_H
#define SAVE_LOAD_H

#include <QWidget>
#include <QObject>
#include <QPushButton>

#include "MainWindow.h"
#include "PathInput.h"

/* tab - manages the saving and loading of the iso state */
class SaveLoadTab : public QWidget
{
    Q_OBJECT

private:

    /* pointer to parent window */
    MainWindow* mMainWindow;

public:

    /* constructor */    
    SaveLoadTab(MainWindow*);
    
public slots:

    /* save/load iso state to/from file */
    void saveFile();
    void loadFile();
};
 
#endif

