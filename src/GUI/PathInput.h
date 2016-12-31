#ifndef PATH_INPUT_H
#define PATH_INPUT_H

#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>

/* input line that accepts file/directory path; can search in file browser */
class PathInput : public QWidget
{
    Q_OBJECT

private:

    /* components of the file input line */
    QLineEdit* mInput;
    QLabel* mLabel;
    QPushButton* mButton;
    QFileDialog* mFileBrowser;

public:

    /* constructor */
    PathInput(QString, QWidget*, bool=true, QString="");

    /* return text in input line */
    std::string text();

public slots:

    /* browse for a file and store in input line */
    void findFile();
};

#endif
