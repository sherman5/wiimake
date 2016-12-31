#ifndef INJECT_H
#define INJECT_H

#include <vector>

#include <QWidget>
#include <QObject>
#include <QPushButton>
#include <QGridLayout>

#include "MainWindow.h"
#include "PathInput.h"

/* tab - manages the injection of C code into the iso */
class InjectTab : public QWidget
{
    Q_OBJECT

private:

    /* c code source directory and libs/includes */
    PathInput* mSourceDir;
    std::vector<PathInput*> mLibs, mIncludeDirs;    

    /* pointer to main window containint this tab */
    MainWindow* mMainWindow;

    /* sub-layouts for libs and includes */
    QVBoxLayout *mLibLayout, *mIncludeLayout;

public:

    /* constructor */
    InjectTab(MainWindow*);

public slots:

    /* add more paths */
    void addLibPath();
    void addIncludeDir();

    /* inject code into iso */
    void createISO();
};
 
#endif
