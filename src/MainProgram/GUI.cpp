#include <QApplication>
#include <QMessageBox>

#include "../GUI/MainWindow.h"

/* class to handle exception throwing */
class GUI Q_DECL_FINAL : public QApplication
{

public:

    GUI(int& argc, char** argv) : QApplication(argc, argv) {}

    /* override defualt notify function */
    bool notify(QObject* receiver, QEvent* event) Q_DECL_OVERRIDE
    {
        /* catch and display exceptions */
        try
        {
            return QApplication::notify(receiver, event);
        }
        catch (std::exception& e)
        {
            QMessageBox mb (QMessageBox::Information, "ERROR", e.what());
            mb.exec();    
        }

        return false;
    }
};

/* GUI */
int main(int argc, char **argv)
{
    GUI app (argc, argv);
 
    MainWindow* mainWindow = new MainWindow();
    mainWindow->show();

    return app.exec();
}
