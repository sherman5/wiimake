#include "GUI_LibBuilder.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "GCI.h"

LibBuilderTab::LibBuilderTab(QWidget* parent) : QWidget(parent) {

    m_source_dir = new PathInput("C Source Code &Directory", this);

    m_name = new QLineEdit(this);
    m_name_label = new QLabel("Library &Name", this);
    m_name_label->setBuddy(m_name);

    m_create_button = new QPushButton("Create Library", this);
    connect(m_create_button, SIGNAL(clicked()), this, SLOT(CreateLibrary()));

    QHBoxLayout* name_layout = new QHBoxLayout();
    name_layout->addWidget(m_name_label);
    name_layout->addWidget(m_name);
    
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(m_source_dir);
    layout->addLayout(name_layout);
    layout->addWidget(m_create_button);

    setLayout(layout);

}

void LibBuilderTab::CreateLibrary() {

    GCI::CreateLibrary(m_source_dir->text(), m_name->text().toStdString());

}
