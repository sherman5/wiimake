#ifndef GUI_MEM_CONFIG_H
#define GUI_MEM_CONFIG_H

#include <QWidget>
#include <QObject>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "GUI_CustomObjects.h"
#include "MemoryConfig.h"

/* object to manage adding memory ranges */
class AddressRange : public QWidget {

    Q_OBJECT

private:

    QLineEdit *m_begin, *m_end;
    QLabel *m_begin_lab, *m_end_lab;

public:

    AddressRange(QString lab, QString begin = "", QString end = "", QWidget* parent = Q_NULLPTR) {

        m_begin = new QLineEdit(begin, this);
        m_end = new QLineEdit(end, this);

        m_begin_lab = new QLabel(lab, this);
        m_end_lab = new QLabel(" - ", this);

        QHBoxLayout* layout = new QHBoxLayout(this);
        layout->addWidget(m_begin_lab);
        layout->addWidget(m_begin);
        layout->addWidget(m_end_lab);
        layout->addWidget(m_end);

        setLayout(layout);

    }

};

/* tab - manages the memory layout of the iso */
class MemConfigTab : public QWidget {

    Q_OBJECT

private:

    QLineEdit *m_inject_addr, *m_inject_inst;
    QLabel *m_inject_addr_lab, *m_inject_inst_lab;
    QVBoxLayout *m_layout, *m_region_layout;
    QPushButton *m_add_button, *m_del_button;

    std::vector<AddressRange*> m_regions;

public:

    MemConfigTab(QWidget* = Q_NULLPTR);

    MemoryConfig getConfig();

public slots:

    void AddRegion();
    void DeleteRegion();

};
 
#endif

