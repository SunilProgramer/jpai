#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "definition.h"
#include "drawer.h"
#include "map.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Drawer *d;
    Map map;
};

#endif // MAINWINDOW_H
