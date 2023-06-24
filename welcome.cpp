#include "welcome.h"
#include "diastyle.h"
#include "optionen.h"
#include "gameview.h"
#include "game.h"

#include <QPushButton>

#include <QMessageBox>
#include "game.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QJsonArray>
#include <QTimer>
#include <QPainterPath>
#include <QCloseEvent>

Welcome::Welcome(QWidget* parent, GameView* gView)
    : QDialog(parent), m_gameView(gView),
    berror(false), bcontrolls(true)  {

    m_scoret = new QLabel(tr("Punkteanzahl:"),this);
    m_levelt = new QLabel(tr("Level:"),this);
    m_score = new QLabel(tr(""),this);
    m_level = new QLabel("",this);
    m_listbox = new QListWidget(this);

    m_loadButton = new QPushButton(tr("laden"), this);
    m_saveButton = new QPushButton(tr("speichern"), this);
    m_optButton = new QPushButton(tr("Optionen"), this);
    m_back2Button = new QPushButton(tr("zurück"), this);
    m_newButton = new QPushButton(tr("neues Spiel"), this);
    m_exitButton = new QPushButton(tr("Spiel verlassen"), this);

    if(!m_scoret || !m_levelt || !m_score || !m_level ||
        !m_listbox || !m_loadButton || !m_saveButton || !m_optButton ||
        !m_back2Button || !m_newButton || !m_exitButton){
        berror = true;
        bcontrolls = false;
    }

    if(bcontrolls){

        setWinHints();
        picBackground(m_gameView->getGame()->getDIA_Back());
        setStyleSheet("QLabel, QAbstractButton, QGroupBox, QRadioButton, QCheckBox { color: magenta; font-size: 15px; font-weight: bold; }");

        m_back2Button->setObjectName("exitbutton");
        m_back2Button->setObjectName("back2button");
        m_saveButton->setObjectName("savebutton");
        m_optButton->setObjectName("diasbleOptButton");

        layout3lsoButtons = new QHBoxLayout();
        layout3bneButtons = new QHBoxLayout();
        layout2Labels = new QHBoxLayout();
        layoutViewList = new QVBoxLayout();

        m_vertiLabelGroupBox = new QGroupBox(tr(""));
        m_vertiViewListGroupBox = new QGroupBox(tr(""));
        m_vertibuttons3lsoGroupBox = new QGroupBox(tr(""));
        m_vertibuttons3bneGroupBox = new QGroupBox(tr(""));

        mainLayoutIn1Spalte = new QVBoxLayout;

        createLabels();

        layout2Labels->addWidget(m_scoret);
        layout2Labels->addWidget(m_score);
        layout2Labels->addWidget(m_levelt);
        layout2Labels->addWidget(m_level);
        m_vertiLabelGroupBox->setLayout(layout2Labels);

        createListBox();
        
        layoutViewList->addWidget(m_listbox);
        m_vertiViewListGroupBox->setLayout(layoutViewList);

        layout3lsoButtons->addWidget(m_loadButton);
        layout3lsoButtons->addWidget(m_saveButton);
        layout3lsoButtons->addWidget(m_optButton);
        m_vertibuttons3lsoGroupBox->setLayout(layout3lsoButtons);

        layout3bneButtons->addWidget(m_back2Button);
        layout3bneButtons->addWidget(m_newButton);
        layout3bneButtons->addWidget(m_exitButton);
        m_vertibuttons3bneGroupBox->setLayout(layout3bneButtons);

        mainLayoutIn1Spalte->addWidget(m_vertiLabelGroupBox);
        mainLayoutIn1Spalte->addWidget(m_vertiViewListGroupBox);
        mainLayoutIn1Spalte->addWidget(m_vertibuttons3lsoGroupBox);
        mainLayoutIn1Spalte->addWidget(m_vertibuttons3bneGroupBox);

        setLayout(mainLayoutIn1Spalte);

        connections();
    }
}


Welcome::~Welcome(){
    
}


bool Welcome::getError() const {
    return berror;
}


void Welcome::ifErrorDisableControls(bool owenErrorMessage) const { 

    if (!bcontrolls) {
        return;
    }

    QList<QWidget*> controlWidgets = {m_listbox, m_saveButton, m_loadButton, m_newButton, m_optButton, m_back2Button};

    for (QWidget* widget : controlWidgets) {
        if (widget && widget->objectName() != "exitbutton") {
            widget->setEnabled(false);
        }
    }
    
    if(owenErrorMessage){
        QMessageBox::critical(nullptr, QObject::tr("Fehler #we01"),
                              QObject::tr("Bitte starten Sie das Spiel erneut. "
                                          "Wenn das Problem weiterhin besteht melden Sie sich bitte beim Support. "
                                          "Alle Bedienmöglichkeiten bis auf die Taste ""Spiel verlassen"" sind nun gesperrt."));
    }
}



