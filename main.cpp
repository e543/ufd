#include "Application.h"
#include <iostream>

int main(int argc, char* argv[])
{
    Application app(argc, argv);
    auto& w = app.getMainWindow();
    w.show();

    app.setAttribute(Qt::AA_DisableWindowContextHelpButton);

    try
    {
        return app.exec();
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "something failed" << std::endl;
    }
}
