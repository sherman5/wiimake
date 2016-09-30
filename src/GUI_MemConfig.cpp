#include "GUI_MemConfig.h"

#include <QHBoxLayout>
#include <QString>

/* constructor */
MemConfigTab::MemConfigTab(QWidget* parent) : QWidget(parent) {

    /* create line for inject address */
    m_inject_addr = new QLineEdit("80377998", this);
    m_inject_addr_lab = new QLabel("&Code Injection Address", this);
    m_inject_addr_lab->setBuddy(m_inject_addr);

    /* create line for original ASM code to overwrite */
    m_inject_inst = new QLineEdit("7ee3bb78", this);
    m_inject_inst_lab = new QLabel("&Original ASM code (32-bit hex)", this);
    m_inject_inst_lab->setBuddy(m_inject_inst);

    /* create the default 4 regions */
    m_regions.push_back(new AddressRange("Region 1", "803fa3e8", "803fc2ec", this));
    m_regions.push_back(new AddressRange("Region 2", "803fc420", "803fdc1c", this));
    m_regions.push_back(new AddressRange("Region 3", "801910e0", "80192930", this));
    m_regions.push_back(new AddressRange("Region 4", "803001dc", "80301e40", this));

    /* create buttons to add/remove memory regions */
    m_add_button = new QPushButton("+", this);
    m_del_button = new QPushButton("-", this);
    connect(m_add_button, SIGNAL(clicked()), this, SLOT(AddRegion()));
    connect(m_del_button, SIGNAL(clicked()), this, SLOT(DeleteRegion()));
    
    /* create layout for inject address */
    QHBoxLayout* layout_1 = new QHBoxLayout();
    layout_1->addWidget(m_inject_addr_lab);
    layout_1->addWidget(m_inject_addr);

    /* create layout for original ASM code */
    QHBoxLayout* layout_2 = new QHBoxLayout();
    layout_2->addWidget(m_inject_inst_lab);
    layout_2->addWidget(m_inject_inst);

    /* create the layout for the memory region section */
    m_region_layout = new QVBoxLayout();
    m_region_layout->addWidget(m_regions[0]);
    m_region_layout->addWidget(m_regions[1]);
    m_region_layout->addWidget(m_regions[2]);
    m_region_layout->addWidget(m_regions[3]);

    /* create the layout for the buttons */
    QHBoxLayout* button_layout = new QHBoxLayout();
    button_layout->addWidget(m_del_button);
    button_layout->addWidget(m_add_button);

    /* add all the components to the main layout */
    m_layout = new QVBoxLayout(this);    
    m_layout->addLayout(layout_1);
    m_layout->addLayout(layout_2);
    m_layout->addLayout(m_region_layout);
    m_layout->addLayout(button_layout);
    
    /* set main layout */    
    setLayout(m_layout);    

}

MemoryConfig MemConfigTab::getConfig() {


}

/* add a slot for another memory region */
void MemConfigTab::AddRegion() {

    /* create label and add to list of regions */
    std::string label = "Region " + std::to_string(m_regions.size() + 1);
    m_regions.push_back(new AddressRange(QString::fromStdString(label)));

    /* add new region to layout */
    m_region_layout->addWidget(m_regions.back());

}

/* remove a  memory region slot */
void MemConfigTab::DeleteRegion() {

    /* always need at least one region */
    if (m_regions.size() > 1) {

        /* delete from layout, then free memory, then delete from vector */
        m_region_layout->removeWidget(m_regions.back()); 
        delete m_regions.back();
        m_regions.pop_back();
 
    }

}
