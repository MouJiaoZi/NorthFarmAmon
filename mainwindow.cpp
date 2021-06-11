#include "mainwindow.h"

mainWindow::mainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::mainWindow)
{
    ui->setupUi(this);
    settings = new QSettings("config.ini", QSettings::IniFormat);

    //初始化变量
    //设置界面
    //指挥官CheckBox变量
    for(int i=0; i<18; i++)
    {
        settingCommanderCheckbox[i] = (QCheckBox*)ui->settingCommanderGrid->itemAt(i)->widget();
    }
    //地图CheckBox变量
    for(int i=0; i<15; i++)
    {
        settingMapCheckbox[i] = (QCheckBox*)ui->settingMapGrid->itemAt(i)->widget();
    }
    //残酷+页面元素变量
    for(int i=0; i<10; i++)
    {
        brutalPlusMutatorIcon[i] = (QWidget*)ui->brutalPlusMutatorIconList->itemAt(i)->widget();
        brutalPlusMutatorName[i] = (QLabel*)ui->brutalPlusMutatorNameList->itemAt(i)->widget();
        brutalPlusMutatorScore[i] = (QLabel*)ui->brutalPlusMutatorScoreList->itemAt(i)->widget();
    }

    //载入信息
    LoadMutators();
    LoadCommanders();
    LoadMaps();

    //设置全局禁用按钮
    if(settings->value("disableAllMutators", 0).toInt() == 1)
        ui->settingDisableAllMutatorCheckBox->setChecked(true);
    if(settings->value("disableAllCMutators", 0).toInt() == 1)
        ui->settingDisableAllCMutatorCheckBox->setChecked(true);

    //自定义残酷+设置
    QIntValidator *aIntValidator = new QIntValidator;
    aIntValidator->setRange(0, 999);
    ui->settingBrutalPlusMinMutatorScoreEditor->setValidator(aIntValidator);
    ui->settingBrutalPlusMaxMutatorScoreEditor->setValidator(aIntValidator);
    if(settings->value("BrutalPlus/RandomCommander", 0).toInt() == 1)
        ui->settingBrutalPlusRandomCommanderCheckBox->toggle();
    if(settings->value("BrutalPlus/RandomMap", 0).toInt() == 1)
        ui->settingBrutalPlusRandomMapCheckBox->toggle();
    if(settings->value("BrutalPlus/RandomAI", 0).toInt() == 1)
        ui->settingBrutalPlusRandomAICheckBox->toggle();
    ui->settingBrutalPlusMutatorAmountSelector->setCurrentIndex(settings->value("BrutalPlus/MutatorAmount", 10).toInt());
    ui->settingBrutalPlusMinMutatorScoreEditor->setText(settings->value("BrutalPlus/MutatorMinScore", 1).toString());
    ui->settingBrutalPlusMaxMutatorScoreEditor->setText(settings->value("BrutalPlus/MutatorMaxScore", 1).toString());
}

mainWindow::~mainWindow()
{
    delete ui;
}

bool mainWindow::SetMutator(int list_id, int mutator_id)
{
    QWidget* icon = ui->brutalPlusMutatorIconList->itemAt(list_id)->widget();
    QLabel* name = (QLabel*)ui->brutalPlusMutatorNameList->itemAt(list_id)->widget();
    QLabel* score = (QLabel*)ui->brutalPlusMutatorScoreList->itemAt(list_id)->widget();

    icon->setStyleSheet(mutatorInfo[mutator_id][2]);
    name->setText(mutatorInfo[mutator_id][1]);
    score->setText(mutatorInfo[mutator_id][3]+"分");
    icon->setToolTip(mutatorInfo[mutator_id][5]);
    name->setToolTip(mutatorInfo[mutator_id][5]);
    return true;
}

