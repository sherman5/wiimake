#ifndef GUI_CUSTOM_OBJECTS_H
#define GUI_CUSTOM_OBJECTS_H

#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>

/* input line that accepts file/directory path; can search in file browser */
class PathInput : public QWidget {

    Q_OBJECT

private:

    /* components of the file input line */
    QLineEdit* m_input;
    QLabel* m_label;
    QPushButton* m_button;
    QFileDialog* m_file_browser;

public:

    /* constructor */
    PathInput(QString, QWidget*, bool=true, QString="");

    /* return text in input line */
    std::string text();

public slots:

    /* browse for a file and store in input line */
    void FindFile();

};

#endif
