#include "ui-components-qt.h"

#include <QApplication>
#include <QMainWindow>
#include <QPushButton>

#include <iostream>

UiComponentsQt::UiComponentsQt() = default;
UiComponentsQt::~UiComponentsQt() = default;

void UiComponentsQt::init(int argc, char* argv[])
{
    _app = std::make_unique<QApplication>(argc, argv);
}

void UiComponentsQt::createWindow()
{
    auto window = std::make_shared<QMainWindow>();
    window->show();

    auto button = new QPushButton();
    QObject::connect(button, &QPushButton::clicked,
                     []() {
        std::cout << "Hello World!" << std::endl;
    });
    window->setCentralWidget(button);

    _windows.push_back(window);
}

void UiComponentsQt::run()
{
    _app->exec();
}