//获取随机因子
int mainWindow::GetRandomMutator()
{
    bool allMutatorDisabled = (settings->value("disableAllMutators", 0).toInt() == 1);
    bool allCMutatorDisabled = (settings->value("disableAllCMutators", 0).toInt() == 1);
    int id = QRandomGenerator::global()->bounded(0, mutatorInfo.size()-1);
    while(mutatorInfo[id][6] == "0" || (allMutatorDisabled && mutatorInfo[id][4] == "0") || (allCMutatorDisabled && mutatorInfo[id][4] == "1"))
    {
        id = QRandomGenerator::global()->bounded(0, mutatorInfo.size()-1);
    }
    return id;
}

//获取随机指挥官
int mainWindow::GetRandomCommander(int anotherCMD)
{
    QStringList disabledCommanders = settings->value("disabledCommanders").toString().split(",");
    int id = QRandomGenerator::global()->bounded(0, commanderInfo.size()-1);
    while(id == anotherCMD || disabledCommanders.contains(QString::number(id)))
        id = QRandomGenerator::global()->bounded(0, commanderInfo.size()-1);
    return id;
}

//获取随机地图
int mainWindow::GetRandomMap()
{
    int map = QRandomGenerator::global()->bounded(0, mapInfo.size()-1);
    while(mapInfo[map][3] == "0")
        map = QRandomGenerator::global()->bounded(0, mapInfo.size()-1);
    return map;
}

//载入突变因子
void mainWindow::LoadMutators()
{
    QFile inFile(":/resource/mutators.csv");    //0=mutator_id 1=mutator_name 2=mutator_icon 3=mutator_score 4=mutator_is_custom 5=mutator_tooltip
    QStringList lines;/*行数据*/
    QStringList disabledMutator = settings->value("disabledMutators").toString().split(","); //因子禁用设置
    if (inFile.open(QIODevice::ReadOnly))
    {
        QTextStream stream_text(&inFile);
        while (!stream_text.atEnd())
        {
            lines.push_back(stream_text.readLine());
        }
        for (int i = 0; i < lines.size(); i++)
        {
            QString line = lines.at(i);
            QStringList split = line.split(",");/*列数据*/
            if(disabledMutator.contains(QString::number(i)))
                split<<"0";
            else
                split<<"1";
            mutatorInfo<<split;
        }
        qDebug()<<mutatorInfo[0][6];
        inFile.close();
    }
    //  创建因子列表
    QListWidget* OListEnable = ui->settingEnableMutatorList;
    QListWidget* OListDisable = ui->settingDisableMutatorList;
    QListWidget* CListEnable = ui->settingEnableCMutatorList;
    QListWidget* CListDisable = ui->settingDisableCMutatorList;
    for(int i=0; i<mutatorInfo.size(); i++)
    {
        if(mutatorInfo[i][4] == "0")
        {
            if(mutatorInfo[i][6] == "1")
            {

                OListEnable->addItem(mutatorInfo[i][1]);
                QListWidgetItem* item = OListEnable->item(OListEnable->count() - 1);
                item->setWhatsThis(QString::number(i));
                item->setToolTip(mutatorInfo[i][5]);
            }
            else if(mutatorInfo[i][6] == "0")
            {
                OListDisable->addItem(mutatorInfo[i][1]);
                QListWidgetItem* item = OListDisable->item(OListDisable->count() - 1);
                item->setWhatsThis(QString::number(i));
                item->setToolTip(mutatorInfo[i][5]);
            }
        }
        else if(mutatorInfo[i][4] == "1")
        {
            if(mutatorInfo[i][6] == "1")
            {

                CListEnable->addItem(mutatorInfo[i][1]);
                QListWidgetItem* item = CListEnable->item(CListEnable->count() - 1);
                item->setWhatsThis(QString::number(i));
                item->setToolTip(mutatorInfo[i][5]);
            }
            else if(mutatorInfo[i][6] == "0")
            {

                CListDisable->addItem(mutatorInfo[i][1]);
                QListWidgetItem* item = CListDisable->item(CListDisable->count() - 1);
                item->setWhatsThis(QString::number(i));
                item->setToolTip(mutatorInfo[i][5]);
            }
        }
    }
}

