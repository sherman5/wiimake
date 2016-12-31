#include "Inject.h"

#include <QMessageBox>

#include "../MainProgram/GCI.h"

/* constructor */
InjectTab::InjectTab(MainWindow* parent) : QWidget(parent)
{
    /* store iso and config file paths */
    mMainWindow = parent;

    /* populate the default fields of the window */
    mSourceDir = new PathInput("C &Source Code Directory:", this);
    mLibs.push_back(new PathInput("&Library 1", this, false,
        "Static Library (*.a)"));
    mIncludeDirs.push_back(new PathInput("&Include Path 1", this));

    /* add buttons to allow for additional paths; create main button */
    QPushButton* addLib = new QPushButton("+", this);
    QPushButton* addInclude = new QPushButton("+", this);
    QPushButton* injectCode = new QPushButton("Inject Code", this);

    /* connect buttons with appropiate functions */
    connect(addLib, SIGNAL(clicked()), this, SLOT(addLibPath()));
    connect(addInclude, SIGNAL(clicked()), this, SLOT(addIncludeDir())); 
    connect(injectCode, SIGNAL(clicked()), this, SLOT(createISO())); 

    /* create initial layout */
    QGridLayout* layout = new QGridLayout(this);
    mLibLayout = new QVBoxLayout(this);
    mIncludeLayout = new QVBoxLayout(this);

    /* add main button and source directory field */
    layout->addWidget(injectCode, 0,0,1,-1, Qt::AlignBottom);
    layout->addWidget(mSourceDir, 1, 0, Qt::AlignBottom);

    /* add library section */
    mLibLayout->addWidget(mLibs.front());
    layout->addLayout(mLibLayout, 2, 0);
    layout->addWidget(addLib, 2, 1);

    /* add include path section */
    mIncludeLayout->addWidget(mIncludeDirs.front());
    layout->addLayout(mIncludeLayout, 3, 0);
    layout->addWidget(addInclude, 3, 1);    

    /* set layout */
    setLayout(layout);
}

/* add another slot for a library */
void InjectTab::addLibPath()
{
    /* insert new slot */
    std::string label = "Library " + std::to_string(mLibs.size() + 1);
    mLibs.push_back(new PathInput(QString::fromStdString(label),
        this, false, "Static Library (*.a)"));

    /* move button and add new library slot */
    mLibLayout->addWidget(mLibs.back());
}

/* add another slot for an include directory */
void InjectTab::addIncludeDir()
{
    /* insert new slot */
    std::string label = "Include Path "
        + std::to_string(mIncludeDirs.size() + 1);

    mIncludeDirs.push_back(new PathInput(
        QString::fromStdString(label), this));

    /* move button and add new include directory slot */
    mIncludeLayout->addWidget(mIncludeDirs.back());
}

/* inject the code into the iso */
void InjectTab::createISO()
{
    /* get code source directory */
/*    std::string source_dir = m_source_dir->text();
    std::vector<std::string> libs, include_dirs;

    /* get library names */
/*    for (auto it = m_libraries.begin(); it != m_libraries.end(); ++it) {

        /* only consider fields that contain content */        
    /*    if ((*it)->text().length() > 0) {

            libs.push_back((*it)->text());

        }

    }

    /* get include directories */
/*    for (auto it = m_include_dirs.begin(); it != m_include_dirs.end(); ++it) {

        /* only consider fields that contain content */
/*        if ((*it)->text().length() > 0) {

            include_dirs.push_back((*it)->text());

        }

    }

    /* call function to compile and inject code */    
/*    GCI::CreateISO(source_dir,
                   m_mem_config_tab->getConfig(),
                   include_dirs,
                   libs,
                   m_iso_path->text());

    //TODO: handle failure
    /* pop up message box for success */
//    QMessageBox mb (QMessageBox::Information, "GCI", "ISO Succesfully Created!");
//    mb.exec();    

}

