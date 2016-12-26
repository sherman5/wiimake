#ifndef GUI_ISOTOOL_H
#define GUI_ISOTOOL_H

#include <vector>

#include <QWidget>
#include <QObject>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>

#include "GUI_CustomObjects.h"

/* tab - manages iso information */
class IsotoolTab : public QWidget {

    Q_OBJECT

private:

    QLineEdit *m_addr_input, *m_addr_value;
    QLabel *m_label1, *m_label2;
    PathInput* m_iso_path;

public:

    IsotoolTab(PathInput*, QWidget* = Q_NULLPTR);

public slots:

    void ReadAddress();
    
};
 
#endif
