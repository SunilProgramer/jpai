#ifndef MATCHDIALOG_H
#define MATCHDIALOG_H

#include <QDialog>
#include <QtGui>

class MatchDialog : public QDialog
{
    Q_OBJECT
public:
    MatchDialog(QWidget *parent, const int &competitionid, const int &match_id);
    QGridLayout *gridLayout;
    QLabel *lMap;
    QComboBox *cbMaps;
    QLabel *lPlayers;
    QPushButton *pbAdd;
    QPushButton *pushButton_2;
    QListWidget *lwAis;
    QDialogButtonBox *buttonBox;
protected:
    int CompetitionId;
    int MatchId;
public slots:
    void AddAi();
    void RemoveAi();
};

#endif // MATCHDIALOG_H