void Welcome::connections(){

    QMetaObject::Connection connection = QObject::connect(m_gameView, &GameView::signalKeyPpressed, this, &Welcome::onDisableOptionButton);
    if (!connection) {
        berror = true;
     }

    connection = QObject::connect(m_gameView, &GameView::signalEnableOption, this, &Welcome::onEnableOptionButton);
    if (!connection) {
        berror = true;
    }

    connection = QObject::connect(m_newButton, &QPushButton::clicked, this, &Welcome::onNewButtonClicked);
    if (!connection) {
        berror = true;
    }

    connection = QObject::connect(m_listbox, &QListWidget::itemClicked, this, &Welcome::onSetNewFileInfo);
    if (!connection) {
        berror = true;
    }

    connection = QObject::connect(m_optButton, &QPushButton::clicked, this, &Welcome::onButtonOptClicked);
    if (!connection) {
        berror = true;
    }

    connection = QObject::connect(m_exitButton, &QPushButton::clicked, this, &Welcome::onExitButtonClicked);
    if (!connection) {
        berror = true;
    }

    connection = QObject::connect(m_back2Button, &QPushButton::clicked, this, &Welcome::onBack2ButtonClicked);
    if (!connection) {
        berror = true;
    }

    connection = QObject::connect(this, &Welcome::newGame, m_gameView, &GameView::inGameNewGame);
    if (!connection) {
        berror = true;
    }

    connection = QObject::connect(this, &Welcome::loadGame, m_gameView, &GameView::inGameLoadGame);
    if (!connection) {
        berror = true;
    }
}


QRectF Welcome::getGeo() const {
    return (QRectF)(this->geometry());
}


QPushButton* const Welcome::getSaveButton() const {
    return m_saveButton;
}


QPushButton* const Welcome::getLoadButton() const {
    return m_loadButton;
}


QPushButton* const Welcome::getExitButton() const {
    return m_exitButton;
}


QPushButton* const Welcome::getNewButton() const {
    return  m_newButton;
}


void Welcome::onButtonOptClicked(){

    DiaStyle ds;

    QSharedPointer<Optionen>m_optionen(new Optionen(m_gameView,1,60,nullptr));

    if(m_optionen){

        if(m_optionen->getError()){
            berror = true;
        }

        m_optionen.data()->setStyle(&ds);

        QMetaObject::Connection connection = QObject::connect(qobject_cast<Optionen*>(m_optionen.data())->getAutoNext(),SIGNAL(stateChanged(int)),m_optionen.data(),SLOT(onCheckedAutoNext(int)));
        if (!connection) {
            berror = true;
        }

        connection = QObject::connect(qobject_cast<Optionen*>(m_optionen.data())->getDlevel(), SIGNAL(valueChanged(int)),m_optionen.data(), SLOT(onDlevelClicked(int)));
        if (!connection) {
            berror = true;
        }

        connection = QObject::connect(qobject_cast<Optionen*>(m_optionen.data())->getEnemies(), SIGNAL(valueChanged(int)),m_optionen.data(), SLOT(onNenemiesClicked(int)));
        if (!connection) {
            berror = true;
        }
    }
    else{
        berror = true;
    }

    if(berror){
        ifErrorDisableControls(true);
        return;
    }

    m_optionen->exec();
}


void Welcome::closeEvent(QCloseEvent* event){
    event->ignore();
}


void Welcome::onNewButtonClicked(){

    if(m_gameView->getController()->getBackground()){
        m_gameView->getController()->getBackground()->setUserAction(0);
    }
    else
    {
        berror = true;
    }
    accept();
    emit newGame();
}


void Welcome::onExitButtonClicked(){

    reject();
    QTimer::singleShot(100,m_gameView->getGame(), &QApplication::quit);
}


void Welcome::onBack2ButtonClicked(){
    accept();
}


void Welcome::onDisableOptionButton(){
    m_optButton->setEnabled(false);
}


void Welcome::onEnableOptionButton(){
    m_optButton->setEnabled(true);
}


void Welcome::onSetNewFileInfo(){

    int index=m_listbox->currentRow();
    QString tail = "/gameData" + QString::number(index) + ".json";

    if(index == 3){
        QString str = "/gameDataCurrent";
        tail = str + ".json";
    }

    QString path = m_gameView->getController()->getBackground()->getPath() + tail;

    QFile loadFile(path);
    if (!loadFile.open(QIODevice::ReadOnly)) {
        index++;
        QString str= tr("Keine Datei Nr.") + QString::number(index);
        m_score->setText(str);
        m_level->setText(str);
        return;
    }

    QJsonDocument loadDoc = QJsonDocument::fromJson(loadFile.readAll());
    loadFile.close();

    QJsonObject gameworld = loadDoc.object();
    QJsonArray gameState = gameworld["GameState"].toArray();

    double score = gameState[4].toString().toDouble();
    int level = gameState[1].toString().toInt();

    m_score->setText(QString().asprintf("%0.5f",score));
    m_level->setText(QString::number(level));

    m_gameView->getController()->getBackground()->setFileTail(tail);

}


