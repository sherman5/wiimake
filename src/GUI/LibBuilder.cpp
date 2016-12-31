#include "LibBuilder.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "../MainProgram/GCI.h"

LibBuilderTab::LibBuilderTab(QWidget* parent) : QWidget(parent)
{
    /* C source directory */
    mSourceDir = new PathInput("C Source Code &Directory", this);

    /* library name */
    mName = new QLineEdit(this);
    QLabel* label = new QLabel("Library &Name", this);
    label->setBuddy(mName);

    /* button to create library */
    QPushButton* createButton = new QPushButton("Create Library", this);
    connect(createButton, SIGNAL(clicked()), this, SLOT(createLibrary()));

    /* create layout */
    QVBoxLayout* layout = new QVBoxLayout(this);
    QHBoxLayout* nameLayout = new QHBoxLayout(this);
    nameLayout->addWidget(label);
    nameLayout->addWidget(mName);

    layout->addWidget(mSourceDir);
    layout->addLayout(nameLayout);
    layout->addWidget(createButton);

    setLayout(layout);
}

void LibBuilderTab::createLibrary()
{
    /* get arguments */
    Arguments args;

    args.cmdOptions.insert(std::make_pair("--ar",
        mSourceDir->text()));

    args.cmdOptions.insert(std::make_pair("--output",
        mName->text().toStdString()));
    
    /* create library */
    GCI::createLibrary(args);
}
