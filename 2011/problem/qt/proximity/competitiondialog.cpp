#include "competitiondialog.h"
#include "ui_competitiondialog.h"
#include "aiselect.h"

CompetitionDialog::CompetitionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::competitiondialog)
{
    ui->setupUi(this);
}

CompetitionDialog::~CompetitionDialog()
{
    delete ui;
}

void CompetitionDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void CompetitionDialog::on_pbAddPlayer_clicked()
{
    AiSelect *aiselect = new AiSelect(this);
    QVector<QString> ais = aiselect->execute(false, 0);
    while (!ais.empty())
    {
        ui->lwPlayers->addItem(ais.front());
        ais.pop_front();
    }
}

void CompetitionDialog::on_pbRemPlayer_clicked()
{
    QList<QListWidgetItem *> l = ui->lwPlayers->selectedItems();
    while (!l.isEmpty())
    {
        ui->lwPlayers->takeItem(ui->lwPlayers->row(l.front()));
        l.pop_front();
    }
}

void CompetitionDialog::on_pbAddMap_clicked()
{
    AiSelect *aiselect = new AiSelect(this);
    QVector<QString> ais = aiselect->execute(false, -1);
    while (!ais.empty())
    {
        ui->lwMaps->addItem(ais.front());
        ais.pop_front();
    }
}

void CompetitionDialog::on_pbRemMap_clicked()
{
    QList<QListWidgetItem *> l = ui->lwMaps->selectedItems();
    while (!l.isEmpty())
    {
        ui->lwMaps->takeItem(ui->lwMaps->row(l.front()));
        l.pop_front();
    }
}


Ui::competitiondialog *CompetitionDialog::GetUi()
{
    return ui;
}
int CompetitionDialog::PlayersCount()
{
    return ui->lwPlayers->count();
}
int CompetitionDialog::MapsCount()
{
    return ui->lwMaps->count();
}
QString CompetitionDialog::Map(int i)
{
    return ui->lwMaps->item(i)->text();
}
QString CompetitionDialog::Player(int i)
{
    return ui->lwPlayers->item(i)->text();
}
QString CompetitionDialog::Name()
{
    return ui->leName->text();
}
