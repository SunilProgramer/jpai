#include "matchdialog.h"
#include "map.h"
#include "directorymanager.h"
#include "definition.h"
#include "aiselect.h"
#include <QSqlQuery>


MatchDialog::MatchDialog(QWidget *parent, const int &competitionid, const int &match_id): QDialog(parent, Qt::Dialog),
    CompetitionId(competitionid), MatchId(match_id)
{
    if (objectName().isEmpty())
        setObjectName(QString::fromUtf8("MatchDialog"));
    resize(393, 300);
    gridLayout = new QGridLayout(this);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    lMap = new QLabel(this);
    lMap->setObjectName(QString::fromUtf8("lMap"));

    gridLayout->addWidget(lMap, 0, 0, 1, 1);

    cbMaps = new QComboBox(this);
    cbMaps->setObjectName(QString::fromUtf8("cbMaps"));
    QSqlQuery query;
    query.prepare("select map_name from matches where match_id = :match_id");
    query.bindValue(":match_id", MatchId);
    query.exec();
    query.first();

    QDir d = DirectoryManager::Instance()->Directory(DIRECTORY_MAPS);
    QStringList l = d.entryList(QStringList(QString("*.map")), QDir::Files);
    while (!l.isEmpty())
    {
        QVector<int> v = Map::Preprocess(d.filePath(l.front()));
        //QString s = l.front() + ;
        cbMaps->addItem(QString("%1 %2x%3").arg(l.front()).arg(v[0]).arg(v[1]), l.front());
        if (l.front() == query.value(0))
        {
            cbMaps->setCurrentIndex(cbMaps->count() - 1);
        }
        l.pop_front();
    }


    gridLayout->addWidget(cbMaps, 0, 1, 1, 2);

    lPlayers = new QLabel(this);
    lPlayers->setObjectName(QString::fromUtf8("lPlayers"));

    gridLayout->addWidget(lPlayers, 1, 0, 1, 1);

    pbAdd = new QPushButton(this);
    pbAdd->setObjectName(QString::fromUtf8("pbAdd"));

    gridLayout->addWidget(pbAdd, 1, 1, 1, 1);

    pushButton_2 = new QPushButton(this);
    pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
    connect(pbAdd, SIGNAL(clicked()), this, SLOT(AddAi()));
    connect(pushButton_2, SIGNAL(clicked()), this, SLOT(RemoveAi()));

    gridLayout->addWidget(pushButton_2, 1, 2, 1, 1);

    lwAis = new QListWidget(this);
    lwAis->setObjectName(QString::fromUtf8("lwAis"));
    lwAis->setSelectionMode(QAbstractItemView::MultiSelection);
    lwAis->setDragDropMode(QAbstractItemView::InternalMove);

    query.prepare("select cp.id, cp.player_name, cp.player from competition_players as cp "
                  "inner join player_competition_matches as pcm on cp.id = pcm.competition_players_id where pcm.match_id = :match_id");
    query.bindValue(":match_id", MatchId);
    query.exec();
    while (query.next())
        lwAis->addItem(QString("%1:%2 %3").arg(query.value(0).toInt()).arg(query.value(1).toString()).arg(query.value(2).toBool()?"Human":"Comp"));

    gridLayout->addWidget(lwAis, 2, 1, 1, 2);

    buttonBox = new QDialogButtonBox(this);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    gridLayout->addWidget(buttonBox, 3, 0, 1, 3);


    setWindowTitle(QApplication::translate("MatchDialog", "\320\234\320\260\321\202\321\207", 0, QApplication::UnicodeUTF8));
    lMap->setText(QApplication::translate("MatchDialog", "\320\232\320\260\321\200\321\202\320\260", 0, QApplication::UnicodeUTF8));
    lPlayers->setText(QApplication::translate("MatchDialog", "\320\230\320\263\321\200\320\276\320\272\320\270", 0, QApplication::UnicodeUTF8));
    pbAdd->setText(QApplication::translate("MatchDialog", "+", 0, QApplication::UnicodeUTF8));
    pushButton_2->setText(QApplication::translate("MatchDialog", "-", 0, QApplication::UnicodeUTF8));    //retranslateUi(this);

    QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    QMetaObject::connectSlotsByName(this);
}

void MatchDialog::AddAi()
{
    AiSelect *aiselect = new AiSelect(this);
    QVector<QString> ais = aiselect->execute(true, CompetitionId);
    while (!ais.empty())
    {
        lwAis->addItem(ais.front());
        ais.pop_front();
    }
}

void MatchDialog::RemoveAi()
{
    QList<QListWidgetItem *> l = lwAis->selectedItems();
    while (!l.isEmpty())
    {
        lwAis->takeItem(lwAis->row(l.front()));
        l.pop_front();
    }

}
