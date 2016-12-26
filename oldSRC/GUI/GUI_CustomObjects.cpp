#include "GUI_CustomObjects.h"

#include <QHBoxLayout>

/********************** PathInput ************************/

/* constructor */
PathInput::PathInput(QString lab, QWidget* parent, bool dir, QString file_type) 
    : QWidget(parent) {

    /* create input line and label */
    m_input = new QLineEdit(this);
    m_label = new QLabel(lab, this);

    /* attach label to input line */
    m_label->setBuddy(m_input);

    /* create button to search for files */
    m_button = new QPushButton("...", this);

    /* create file dialog object; set file type filter */
    m_file_browser = new QFileDialog(this);
    m_file_browser->setNameFilter(file_type);        

    /* set file mode to either look for a directory or a file */
    if (dir) { m_file_browser->setFileMode(QFileDialog::DirectoryOnly);}
    else { m_file_browser->setFileMode(QFileDialog::ExistingFile);}

    /* connect button to file browser */       
    connect(m_button, SIGNAL(clicked()), this, SLOT(FindFile()));       

    /* create layout */
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(m_label);
    layout->addWidget(m_input);
    layout->addWidget(m_button);
    setLayout(layout);   

}

/* return text in input line */
std::string PathInput::text() {

    return m_input->text().toStdString(); 

}

/* browse for a file and store in input line */
void PathInput::FindFile() {

    /* open file browser */
    m_file_browser->exec();

    /* check if no file was selected */
    if (!m_file_browser->selectedFiles().isEmpty()) {

        /* store selected file/directory */
        m_input->insert(m_file_browser->selectedFiles().at(0));

    }

}

/*********************************************************/
