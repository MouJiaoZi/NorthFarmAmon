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

    void on_seettingCommanderEnableCheckBox_toggled(bool checked);

    void on_settingBrutalPlusRandomAICheckBox_toggled(bool checked);

    void on_settingBrutalPlusRandomMapCheckBox_toggled(bool checked);

private:
    Ui::mainWindow *ui;
    QSettings *settings;

    QCheckBox* settingCommanderCheckbox[18];

    QWidget* brutalPlusMutatorIcon[10];
    QLabel* brutalPlusMutatorName[10];
    QLabel* brutalPlusMutatorScore[10];

    int max_mutators = 0;
    int mutator_info[256][4];
    /*
     *  0   id          0   1   2   3   4   5   6
     *  1   score       5   5   5   5   5   5   5
     *  2   is_custom   0   0   0   0   0   1   1
     *  3   is_enabled  1   1   1   0   0   1   1
     */
    QStringList mutator_name;
    QStringList mutator_icon;
    QStringList mutator_tooltip;

    int mutators[11] = {0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};  //Custom Mutators = Custom Mutator ID + 1000.

    void LoadMutators();
    void SaveMutatorEnableInfo();
    void SaveCMutatorEnableInfo();
    int GetRandomMutator();
    int GetRandomCommander(int anotherCMD = -1);

};
#endif // MAINWINDOW_H
