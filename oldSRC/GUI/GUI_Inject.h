#ifndef GUI_INJECT_H
#define GUI_INJECT_H

#include <vector>

#include <QWidget>
#include <QObject>
#include <QPushButton>
#include <QGridLayout>

#include "GUI_CustomObjects.h"
#include "GUI_MemConfig.h"

/* tab - manages the injection of C code into the iso */
class InjectTab : public QWidget {

    Q_OBJECT

private:

    PathInput* m_source_dir;
    std::vector<PathInput*> m_libraries;
    std::vector<PathInput*> m_include_dirs;    

    QPushButton *m_add_lib_button, *m_add_include_button, *m_create_button;

    PathInput* m_iso_path;
    MemConfigTab* m_mem_config_tab; 

    QGridLayout* m_layout;
    QVBoxLayout *m_lib_layout, *m_include_layout;

public:

    InjectTab(PathInput*, MemConfigTab*, QWidget* = Q_NULLPTR);

public slots:

    void AddLibPath();
    void AddIncludeDir();
    void CreateISO();

};
 
#endif
