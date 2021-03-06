#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QMainWindow>
#include <qfile.h>
#include <qtextstream.h>
#include <QDebug>
#include <QRandomGenerator>
#include <qlistwidget.h>
#include <QSettings>
#include <QSignalMapper>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class mainWindow; }
QT_END_NAMESPACE

class mainWindow : public QMainWindow
{
    Q_OBJECT

public:
    mainWindow(QWidget *parent = nullptr);
    ~mainWindow();
    bool SetMutator(int list_id, int mutator_id);

private slots:
    void on_pushButton_clicked();

    void on_settingEnableMutatorList_itemDoubleClicked(QListWidgetItem *item);

    void on_settingDisableMutatorButton_clicked();

    void on_settingDisableMutatorList_itemDoubleClicked(QListWidgetItem *item);

    void on_settingEnableMutatorButton_clicked();

    void on_settingEnableCMutatorList_itemDoubleClicked(QListWidgetItem *item);

    void on_settingDisableCMutatorButton_clicked();

    void on_settingDisableCMutatorList_itemDoubleClicked(QListWidgetItem *item);

    void on_settingEnableCMutatorButton_clicked();

    void on_settingDisableAllMutatorCheckBox_toggled(bool checked);

    void on_settingDisableAllCMutatorCheckBox_toggled(bool checked);

    void on_settingBrutalPlusRandomCommanderCheckBox_toggled(bool checked);

    void on_settingBrutalPlusMutatorAmountSelector_currentIndexChanged(int index);

    void on_settingBrutalPlusMinMutatorScoreEditor_textChanged(const QString &arg1);

    void on_settingBrutalPlusMaxMutatorScoreEditor_textChanged(const QString &arg1);

    void on_settingCommanderEnableCheckBox_toggled(bool checked);

    void on_settingMapEnableCheckBox_toggled(bool checked);

    void on_settingBrutalPlusRandomAICheckBox_toggled(bool checked);

    void on_settingBrutalPlusRandomMapCheckBox_toggled(bool checked);

private:
    Ui::mainWindow *ui;
    QSettings *settings;

    QCheckBox* settingCommanderCheckbox[18];
    QCheckBox* settingMapCheckbox[15];

    QWidget* brutalPlusMutatorIcon[10];
    QLabel* brutalPlusMutatorName[10];
    QLabel* brutalPlusMutatorScore[10];

    //Database
    //Commander
    QList<QStringList> commanderInfo;//0=commander id, 1=commander name, 2=v1, 3=v2, 4=v3, 5~8=is commander or v enabled
    //Mutator
    QList<QStringList> mutatorInfo;//0=id, 1=name, 2=icon, 3=score, 4=is custom, 5=tooltip, 6=is enabled
    //Map
    QList<QStringList> mapInfo;//0=id, 1=name, 2=img, 3=is enabled

    //int mutators[11] = {0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};  //Custom Mutators = Custom Mutator ID + 1000.

    void LoadMutators();
    void LoadCommanders();
    void LoadMaps();
    void SaveMutatorEnableInfo();
    int GetRandomMutator();
    int GetRandomCommander(int anotherCMD = -1);
    int GetRandomMap();

};
#endif // MAINWINDOW_H