//载入指挥官
void mainWindow::LoadCommanders()
{
    QFile inFile(":/resource/commanders.csv");
    QStringList lines;/*行数据*/
    QStringList disabledCommander = settings->value("disabledCommanders").toString().split(","); //指挥官禁用设置
    if (inFile.open(QIODevice::ReadOnly))
    {
        QTextStream stream_text(&inFile);
        while (!stream_text.atEnd())
        {
            lines.push_back(stream_text.readLine());
        }
        for (int i=0; i<lines.size(); i++)
        {
            QString line = lines.at(i);
            QStringList split = line.split(",");//0=commander id, 1=commander name, 2=v1, 3=v2, 4=v3, 5~8=commander or v enabled or disabled 1=enabled 0=disabled
            if(disabledCommander.contains(QString::number(i)))
                split<<"0";
            else
                split<<"1";
            commanderInfo<<split;
        }
        inFile.close();
    }
    //设置CheckBox
    for(int i=0; i<commanderInfo.size(); i++)
    {
        settingCommanderCheckbox[i]->setChecked(commanderInfo[i][5].toInt() == 1);
        settingCommanderCheckbox[i]->setText(commanderInfo[i][1]);
        connect(settingCommanderCheckbox[i], SIGNAL(toggled(bool)), this, SLOT(on_settingCommanderEnableCheckBox_toggled(bool)));
    }
}

//载入地图
void mainWindow::LoadMaps()
{
    QFile inFile(":/resource/maps.csv");
    QStringList lines;/*行数据*/
    QStringList disabledMap = settings->value("disabledMaps").toString().split(","); //指挥官禁用设置
    if (inFile.open(QIODevice::ReadOnly))
    {
        QTextStream stream_text(&inFile);
        while (!stream_text.atEnd())
        {
            lines.push_back(stream_text.readLine());
        }
        for (int i=0; i<lines.size(); i++)
        {
            QString line = lines.at(i);
            QStringList split = line.split(",");//0=id, 1=name, 2=img, 3=is enabled
            if(disabledMap.contains(QString::number(i)))
                split<<"0";
            else
                split<<"1";
            mapInfo<<split;
        }
        inFile.close();
    }
    for(int i=0; i<mapInfo.size(); i++)
    {
        settingMapCheckbox[i]->setChecked(mapInfo[i][3] == "1");
        settingMapCheckbox[i]->setText(mapInfo[i][1]);
        connect(settingMapCheckbox[i], SIGNAL(toggled(bool)), this, SLOT(on_settingMapEnableCheckBox_toggled(bool)));
    }
}

