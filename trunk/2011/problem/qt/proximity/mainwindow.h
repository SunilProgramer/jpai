#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "definition.h"
#include "drawer.h"
#include "mapdrawer.h"
#include "aihandler.h"
#include "initmanager.h"
#include <QSqlQueryModel>

namespace Ui {
    class MainWindow;
}

class ScoresViewer : public QSqlQueryModel
{
    Q_OBJECT

public:
    ScoresViewer(QObject *parent = 0);

    QVariant data(const QModelIndex &item, int role) const;

};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void EditMatch();
    void Stop();
    void UpdateScores();
protected:
    void showEvent(QShowEvent *event);
private:
    Ui::MainWindow *ui;
    Drawer *d;
    ScoresViewer *viewer;
//    MapHandler map;
    InitManager im;
};

#endif // MAINWINDOW_H
