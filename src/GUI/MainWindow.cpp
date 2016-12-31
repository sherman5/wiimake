#include "MainWindow.h"

#include <QVBoxLayout>

#include "Inject.h"
#include "SaveLoad.h"
#include "IsoInfo.h"
#include "LibBuilder.h"

/* constructor */
MainWindow::MainWindow() : QWidget()
{
    mIsoPath = new PathInput("iso file:", this, false, "ISO File (*.iso)");
    mConfigFile = new PathInput("Configuration File:", this, false,
        "Config File (*.ini)");

    mTabs = new QTabWidget(this);

   
    mTabs->addTab(new InjectTab(this), "Inject C Code");
    mTabs->addTab(new SaveLoadTab(this), "Save/Load ISO State");
    mTabs->addTab(new IsoInfoTab(this), "ISO Info");
    mTabs->addTab(new LibBuilderTab(this), "Create Library");

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(mIsoPath);
    layout->addWidget(mConfigFile);
    layout->addWidget(mTabs);
  
    setLayout(layout);
    setWindowTitle("GCI");
    resize(750, 400);
}

/* get arguments */
Arguments MainWindow::getArgs()
{
    return Arguments();
}


//    return mIsoPath->text();

