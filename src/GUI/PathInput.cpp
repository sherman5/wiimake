#include "PathInput.h"

#include <QHBoxLayout>

/* constructor */
PathInput::PathInput(QString lab, QWidget* parent, bool dir,
QString fileType) : QWidget(parent)
{
    /* create input line and label */
    mInput = new QLineEdit(this);
    mLabel = new QLabel(lab, this);

    /* attach label to input line */
    mLabel->setBuddy(mInput);

    /* create button to search for files */
    mButton = new QPushButton("...", this);

    /* create file dialog object; set file type filter */
    mFileBrowser = new QFileDialog(this);
    mFileBrowser->setNameFilter(fileType);        

    /* set file mode to either look for a directory or a file */
    if (dir) { mFileBrowser->setFileMode(QFileDialog::DirectoryOnly);}
    else { mFileBrowser->setFileMode(QFileDialog::ExistingFile);}

    /* connect button to file browser */       
    connect(mButton, SIGNAL(clicked()), this, SLOT(findFile()));       

    /* create layout */
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(mLabel);
    layout->addWidget(mInput);
    layout->addWidget(mButton);
    setLayout(layout);   
}

/* return text in input line */
std::string PathInput::text()
{
    return mInput->text().toStdString(); 
}

/* browse for a file and store in input line */
void PathInput::findFile()
{
    /* open file browser */
    mFileBrowser->exec();

    /* check if no file was selected */
    if (!mFileBrowser->selectedFiles().isEmpty())
    {
        /* store selected file/directory */
        mInput->insert(mFileBrowser->selectedFiles().at(0));

        /* send signal */
        emit valueChanged();        
    }
}


