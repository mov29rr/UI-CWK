#pragma once

#include "window.hpp"

#include <memory>
#include <sstream>
#include <fstream>
#include <iostream>

class Application
{
private:
    static constexpr const char* STYLE_SHEET_FILE_PATH = "stylesheets/stylesheet.qss";

    std::unique_ptr<QApplication> _handle;
    std::unique_ptr<Window> _window;

    bool _errored = false;
public:
    Application(int argc, char** argv)
        : _handle(std::make_unique<QApplication>(argc, argv))
        , _window(std::make_unique<Window>())
    {
        try
        {
            // Reading stylesheet from file
            QFile file(STYLE_SHEET_FILE_PATH);
            if(!file.exists())
            {
                throw std::runtime_error("Failed to open stylesheet");
            }
            
            file.open(QFile::ReadOnly);
            auto contents = QLatin1String(file.readAll());

            _handle->setStyleSheet(contents);
            
            _window->show();
        }
        catch(const std::exception& exception)
        {
            std::cerr << "\033[0;31mError\033[0m: " << exception.what() << std::endl;
            _errored = true;
        }
    }

    int exec()
    {
        return _errored ? EXIT_FAILURE : _handle->exec();
    }
};