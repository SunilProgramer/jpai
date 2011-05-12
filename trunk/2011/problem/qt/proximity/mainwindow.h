#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "definition.h"
#include "drawer.h"
#include "mapdrawer.h"
#include "aihandler.h"
#include "initmanager.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void EditMatch();

private:
    Ui::MainWindow *ui;
    Drawer *d;
//    MapHandler map;
    InitManager im;
};

#endif // MAINWINDOW_H
