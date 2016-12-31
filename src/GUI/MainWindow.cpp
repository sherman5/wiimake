#include "MainWindow.h"

#include <QVBoxLayout>

#include "Inject.h"
#include "SaveLoad.h"
#include "IsoInfo.h"
#include "LibBuilder.h"
#include "../MainProgram/Parser.h"

/* constructor */
MainWindow::MainWindow() : QWidget()
{
    /* create iso path and config file boxes */
    mIsoPath = new PathInput("iso file:", this, false, "ISO File (*.iso)");
    mConfigFile = new PathInput("Configuration File:", this, false,
        "Config File (*.ini)");

    /* update game titles when config file changes */
    connect(mConfigFile, SIGNAL(valueChanged()), this,
        SLOT(getGameTitles()));       

    /* add game title list */
    mGameTitle = new QComboBox(this);
    mGameTitle->addItem("Game Title");

    /* add main functionality tabs */
    mTabs = new QTabWidget(this);

    mTabs->addTab(new InjectTab(this), "Inject C Code");
    mTabs->addTab(new SaveLoadTab(this), "Save/Load ISO State");
    mTabs->addTab(new IsoInfoTab(this), "ISO Info");
    mTabs->addTab(new LibBuilderTab(this), "Create Library");

    /* create layout */
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(mIsoPath);
    layout->addWidget(mConfigFile);
    layout->addWidget(mGameTitle);
    layout->addWidget(mTabs);
  
    setLayout(layout);
    setWindowTitle("GCI");
    resize(750, 400);
}

/* get arguments */
Arguments MainWindow::getArgs()
{
    /* struct that holds all arguments */
    Arguments args;
    
    /* get iso path */
    args.cmdOptions.insert(std::make_pair("--iso-file", mIsoPath->text()));

    /* get config file */
    args.cmdOptions.insert(std::make_pair("--config-file", 
        mConfigFile->text()));

    /* get game index */    
    if (mGameTitle->currentIndex() >= 1)
    {
        args.cmdOptions.insert(std::make_pair("--game-id", 
            std::to_string(mGameTitle->currentIndex())));
    }
    
    /* return arguments */
    return args;
}

/* get list of game titles from config file */
void MainWindow::getGameTitles()
{
    /* reset list */
    mGameTitle->clear();
    mGameTitle->addItem("Game Title");

    /* get list of titles */ 
    auto titles = ConfigParser::getGameTitles(mConfigFile->text());

    /* add titles to list */
    for (auto it = titles.begin(); it != titles.end(); ++it)
    {
        mGameTitle->addItem(QString::fromStdString(*it));
    }        
}

