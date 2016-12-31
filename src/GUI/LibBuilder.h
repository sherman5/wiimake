#ifndef LIB_BUILDER_H
#define LIB_BUILDER_H

#include <QWidget>
#include <QObject>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

#include "PathInput.h"

class LibBuilderTab : public QWidget
{
    Q_OBJECT

private:

    /* C source directory */
    PathInput* mSourceDir;

    /* library name */
    QLineEdit* mName;

public:

    /* constructor */
    LibBuilderTab(QWidget* = Q_NULLPTR);

public slots:

    /* create library */
    void createLibrary();
};

#endif
