#include "IsoInfo.h"

#include <stdint.h>
#include <string>
#include <sstream>
#include <exception>

#include <QString>

#include "../MainProgram/GCI.h"

/* constructor */
IsoInfoTab::IsoInfoTab(MainWindow* parent) : QWidget(parent)
{
    /* store parent window */
    mMainWindow = parent;

    /* address input */
    mAddrInput = new QLineEdit(this);
    QLabel* addrLabel = new QLabel("&Address:", this);
    addrLabel->setBuddy(mAddrInput);

    /* value at address */
    mAddrValue = new QLineEdit(this);
    QLabel* valueLabel = new QLabel("Value:", this);
    valueLabel->setBuddy(mAddrValue);
    mAddrValue->setReadOnly(true);

    /* connect fields */
    connect(mAddrInput, SIGNAL(editingFinished()), this,
        SLOT(readAddress()));

    /* create layout */
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(addrLabel);
    layout->addWidget(mAddrInput);
    layout->addWidget(valueLabel);
    layout->addWidget(mAddrValue);

    /* set layout */
    setLayout(layout);
}

/* read address given by user */
void IsoInfoTab::readAddress()
{
    /* get address */
    std::string address = mAddrInput->text().toStdString();

    /* get arguments */ 
    Arguments args = mMainWindow->getArgs();
    args.cmdOptions.insert(std::make_pair("--read", address));
    
    /* try to read address */
    try
    {
        /* get value and convert to string */
        uint32_t val = GCI::readAddress(args);
        std::stringstream ss;
        ss << std::hex << val;

        /* put value in field */
        mAddrValue->setPlaceholderText(QString::fromStdString(ss.str()));
    }
    catch (std::invalid_argument& e)
    {
        if (!mAddrInput->text().isEmpty())
        {
            mAddrValue->setPlaceholderText("Invalid Address");
        }
    }
}