void Welcome::onSaveButtonClicked() {

    int index=m_listbox->currentRow();
    QString tail = "/gameData" + QString::number(index) + ".json";
    if(index == 3){
        QString str = "/gameDataCurrent";
        tail = str + ".json";
    }
    m_gameView->getController()->getBackground()->setFileTail(tail);
    m_gameView->getController()->getBackground()->saveGameWorld();
}


void Welcome::onLoadButtonClicked() {

    if(!m_gameView->getController()->getBackground()->getFileStatus(-1)){

        m_loadButton->setEnabled(false);
        QMessageBox qm(QMessageBox::Information,tr("Hinweis"), tr("Sie haben keine Daten abgespeichert um welche laden zu können."));
            qm.iconPixmap();
        qm.exec();
        return;
    }
    else{

        int index=m_listbox->currentRow();
        QString tail = "/gameData" + QString::number(index) + ".json";
        if(index == 3){
            QString str = "/gameDataCurrent";
            tail = str + ".json";
        }

        m_gameView->getController()->getBackground()->setFileTail(tail);
        m_gameView->getController()->getBackground()->loadGameWorld();

        if(m_gameView->getController()->getBackground()){
            m_gameView->getController()->getBackground()->setUserAction(1);
        }
        else
        {
            berror = true;
        }

        m_back2Button->setEnabled(true);
        m_saveButton->setEnabled(true);

        emit loadGame();
    }
}


void Welcome::showEvent(QShowEvent* event){

    if(m_gameView->getController()->getBackground() && m_back2Button && m_saveButton){
        if(m_gameView->getController()->getBackground()->getUserAction() == -1 ){

            m_back2Button->setEnabled(false);
            m_saveButton->setEnabled(false);
        }
        else
        {
            m_back2Button->setEnabled(true);
            m_saveButton->setEnabled(true);
        }
    }
    else
    {
        berror = true;
    }

    if(berror){
        ifErrorDisableControls(true);
    }

    if(!bcontrolls){
        return;
    }

    QDialog::showEvent(event);
    qreal posx = m_gameView->getGame()->getPlace4Scene().width()/2.0-this->geometry().width()/2.0;
    qreal posy = m_gameView->getGame()->getPlace4Scene().height()/2.0-this->geometry().height()/2.0;
    this->move(posx,posy);
}


void Welcome::paintEvent(QPaintEvent* event)
{    
    QDialog::paintEvent(event);

    paintTitlebar();
    paintRoundBackground(m_gameView->getGame()->getDIA_Back());
    adjustLayout();
}


void Welcome::createListBox() const {

    QListWidgetItem* item0 = new QListWidgetItem(tr("Speicherplatz 1"));
    item0->setIcon(QIcon(m_gameView->getGame()->getAPP_Icon()));
    item0->setForeground(Qt::white);
    m_listbox->addItem(item0);

    QListWidgetItem* item1 = new QListWidgetItem(tr("Speicherplatz 2"));
    item1->setIcon(QIcon(m_gameView->getGame()->getAPP_Icon()));
    item1->setForeground(Qt::white);
    m_listbox->addItem(item1);

    QListWidgetItem* item2 = new QListWidgetItem(tr("Speicherplatz 3"));
    item2->setIcon(QIcon(m_gameView->getGame()->getAPP_Icon()));
    item2->setForeground(Qt::white);
    m_listbox->addItem(item2);

    QListWidgetItem* item3 = new QListWidgetItem("Zwischenspeicher");
    item3->setIcon(QIcon(m_gameView->getGame()->getAPP_Icon()));
    item3->setForeground(Qt::blue);
    m_listbox->addItem(item3);

    QString str = "background-image: url(";
    QString str1 = m_gameView->getGame()->getLBox_Back();
    QString str2 = ")";
    str.append(str1);
    str.append(str2);
    m_listbox->setStyleSheet(QString("QListWidget { %1;} QListWidget::item:selected { background-color: orange; }").arg(str));

    QFont font("Arial", 20);
    font.setBold(true);
    m_listbox->setFont(font);
    m_listbox->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

}


void Welcome::createLabels() const {

    m_scoret->setObjectName("fixtextscore");
    m_scoret->setStyleSheet("#fixtextscore { color : black; font-size : 13px; font-weight : bold; }");
  
    m_score->setObjectName("score");
    m_score->setStyleSheet("#score { color : magenta; font-size : 13px; font-weight : bold; text-align : left; }");
    m_score->setContentsMargins(0, 0, 175, 0);
   
    m_levelt->setObjectName("fixlevel");
    m_levelt->setStyleSheet("#fixlevel { color : black; font-size : 13px; font-weight : bold; }");
   
    m_level->setObjectName("level");
    m_level->setStyleSheet("#level { color : magenta; font-size : 13px; font-weight : bold; text-align : left; }");
}