//残酷+ 随机按钮
void mainWindow::on_pushButton_clicked()
{
    if(settings->value("BrutalPlus/RandomCommander", 0) == 1)   //随机指挥官0-17
    {
        int cmd1 = GetRandomCommander(), cmd2 = GetRandomCommander(cmd1);
        ui->brutalPlusCommanderSelector_1->setCurrentIndex(cmd1);
        ui->brutalPlusCommanderSelector_2->setCurrentIndex(cmd2);
    }

    //随机因子
    int mutatorCount = settings->value("BrutalPlus/MutatorAmount", 10).toInt(), id = GetRandomMutator(),
        minScore = settings->value("BrutalPlus/MutatorMinScore", 0).toInt(), maxScore = settings->value("BrutalPlus/MutatorMaxScore", 999).toInt(),
        totalScore = 0, currentAttempt = 0, maxAttempt = 150;
    QStringList mutators;
    do
    {
        mutators.clear();
        totalScore = 0;
        currentAttempt++;
        for(int i=0; i<mutatorCount; i++)
        {
            while(mutators.contains(QString::number(id)))
                id = GetRandomMutator();
            mutators<<QString::number(id);
            totalScore = totalScore + mutatorInfo[id][3].toInt();
        }
    }
    while((mutators.size() != mutatorCount || totalScore < minScore || totalScore > maxScore) && currentAttempt <= maxAttempt);
    if(mutators.size() == mutatorCount && totalScore >= minScore && totalScore <= maxScore)
    {
        for(int i=0; i<mutators.size(); i++)
        {
            int idd = mutators[i].toInt();
            SetMutator(i, idd);
            qDebug()<<i<<":"<<mutatorInfo[idd][1]<<mutatorInfo[idd][3];
        }
        qDebug()<<"Total Score: "<<totalScore<<", Attempt: "<<currentAttempt;
    }
    else
    {
        QMessageBox::warning(NULL, "警告", "经过"+QString::number(currentAttempt)+"次尝试后仍未获得符合设置的突变因子组合，请检查您的设置。", QMessageBox::Yes, QMessageBox::Yes);
    }

    //随机地图
    if(settings->value("BrutalPlus/RandomMap", 0).toInt() == 1)
    {
        int map = GetRandomMap();
        ui->brutalPlusMapNameLabel->setText(mapInfo[map][1]);
        ui->brutalPlusMapImage->setStyleSheet(mapInfo[map][2]);
    }
}

/*
 * 官突   启用禁用因子功能
 *
 *
 */

void mainWindow::on_settingEnableMutatorList_itemDoubleClicked(QListWidgetItem *item)   //双击启用区的将其禁用
{
    int id = item->whatsThis().toInt();
    QListWidget* list = ui->settingEnableMutatorList;
    QListWidget* list2 = ui->settingDisableMutatorList;
    list->removeItemWidget(item);
    delete item;
    list2->addItem(mutatorInfo[id][1]);
    QListWidgetItem* item2 = list2->item(list2->count() - 1);
    item2->setWhatsThis(QString::number(id));
    item2->setToolTip(mutatorInfo[id][5]);
    mutatorInfo[id][6] = "0";
    SaveMutatorEnableInfo();
}

void mainWindow::on_settingDisableMutatorButton_clicked()   //点击按钮将其禁用
{
    QListWidget* list = ui->settingEnableMutatorList;
    QListWidgetItem* item = list->currentItem();
    if(item != NULL)
    {
        int id = item->whatsThis().toInt();
        list->removeItemWidget(item);
        delete item;
        QListWidget* list2 = ui->settingDisableMutatorList;
        list2->addItem(mutatorInfo[id][1]);
        QListWidgetItem* item2 = list2->item(list2->count() - 1);
        item2->setWhatsThis(QString::number(id));
        item2->setToolTip(mutatorInfo[id][5]);
        mutatorInfo[id][6] = "0";
        SaveMutatorEnableInfo();
    }
}

void mainWindow::on_settingDisableMutatorList_itemDoubleClicked(QListWidgetItem *item)  //双击禁用区将其启用
{
    int id = item->whatsThis().toInt();
    QListWidget* list = ui->settingEnableMutatorList;
    QListWidget* list2 = ui->settingDisableMutatorList;
    list2->removeItemWidget(item);
    delete item;
    list->addItem(mutatorInfo[id][1]);
    QListWidgetItem* item2 = list->item(list->count() - 1);
    item2->setWhatsThis(QString::number(id));
    item2->setToolTip(mutatorInfo[id][5]);
    mutatorInfo[id][6] = "1";
    SaveMutatorEnableInfo();
}

