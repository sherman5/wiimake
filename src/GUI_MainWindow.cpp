#include "GUI_MainWindow.h"

#include <QVBoxLayout>

#include "GUI_MemConfig.h"
#include "GUI_Inject.h"
#include "GUI_SaveLoad.h"
#include "GUI_Isotool.h"
#include "GUI_LibBuilder.h"

MainWindow::MainWindow() : QWidget() {

    m_iso_path = new PathInput("iso file:", this, false, "ISO File (*.iso)");

    m_tabs = new QTabWidget();

    MemConfigTab* mem_config_tab = new MemConfigTab();
    
    m_tabs->addTab(mem_config_tab, "Memory Configuration");
    m_tabs->addTab(new InjectTab(m_iso_path, mem_config_tab), "Inject C Code");
    m_tabs->addTab(new SaveLoadTab(m_iso_path, mem_config_tab), "Save/Load ISO State");
    m_tabs->addTab(new IsotoolTab(m_iso_path), "ISO Info");
    m_tabs->addTab(new LibBuilderTab(), "Create Library");

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(m_iso_path);
    layout->addWidget(m_tabs);
  
    setLayout(layout);
    setWindowTitle("GCI");
    resize(750, 400);

}

std::string MainWindow::GetIsoPath() {

    return m_iso_path->text();

}

