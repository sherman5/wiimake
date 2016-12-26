#ifndef GUI_SAVE_LOAD_H
#define GUI_SAVE_LOAD_H

#include <QWidget>
#include <QObject>
#include <QPushButton>

#include "GUI_CustomObjects.h"
#include "GUI_MemConfig.h"

/* tab - manages the saving and loading of the iso state */
class SaveLoadTab : public QWidget {

    Q_OBJECT

private:

    QPushButton *m_save_button, *m_load_button;
    PathInput* m_iso_path;
    MemConfigTab* m_mem_config;

public:

    SaveLoadTab(PathInput*, MemConfigTab*, QWidget* = Q_NULLPTR);
    
public slots:

    void SaveFile();
    void LoadFile();

};
 
#endif

