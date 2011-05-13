#include "aiselect.h"
#include "directorymanager.h"
#include "definition.h"
#include <QSqlQuery>

AiSelect::AiSelect(QWidget *parent) : QDialog(parent, Qt::Dialog)
{
    if (objectName().isEmpty())
        setObjectName(QString::fromUtf8("AiSelect"));
    resize(400, 300);
    gridLayout = new QGridLayout(this);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));

    lwAis = new QListWidget(this);
    lwAis->setObjectName(QString::fromUtf8("lwAis"));
    lwAis->setSelectionMode(QAbstractItemView::MultiSelection);

    gridLayout->addWidget(lwAis, 0, 0, 1, 1);
    // select from current competition

    bbBox = new QDialogButtonBox(this);
    bbBox->setObjectName(QString::fromUtf8("bbBox"));
    bbBox->setOrientation(Qt::Horizontal);
    bbBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    gridLayout->addWidget(bbBox, 1, 0, 1, 1);


    //retranslateUi(Dialog);
    QObject::connect(bbBox, SIGNAL(accepted()), this, SLOT(accept()));
    QObject::connect(bbBox, SIGNAL(rejected()), this, SLOT(reject()));

    QMetaObject::connectSlotsByName(this);
}

QVector<QString> AiSelect::execute(bool selectFromDb, const int &competition_id)
{
    if (!selectFromDb)
    {
        if (competition_id == -1)
        {
            QDir d = DirectoryManager::Instance()->Directory(DIRECTORY_MAPS);
            QStringList l = d.entryList(QStringList(QString("*.map")), QDir::Files);
            while (!l.isEmpty())
            {
                lwAis->addItem(l.front());
                l.pop_front();
            }
        }
        else
        {
            lwAis->addItem("<Player>");
            QDir d = DirectoryManager::Instance()->Directory(DIRECTORY_AIS);
            QStringList l = d.entryList(QDir::Executable|QDir::Files);
            while (!l.isEmpty())
            {
                lwAis->addItem(l.front());
                l.pop_front();
            }
        }
    }
    else
    {
        QSqlQuery q;
        q.setForwardOnly(true);
        if (competition_id == 0)
            q.exec("select * from competition_players where competition_id is NULL");
        else
        {
            q.prepare("select * from competition_players where competition_id = :competition_id");
            q.bindValue(":competition_id", competition_id);
            q.exec();
        }
        while (q.next())
        {
            lwAis->addItem(QString("%1:%2 %3").arg(q.value(0).toInt()).arg(q.value(2).toString()).arg(q.value(3).toBool()?"Human":"Comp"));
        }
    }
    QVector<QString> res;
    if (!exec())
        return res;
    QList<QListWidgetItem *> ql = lwAis->selectedItems();
    while (!ql.isEmpty())
    {
        res.push_back(ql.front()->text());
        ql.pop_front();
    }
    return res;

}
