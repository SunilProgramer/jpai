#ifndef COMPETITIONDIALOG_H
#define COMPETITIONDIALOG_H

#include <QDialog>

namespace Ui {
    class competitiondialog;
}

class CompetitionDialog : public QDialog {
    Q_OBJECT
public:
    CompetitionDialog(QWidget *parent = 0);
    ~CompetitionDialog();

    Ui::competitiondialog *GetUi();
    int PlayersCount();
    int MapsCount();
    QString Name();
    QString Player(int i);
    QString Map(int i);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::competitiondialog *ui;

private slots:
    void on_pbRemMap_clicked();
    void on_pbAddMap_clicked();
    void on_pbRemPlayer_clicked();
    void on_pbAddPlayer_clicked();
};

#endif // COMPETITIONDIALOG_H
