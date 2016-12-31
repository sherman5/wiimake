#include "SaveLoad.h"

#include <QFileDialog>
#include <QVBoxLayout>

#include "../MainProgram/GCI.h"
#include "../MainProgram/Global.h"

/* constructor */
SaveLoadTab::SaveLoadTab(MainWindow* parent) : QWidget(parent)
{
    /* store parent window */
    mMainWindow = parent;

    /* create save and load buttons */
    QPushButton* saveButton = new QPushButton("SAVE", this);
    QPushButton* loadButton = new QPushButton("LOAD", this);

    /* connect buttons to functions */
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveFile()));
    connect(loadButton, SIGNAL(clicked()), this, SLOT(loadFile()));
    
    /* create layout */
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(saveButton);
    layout->addWidget(loadButton);
    setLayout(layout);
}

/* save iso state to file */
void SaveLoadTab::saveFile()
{
    /* create file dialog */
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter("Data File (*.data)");
    dialog.setViewMode(QFileDialog::Detail);

    /* get save file name */
    std::string file = dialog.selectedFiles().at(0).toStdString();

    /* create arguments */
    Arguments args = mMainWindow->getArgs();
    args.cmdOptions.insert(std::make_pair("--save", file));

    /* save file */
    GCI::saveISO(args);
}

/* load iso state from file */
void SaveLoadTab::loadFile()
{
    /* create file dialog */
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter("Data File (*.data)");
    dialog.setViewMode(QFileDialog::Detail);
    
    /* get load file name */ 
    std::string file = dialog.selectedFiles().at(0).toStdString();

    /* create arguments */
    Arguments args = mMainWindow->getArgs();
    args.cmdOptions.insert(std::make_pair("--load", file));

    /* load file */    
    GCI::loadISO(args);
}