void mainWindow::on_settingEnableMutatorButton_clicked()    //点击按钮将其启用
{
    QListWidget* list = ui->settingDisableMutatorList;
    QListWidgetItem* item = list->currentItem();
    if(item != NULL)
    {
        int id = item->whatsThis().toInt();
        list->removeItemWidget(item);
        delete item;
        QListWidget* list2 = ui->settingEnableMutatorList;
        list2->addItem(mutatorInfo[id][1]);
        QListWidgetItem* item2 = list2->item(list2->count() - 1);
        item2->setWhatsThis(QString::number(id));
        item2->setToolTip(mutatorInfo[id][5]);
        mutatorInfo[id][6] = "1";
        SaveMutatorEnableInfo();
    }
}

void mainWindow::on_settingDisableAllMutatorCheckBox_toggled(bool checked)
{
    if(checked)
    {
        settings->setValue("disableAllMutators", 1);
    }
    else
    {
        settings->setValue("disableAllMutators", 0);
    }
}

/*
 * 练习图   启用禁用因子功能
 *
 *
 */

void mainWindow::on_settingEnableCMutatorList_itemDoubleClicked(QListWidgetItem *item)   //双击启用区的将其禁用
{
    int id = item->whatsThis().toInt();
    QListWidget* list = ui->settingEnableCMutatorList;
    QListWidget* list2 = ui->settingDisableCMutatorList;
    list->removeItemWidget(item);
    delete item;
    list2->addItem(mutatorInfo[id][1]);
    QListWidgetItem* item2 = list2->item(list2->count() - 1);
    item2->setWhatsThis(QString::number(id));
    item2->setToolTip(mutatorInfo[id][5]);
    mutatorInfo[id][6] = "0";
    SaveMutatorEnableInfo();
}

void mainWindow::on_settingDisableCMutatorButton_clicked()   //点击按钮将其禁用
{
    QListWidget* list = ui->settingEnableCMutatorList;
    QListWidgetItem* item = list->currentItem();
    if(item != NULL)
    {
        int id = item->whatsThis().toInt();
        list->removeItemWidget(item);
        delete item;
        QListWidget* list2 = ui->settingDisableCMutatorList;
        list2->addItem(mutatorInfo[id][1]);
        QListWidgetItem* item2 = list2->item(list2->count() - 1);
        item2->setWhatsThis(QString::number(id));
        item2->setToolTip(mutatorInfo[id][5]);
        mutatorInfo[id][6] = "0";
        SaveMutatorEnableInfo();
    }
}

void mainWindow::on_settingDisableCMutatorList_itemDoubleClicked(QListWidgetItem *item)  //双击禁用区将其启用
{
    int id = item->whatsThis().toInt();
    QListWidget* list = ui->settingEnableCMutatorList;
    QListWidget* list2 = ui->settingDisableCMutatorList;
    list2->removeItemWidget(item);
    delete item;
    list->addItem(mutatorInfo[id][1]);
    QListWidgetItem* item2 = list->item(list->count() - 1);
    item2->setWhatsThis(QString::number(id));
    item2->setToolTip(mutatorInfo[id][5]);
    mutatorInfo[id][6] = "1";
    SaveMutatorEnableInfo();
}

void mainWindow::on_settingEnableCMutatorButton_clicked()    //点击按钮将其启用
{
    QListWidget* list = ui->settingDisableCMutatorList;
    QListWidgetItem* item = list->currentItem();
    if(item != NULL)
    {
        int id = item->whatsThis().toInt();
        list->removeItemWidget(item);
        delete item;
        QListWidget* list2 = ui->settingEnableCMutatorList;
        list2->addItem(mutatorInfo[id][1]);
        QListWidgetItem* item2 = list2->item(list2->count() - 1);
        item2->setWhatsThis(QString::number(id));
        item2->setToolTip(mutatorInfo[id][5]);
        mutatorInfo[id][6] = "1";
        SaveMutatorEnableInfo();
    }
}

void mainWindow::on_settingDisableAllCMutatorCheckBox_toggled(bool checked)
{
    if(checked)
    {
        settings->setValue("disableAllCMutators", 1);
    }
    else
    {
        settings->setValue("disableAllCMutators", 0);
    }
}

