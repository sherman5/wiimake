#include <QMessageBox>

#include "GUIwindow.h"
#include "RegionFileParser.h"
#include "ISOhandler.h"
#include "CodeAssembler.h"

/* constructor */
GUIwindow::GUIwindow() {

    /* construct the window */
    m_window = new QWidget();

    /* Set size of the window */
    m_window->resize(500, 400);
 
    /* populate the default fields of the window */
    m_input_fields.push_back(new PathInput ("ISO File:", m_window, false, "ISO File (*.iso)"));
    m_input_fields.push_back(new PathInput ("Region File:", m_window, false, "Text File (*.txt)"));
    m_input_fields.push_back(new PathInput ("C Source Code Directory:", m_window));
    m_input_fields.push_back(new PathInput ("Libraries:", m_window, false, "Static Lib (*.a)"));
    m_input_fields.push_back(new PathInput ("Include Directories:", m_window));

    /* add buttons to allow for additional paths; create main button*/
    m_add_lib_button = new QPushButton("+", m_window);
    m_add_include_button = new QPushButton("+", m_window);
    m_main_button = new QPushButton("Create ISO", m_window);

    /* connect buttons with appropiate functions */
    QObject::connect(m_add_lib_button, SIGNAL(clicked()), this, SLOT(AddLibPath()));
    QObject::connect(m_add_include_button, SIGNAL(clicked()), this, SLOT(AddIncludeDir())); 
    QObject::connect(m_main_button, SIGNAL(clicked()), this, SLOT(RunProgram())); 

    /* keep track of where the buttons are */
    m_lib_index = 3;
    m_include_index = 4;

    /* populate with empty layout, need something to replace */
    m_layout = new QGridLayout();

}

/* create layout of window */
void GUIwindow::CreateLayout() {

    /* delete previous layout and create new empty one */
    delete m_layout;
    m_layout = new QGridLayout();

    /* populate layout with all input fields */
    for (unsigned int i = 0; i < m_input_fields.size(); ++i) {

        m_input_fields[i]->addToLayout(m_layout, i, 0);

    }

    /* add buttons to layout */
    m_layout->addWidget(m_add_lib_button, m_lib_index, 4);
    m_layout->addWidget(m_add_include_button, m_include_index, 4);
    m_layout->addWidget(m_main_button, m_include_index + 1, 2);

    /* set window layout */
    m_window->setLayout(m_layout);

}

/* add another slot for a library */
void GUIwindow::AddLibPath() {

    /* insert new slot */
    m_input_fields.insert(m_input_fields.begin() + ++m_lib_index, new PathInput("", m_window));

    /* increment slot position */
    ++m_include_index;

    /* show new window */
    show();

}

/* add another slot for an include directory */
void GUIwindow::AddIncludeDir() {

    /* insert new slot */
    m_input_fields.insert(m_input_fields.begin() + ++m_include_index, new PathInput("", m_window));

    /* show new window */
    show();

}

/* show window */
void GUIwindow::show() {

    /* update layout */
    CreateLayout();

    /* show window */
    m_window->show();

}

void GUIwindow::RunProgram() {

    std::string iso_file = m_input_fields[0]->text();
    std::string region_file = m_input_fields[1]->text();
    std::string source_dir = m_input_fields[2]->text();
    std::vector<std::string> libs, include_dirs;

    for (unsigned int i = 3; i <= m_lib_index; ++i) {

        if (m_input_fields[i]->text().length() > 0) {

            libs.push_back(m_input_fields[i]->text());

        }

    }

    for (unsigned int i = m_lib_index + 1; i <= m_include_index; ++i) {

        if (m_input_fields[i]->text().length() > 0) {

            include_dirs.push_back(m_input_fields[i]->text());

        }

    }

    ISOhandler iso (iso_file);
    CodeAssembler code (source_dir, region_file, include_dirs, libs);
    iso.InjectCode(code.GetRawASM());
//    code.CleanDirectory();

    RegionFileParser parser (region_file);
    uint32_t replace_addr = (*parser.begin()).first + 0x04;
    iso.IsoWrite(replace_addr, parser.GetInjectionInstruction());

    QMessageBox mb (QMessageBox::Information, "GCI", "ISO Succesfully Created!");
    mb.exec();    

}

