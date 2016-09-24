#ifndef GUI_WINDOW_H
#define GUI_WINDOW_H

#include <vector>
 
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QString> 
#include <QPushButton>
#include <QGridLayout>
#include <QObject>
#include <QFileDialog>

class PathInput : QObject {

    Q_OBJECT

private:

    QLineEdit* m_input;
    QLabel* m_label;
    QPushButton* m_button;
    QFileDialog* m_file_browser;

public:

    PathInput(QString lab, QWidget* parent, bool dir=true, QString file_type="") {

        m_input = new QLineEdit(parent);
        m_label = new QLabel(lab, parent);
        m_label->setBuddy(m_input);
        m_button = new QPushButton("...", parent);
        m_file_browser = new QFileDialog(parent);
        if (dir) { m_file_browser->setFileMode(QFileDialog::DirectoryOnly);}
        else { m_file_browser->setFileMode(QFileDialog::ExistingFile);}
        m_file_browser->setNameFilter(file_type);
        QObject::connect(m_button, SIGNAL(clicked()), this, SLOT(FindFile()));       

    }

    void addToLayout(QGridLayout* layout, int r, int c) {

        layout->addWidget(m_label, r, c);
        layout->addWidget(m_input, r, c + 1);
        layout->addWidget(m_button, r, c + 2);

    }

    std::string text() {

        return m_input->text().toStdString(); 

    }

public slots:

    void FindFile() {

        m_file_browser->exec();
        m_input->insert(m_file_browser->selectedFiles().at(0));

    }

};

class GUIwindow : public QObject {

    Q_OBJECT

private:

    QWidget* m_window;
    std::vector<PathInput*> m_input_fields;
    QGridLayout* m_layout;
    QPushButton *m_add_lib_button, *m_add_include_button;
    int m_lib_index, m_include_index;
    QPushButton* m_main_button;

public:

    GUIwindow();
    
    void CreateLayout();
    void show();

public slots:

    void AddLibPath();
    void AddIncludeDir();
    void RunProgram();

};
 
#endif