void mainWindow::SaveMutatorEnableInfo()
{
    QString info;
    for(int i=0; i<mutatorInfo.size(); i++)
    {
        if(mutatorInfo[i][6] == "0")
        {
            info = info + QString::number(i) + ",";
        }
    }
    info.chop(1);
    settings->setValue("disabledMutators", info);
}

/*
 * 指挥官禁用设置
 *
 *
 */

void mainWindow::on_settingCommanderEnableCheckBox_toggled(bool checked)
{
    QString commanderSet;
    for(int i=0; i<commanderInfo.size(); i++)
    {
        if(!settingCommanderCheckbox[i]->isChecked())
        {
            commanderSet = commanderSet + QString::number(i) + ",";
            commanderInfo[i][5] = "0";
        }
    }
    commanderSet.chop(1);
    settings->setValue("disabledCommanders", commanderSet);
}

/*
 * 地图禁用设置
 *
 *
 */

void mainWindow::on_settingMapEnableCheckBox_toggled(bool checked)
{
    QString mapSet;
    for(int i=0; i<mapInfo.size(); i++)
    {
        if(!settingMapCheckbox[i]->isChecked())
        {
            mapSet = mapSet + QString::number(i) + ",";
            mapInfo[i][3] = "0";
        }
    }
    mapSet.chop(1);
    settings->setValue("disabledMaps", mapSet);
}

//自定义残酷+设置

void mainWindow::on_settingBrutalPlusRandomCommanderCheckBox_toggled(bool checked)  //随机指挥官
{
    if(checked)
    {
        settings->setValue("BrutalPlus/RandomCommander", 1);
    }
    else
    {
        settings->setValue("BrutalPlus/RandomCommander", 0);
    }
}

void mainWindow::on_settingBrutalPlusMutatorAmountSelector_currentIndexChanged(int index) //因子数量
{
    settings->setValue("BrutalPlus/MutatorAmount", index);
    for(int i=0; i<index; i++)
    {
        QWidget* icon = ui->brutalPlusMutatorIconList->itemAt(i)->widget();
        QLabel* name = (QLabel*)ui->brutalPlusMutatorNameList->itemAt(i)->widget();
        QLabel* score = (QLabel*)ui->brutalPlusMutatorScoreList->itemAt(i)->widget();
        icon->setVisible(true);
        name->setVisible(true);
        score->setVisible(true);
    }
    for(int i=index; i<10; i++)
    {
        QWidget* icon = ui->brutalPlusMutatorIconList->itemAt(i)->widget();
        QLabel* name = (QLabel*)ui->brutalPlusMutatorNameList->itemAt(i)->widget();
        QLabel* score = (QLabel*)ui->brutalPlusMutatorScoreList->itemAt(i)->widget();
        icon->setVisible(false);
        name->setVisible(false);
        score->setVisible(false);
    }
}

void mainWindow::on_settingBrutalPlusMinMutatorScoreEditor_textChanged(const QString &arg1) //最小因子总分
{
    settings->setValue("BrutalPlus/MutatorMinScore", arg1);
}

void mainWindow::on_settingBrutalPlusMaxMutatorScoreEditor_textChanged(const QString &arg1) //最大因子总分
{
    settings->setValue("BrutalPlus/MutatorMaxScore", arg1);
}

void mainWindow::on_settingBrutalPlusRandomMapCheckBox_toggled(bool checked) //随机地图
{
    if(checked)
    {
        settings->setValue("BrutalPlus/RandomMap", 1);
    }
    else
    {
        settings->setValue("BrutalPlus/RandomMap", 0);
    }
}

void mainWindow::on_settingBrutalPlusRandomAICheckBox_toggled(bool checked) //随机AI
{
    if(checked)
    {
        settings->setValue("BrutalPlus/RandomAI", 1);
    }
    else
    {
        settings->setValue("BrutalPlus/RandomAI", 0);
    }
}
