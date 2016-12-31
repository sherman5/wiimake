#ifndef ISO_INFO_H
#define ISO_INFO_H

#include <vector>

#include <QWidget>
#include <QObject>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>

#include "MainWindow.h"
#include "PathInput.h"

/* tab - manages iso information */
class IsoInfoTab : public QWidget
{
    Q_OBJECT

private:
    
    /* parent window */
    MainWindow* mMainWindow;

    /* address and value */
    QLineEdit *mAddrInput, *mAddrValue;

public:

    /* constructor */
    IsoInfoTab(MainWindow*);

public slots:

    /* read address given by user */
    void readAddress();
};
 
#endif
