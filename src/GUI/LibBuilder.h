#ifndef GUI_LIB_BUILDER_H
#define GUI_LIB_BUILDER_H

#include <QWidget>
#include <QObject>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

#include "GUI_CustomObjects.h"

class LibBuilderTab : public QWidget {

    Q_OBJECT

private:

    PathInput* m_source_dir;
    QLineEdit* m_name;
    QLabel* m_name_label;
    QPushButton* m_create_button;

public:

    LibBuilderTab(QWidget* = Q_NULLPTR);

public slots:

    void CreateLibrary();

};

#endif
