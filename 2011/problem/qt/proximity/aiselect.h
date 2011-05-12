#ifndef AISELECT_H
#define AISELECT_H

#include <QDialog>
#include <QtGui>
#include <QVector>

class AiSelect : public QDialog
{
public:
    AiSelect(QWidget *parent);
    QGridLayout *gridLayout;
    QListWidget *lwAis;
    QDialogButtonBox *bbBox;
    QVector<QString> execute(bool selectFromDb = false, const int &competition_id = 0);
};

#endif // AISELECT_H
