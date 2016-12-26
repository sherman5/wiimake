#include "GUI_SaveLoad.h"

#include <QFileDialog>
#include <QVBoxLayout>

#include "ISOhandler.h"
#include "GCI.h"

SaveLoadTab::SaveLoadTab(PathInput* iso, MemConfigTab* mem, QWidget* parent) : QWidget(parent) {

    /* store iso path location and memory configuration */
    m_iso_path = iso;
    m_mem_config = mem;

    /* create save and load buttons */
    m_save_button = new QPushButton("SAVE", this);
    m_load_button = new QPushButton("LOAD", this);

    /* connect buttons to functions */
    connect(m_save_button, SIGNAL(clicked()), this, SLOT(SaveFile()));
    connect(m_load_button, SIGNAL(clicked()), this, SLOT(LoadFile()));
    
    /* create layout */
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(m_save_button);
    layout->addWidget(m_load_button);
    setLayout(layout);

}

void SaveLoadTab::SaveFile() {

    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter("Data File (*.data)");
    dialog.setViewMode(QFileDialog::Detail);
    std::string file_name = dialog.selectedFiles().at(0).toStdString();
    GCI::SaveISO(m_iso_path->text(), m_mem_config->getConfig(), file_name);

}

void SaveLoadTab::LoadFile() {

    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter("Data File (*.data)");
    dialog.setViewMode(QFileDialog::Detail);
    std::string file_name = dialog.selectedFiles().at(0).toStdString();
    GCI::LoadISO(m_iso_path->text(), file_name);

}
