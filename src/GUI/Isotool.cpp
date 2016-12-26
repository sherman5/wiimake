#include "GUI_Isotool.h"

#include <stdint.h>
#include <string>
#include <sstream>
#include <exception>

#include <QString>

#include "ISOhandler.h"
#include "GCI.h"

IsotoolTab::IsotoolTab(PathInput* iso, QWidget* parent) : QWidget(parent) {

    m_iso_path = iso;

    m_addr_input = new QLineEdit(this);
    m_label1 = new QLabel("Address:", this);
    m_label1->setBuddy(m_addr_input);

    m_addr_value = new QLineEdit(this);
    m_label2 = new QLabel("Value:", this);
    m_label2->setBuddy(m_addr_value);
    m_addr_value->setReadOnly(true);

    QHBoxLayout* layout = new QHBoxLayout();
    layout->addWidget(m_label1);
    layout->addWidget(m_addr_input);
    layout->addWidget(m_label2);
    layout->addWidget(m_addr_value);

    connect(m_addr_input, SIGNAL(editingFinished()), this, SLOT(ReadAddress()));

    setLayout(layout);

}

void IsotoolTab::ReadAddress() {

    try {

        ISOhandler iso(m_iso_path->text());
        std::string val = GCI::ReadAddr(iso, m_addr_input->text().toStdString()); 
        m_addr_value->setPlaceholderText(QString::fromStdString(val));
   
    } catch (std::invalid_argument& e) {

        if (!m_addr_input->text().isEmpty()) {

            m_addr_value->setPlaceholderText("Invalid Address");

        }

    }

}
