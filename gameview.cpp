#include "gameview.h"
#include "gifs.h"
#include "game.h"
#include "controller.h"
#include "laufband.h"
#include "pdfdia.h"
#include "xmbox.h"

#include <QSplashScreen>
#include <QMoveEvent>
#include <QKeyEvent>
#include <QMessageBox>

//########################################################
typedef QSharedPointer<Items4Game> I4GPtr;
//########################################################

GameView::GameView(QGraphicsScene* scene)
    : QGraphicsView(scene), m_scene(scene),fixed_pos_(QPoint(0, 0)),
    bplayerReady(false), bgameOver(false), bshowDialog(true), bcalculate(true),
    berror(false)
{
    
    m_theGame = (Game*)qApp;
    if(!m_theGame){

        berror = true;
        QMessageBox::critical(nullptr, tr("Fehler #gv01"), tr(" Das Progamm beendet sich nachdem Sie mit OK bestätigt haben."));
        QTimer::singleShot(100, qApp, &QApplication::quit);
        return;
    }

    if (!m_scene) {

        berror = true;
        QMessageBox::critical(nullptr, tr("Fehler #gv02"), tr(" Das Progamm beendet sich nachdem Sie mit OK bestätigt haben."));
        QTimer::singleShot(100, qApp, &QApplication::quit);
        return;
    }

    m_player = m_theGame->getPlayer();
    if(!m_player){

        berror = true;
        QMessageBox::critical(nullptr, tr("Fehler #gv03"), tr(" Das Progamm beendet sich nachdem Sie mit OK bestätigt haben."));
        QTimer::singleShot(100, qApp, &QApplication::quit);
        return;
    }

    QPixmap pixmap(m_theGame->getAPP_Back_Start());
    QSplashScreen splash(pixmap);
    splash.show();

    m_con = std::make_unique<Controller>(this);
    if(!m_con){

        berror = true;
        QMessageBox::critical(nullptr, tr("Fehler #gv04"), tr(" Das Progamm beendet sich nachdem Sie mit OK bestätigt haben."));
        QTimer::singleShot(100, qApp, &QApplication::quit);
        return;
    }

    if(m_con && m_con->getError()){

        berror = true;
        m_theGame->setError();
        return;
    }

    m_con->getBackground()->setPlayerdata(*m_player);

    m_laufband = QSharedPointer<Laufband>(new Laufband(m_scene));
    if(!m_laufband){

        berror = true;
        QMessageBox::critical(nullptr, tr("Fehler #gv06"), tr(" Das Progamm beendet sich nachdem Sie mit OK bestätigt haben."));
        QTimer::singleShot(100, qApp, &QApplication::quit);
        return;
    }

    if(m_laufband->getError()){

        berror = true;
        QMessageBox::critical(nullptr, tr("Fehler #gv07"), tr(" Das Progamm beendet sich nachdem Sie mit OK bestätigt haben."));
        QTimer::singleShot(100, qApp, &QApplication::quit);
        return;
    }

    createTimer();

    connect(this, &GameView::viewQifExit, this, &GameView::qIfExit);
    m_connectionBravo = connect(this, &GameView::viewRemoveBravoItem, this, &GameView::removeBravoItem);
    if (!m_connectionBravo) {
        m_theGame->setError();
    }

    QMetaObject::Connection connection = connect(this, &GameView::viewSignal4Shild, this, &GameView::onViewSignal4Shild);
    if (!connection) {
        m_theGame->setError();
    }
        
    connection = connect(t_shieldTimer, &QTimer::timeout, this, &GameView::onTimerTimeout);
    if (!connection) {
        m_theGame->setError();
    }
}

GameView::~GameView(){

    if(t_timer){delete t_timer;}
    if(t_gifTimer){delete t_gifTimer;}
    if(t_newcolor){delete t_newcolor;}
    if(t_shieldTimer){delete t_shieldTimer;}

    if(gameOverItem){delete gameOverItem;}
    if(bravoItem){delete bravoItem;}
    if(m_mbox){delete m_mbox;}
}


void GameView::scaleTheScene(qreal scale){

    QList<QGraphicsItem*> enemyItems = m_scene->items(); 
    for (QGraphicsItem* item : enemyItems) {

        GIFs* enemy = dynamic_cast<GIFs*>(item);
        if ((enemy != nullptr) &&
            (item->data(0).value<I4GPtr>()->itemType == "enemy")) {
            item->setScale(scale);
        }
        else if((enemy != nullptr) &&
                 (item->data(0).value<I4GPtr>()->itemType == "Player")){
            item->setScale(scale);
        }
    }
}

void GameView::createTimer(){

    t_timer = new QTimer(this);
    if (!t_timer) {
        m_theGame->setError();
        return;
    }

    QMetaObject::Connection connection = connect(t_timer, &QTimer::timeout, this, &GameView::updateGifPosition);
    if (!connection) {
        m_theGame->setError();
    }
    t_timer->start(10);

    t_gifTimer = new QTimer(this);
    if (!t_gifTimer) {
        m_theGame->setError();
        return;
    }

    connect(t_gifTimer, &QTimer::timeout, this, &GameView::updateGif);
    if (!connection) {
        m_theGame->setError();
    }
    t_gifTimer->start(100);

    t_newcolor = new QTimer(this);
    if (!t_newcolor) {
        m_theGame->setError();
        return;
    }

    connect(t_newcolor, &QTimer::timeout, this, &GameView::changeEnemyColor);
    if (!connection) {
        m_theGame->setError();
    }
    t_newcolor->start(6000);

    t_shieldTimer = new QTimer(this);
    if (!t_shieldTimer) {
        m_theGame->setError();
        return;
    }

    t_shieldTimer->setSingleShot(true);
}


Game* const GameView::getGame() const {
    return m_theGame;
}

Controller* const GameView::getController() const {
    return m_con.get();
}


void GameView::setGameOver(){
    bgameOver = true;
}


void GameView::onViewSignal4Shild(){  

    m_player->setShild(true);
    if(!t_shieldTimer->isActive()){
        t_shieldTimer->start(5000);}
}

void GameView::onTimerTimeout(){
    m_player->setShild(false);
}


void GameView::setViewdata4Score(){

    GameWorld data;
    if(!m_con->getBackground()->getGameWorld()){
        m_theGame->setError();
        return;
    }

    data = *m_con->getBackground()->getGameWorld();

    m_currentenemies = data.currentenemies;
    m_score = data.score;
    m_scorepreview = m_score;
    m_score = score();
    m_count4Score = data.cevent;
}


void GameView::theView(){

    if(berror){
        return;
    }

    if(m_connectionBravo){
        emit viewRemoveBravoItem();}

    bplayerReady = false; bgameOver = false;
    m_player->setShild(false);

    if(bshowDialog){

        bcalculate = false;
        m_con->showWelcome();
        if(m_con->getError()){
            QMessageBox::critical(nullptr, tr("Fehler #gv08b"), tr(" Das Progamm beendet sich nachdem Sie mit OK bestätigt haben."));
            QTimer::singleShot(100, qApp, &QApplication::quit);
            berror = true;
            return;
        }

        bcalculate = true;
    }
    
    setViewdata4Score();

    m_theGame->createEnemies();

    m_laufband.data()->getUpdateInfoTimer()->start(m_theGame->getT_InfoUpdate());
    m_laufband.data()->getUpdateTextTimer()->start(m_theGame->getT_InfoNextText());
    setData4LaufBand("start");
    scaleTheScene(m_theGame->getScaleF());
}

void GameView::changeEnemyColor(){
    if(bgameOver || bplayerReady == false){
        return;}
    if(bcalculate){
        m_count4Score++;
    }

    setData4LaufBand("changeEnemyColor()");
    QList<QGraphicsItem*> items = m_scene->items();
    for (QGraphicsItem* item : items) {

        GIFs* enemy = dynamic_cast<GIFs*>(item);
        if ((enemy != nullptr) &&
            (item->data(0).value<I4GPtr>()->itemType == "enemy")) {
            if(item->data(0).value<I4GPtr>()->lastCondition == "keiner"){

                if(item->data(0).value<I4GPtr>()->scolor != "lila"){
                    enemy->setRandomColor(false);
                    item->data(0).value<I4GPtr>()->energy = enemy->getEnergy();
                    item->data(0).value<I4GPtr>()->scolor = enemy->getColorString();}
            }
            else{
                item->data(0).value<I4GPtr>()->lastCondition = "keiner";
            }
        }
    }
}


void GameView::sendSignalKeyPpressed(){
    emit signalKeyPpressed();
}

void GameView::sendSignalEnableOption(){
    emit signalEnableOption();
}

void GameView::noMoreEnemies(){

    congrats();
    int lev = m_player->getLevel(); lev++;
    m_player->setLevel(lev);

    GameWorld gw;
    gw.level = m_player->getLevel();
    gw.energy = m_player->getEnergy();
    gw.lives = m_player->getLives();
    gw.score = m_score;             
    gw.currentenemies = m_currentenemies = m_con->getBackground()->getnEnemiesStart(); 
    m_con->getBackground()->setGameWorld(gw);
    m_scorepreview = m_score;
    m_score = score();       

    emit viewNoMoreEnemies();
    QTimer::singleShot(2500, this, [=]() {

        int userChoice = -1;
        MBox mtemp(nullptr, m_con->getBackground()->getautoSaveStart());

        if(!mtemp.getError()){
            userChoice = mtemp.exec();
        }
        else
        {
            QMessageBox::critical(nullptr, tr("Fehler #gv09"),
                                           tr("Das erreichen des Levels ist gespeichert. Bitte starten Sie das Spiel erneut. "
                                              "Wenn das Problem weiterhin besteht melden Sie sich bitte beim Support. "
                                              "Das Progamm beendet sich nachdem Sie mit OK bestätigt haben."));
            QTimer::singleShot(100, qApp, &QApplication::quit);
            return;

        }

        bshowDialog = true;
        if(userChoice == 1)
        {
            bshowDialog = false;

        }

        m_theGame->setEnemysCreated(false);
        m_theGame->playNext();
            });
}


void GameView::moveEvent(QMoveEvent* event){
    if (event->pos() != fixed_pos_)
        move(fixed_pos_);
}



void GameView::keyPressEvent(QKeyEvent* event){

    if (!event->isAutoRepeat()) {
        if (event->key() == Qt::Key_P) {
            keyPisPressed();
        }

        if (event->key() == Qt::Key_H) {
            keyHisPressed();
        }

        switch (event->key()) {
        case Qt::Key_Left:
            direction_.setX(-3);
            break;
        case Qt::Key_Up:
            direction_.setY(-3);
            break;
        case Qt::Key_Right:
            direction_.setX(3);
            break;
        case Qt::Key_Down:
            direction_.setY(3);
            break;
        case Qt::Key_B:
            bplayerReady  = true;
            break;

        }
    }
}


void GameView::keyReleaseEvent(QKeyEvent* event){

    if (!event->isAutoRepeat()) {

        switch (event->key()) {
        case Qt::Key_Left:
        case Qt::Key_Right:
            direction_.setX(0);
            break;
        case Qt::Key_Up:
        case Qt::Key_Down:
            direction_.setY(0);
            break;
        }
    }
}


void GameView::setData4LaufBand(QString str){

    GameWorld data = *m_con->getBackground()->getGameWorld();

    if(str!="start")
    {
        data.level = m_player->getLevel();
        data.lives = m_player->getLives();
        data.energy = m_player->getEnergy();
        data.currentenemies = m_currentenemies;
        data.score = score();
    }

    m_laufband.data()->setGameWorlddata(data);
}

bool GameView::ifEnergy2Zero(){

    direction_.setX(0); direction_.setY(0);

    if (m_player->getEnergy() < 0) {

        if (m_player->getLives() > 0) {
            // Show dialog to ask if the player wants to spend a life on new energy or quit the game
            bcalculate = false;
            int ret = messageBox();
            if (ret == 1){

                int liv = m_player->getLives(); liv--;
                m_player->setLives(liv);
                m_player->setEnergy(100);
                setData4LaufBand("ifEnergy2Zero()");
                emit viewSignal4Shild();
                bcalculate = true;
                return false;
            }
            else
            {
                gameOver();
                return true;
            }
        }
        else
        {
            gameOver();
            return true;
        }
    }
    else
    {
        return false;
    }
}


void GameView::gameOver(){
    QPixmap gameOverPixmap(m_theGame->getAPP_Back_Gover());
    if(!gameOverItem){
        gameOverItem = new QGraphicsPixmapItem(gameOverPixmap);
        QSharedPointer<Items4Game>SI4G(new Items4Game{0,0,"gameOverItem"});
        gameOverItem->setData(0,QVariant::fromValue(SI4G));
        m_scene->addItem(gameOverItem);
    }
    bgameOver = true;
    emit viewQifExit();

}


void GameView::congrats(){
    QPixmap bravoPixmap(m_theGame->getAPP_Back_Bravo());
    if(!bravoItem){
        bravoItem = new QGraphicsPixmapItem(bravoPixmap);
        QSharedPointer<Items4Game>SI4G(new Items4Game{0,0,"bravoItem"});
        gameOverItem->setData(0,QVariant::fromValue(SI4G));
        m_scene->addItem(bravoItem);
    }
    bgameOver = true;
}


void GameView::removeBravoItem(){

    if(bravoItem){
        m_scene->removeItem(bravoItem);
        bravoItem =nullptr;}
}


void GameView::onOptionNenemiesChanged(){ 
    clearAllEnemys();
}


void GameView::inGameLoadGame(){
    clearAllEnemys();
    bshowDialog = false;
    m_con->getBackground()->setPlayerdata(*m_player);
    m_theGame->playNext();
}


void GameView::inGameNewGame(){
    clearAllEnemys();
    bshowDialog = false;
    m_con->getBackground()->set4NewGame();
    m_con->getBackground()->setPlayerdata(*m_player);
    m_theGame->playNext();
}


void GameView::qIfExit(){

    bcalculate = false;
    bshowDialog = true;
    m_con->getBackground()->setSameAgain(false);

    if(t_timer->isActive()){t_timer->stop();}
    if(t_gifTimer->isActive()){t_gifTimer->stop();}
    if(t_newcolor->isActive()){t_newcolor->stop();}
    if(t_shieldTimer->isActive()){t_shieldTimer->stop();}

    if(m_laufband.data()->getUpdateInfoTimer()->isActive()){m_laufband.data()->getUpdateInfoTimer()->stop();}
    if(m_laufband.data()->getUpdateTextTimer()->isActive()){m_laufband.data()->getUpdateTextTimer()->stop();}

    if(!m_mbox){

        m_mbox = new MBox(nullptr,2);
        if(!m_mbox){
            m_theGame->setError();
        }
    }

    QMetaObject::Connection connection = QMetaObject::Connection();
    if(!m_mbox->getError()){
        connection = QObject::connect(m_mbox->getSameAgainButton(), SIGNAL(clicked()),m_con.get(), SLOT(onSameAgain())); 
        
    }
    else
    {
        QMessageBox::critical(nullptr, tr("Fehler #gv11"),
                                       tr("Bitte starten Sie das Spiel erneut. "
                                          "Wenn das Problem weiterhin besteht melden Sie sich bitte beim Support. "
                                          "Das Progamm beendet sich nachdem Sie mit OK bestätigt haben."));
        QTimer::singleShot(100, qApp, &QApplication::quit);
    }


    if (!connection) {
        m_theGame->setError();
    }

    if ( m_mbox->exec() == 1) {

        clearAllEnemys();
        startTimer();
        m_scene->removeItem(gameOverItem);
        gameOverItem =nullptr;

        if(m_con->getBackground()->getSameAgain()){
            bshowDialog = false;
            m_con->getBackground()->setSameAgain(false); 
            m_con->getBackground()->reload4SameAgain();
        }
        m_con->getBackground()->setPlayerdata(*m_player);
        bcalculate = true;
        m_theGame->playNext();
    }

    else
    {
        QTimer::singleShot(100, qApp, &QApplication::quit);
    }

}


int GameView::messageBox() const {

    MBox d(nullptr,3);
    if(!d.getError()){
        return d.exec();
    }
    else
    {
        QMessageBox::critical(nullptr, tr("Fehler #gv10"),
                              tr("Bitte starten Sie das Spiel erneut. "
                                 "Wenn das Problem weiterhin besteht melden Sie sich bitte beim Support. "
                                 "Das Progamm beendet sich nachdem Sie mit OK bestätigt haben."));
        QTimer::singleShot(100, qApp, &QApplication::quit);
        return -1;
    }
}


void GameView::startTimer(){

    if(!t_timer->isActive()){
        t_timer->start(10);
    }

    if(!t_gifTimer->isActive()){
        t_gifTimer->start(100);
    }

    if(!t_newcolor->isActive()){
        t_newcolor->start(9000);
    }

    if(!m_laufband.data()->getUpdateInfoTimer()->isActive()){
        m_laufband.data()->getUpdateInfoTimer()->start(m_theGame->getT_InfoUpdate());
    }

    if(!m_laufband.data()->getUpdateTextTimer()->isActive()){
        m_laufband.data()->getUpdateTextTimer()->start(m_theGame->getT_InfoNextText());
    }
}


void GameView::clearAllEnemys(){

    QList<QGraphicsItem*> enemyItems = m_scene->items();

    for (QGraphicsItem* item : enemyItems) {

        if(item->data(0).value<QSharedPointer<Items4Game>>()&&
            (item->data(0).value<QSharedPointer<Items4Game>>()->itemType != "Laufband")&&
            (item->data(0).value<QSharedPointer<Items4Game>>()->itemType != "gameOverItem")&&
            (item->data(0).value<QSharedPointer<Items4Game>>()->itemType != "Player") ){
                m_scene->removeItem(item);
                QSharedPointer<Items4Game> tempitem = item->data(0).value<QSharedPointer<Items4Game>>();
                tempitem.clear();
        }
    }
    m_theGame->setEnemysCreated(false);
}


void GameView::keyPisPressed(){

    t_timer->stop();
    t_gifTimer->stop();
    t_newcolor->stop(); 
    direction_.setX(0);direction_.setY(0);
    GameWorld gw;

    gw.energy = m_player->getEnergy();
    gw.level = m_player->getLevel();
    gw.lives = m_player->getLives();
    gw.score = m_score;
    gw.currentenemies = m_currentenemies;
    m_con->getBackground()->setGameWorld(gw);
    m_con->getWelcome()->getNewButton()->setEnabled(true);
    m_con->getWelcome()->getSaveButton()->setEnabled(true);

    bcalculate = false;
    sendSignalKeyPpressed();
    sendSignalEnableOption();
    bcalculate = true;
    t_timer->start(10);
    t_gifTimer->start(100);
    t_newcolor->start(6000); 
}


void GameView::keyHisPressed(){

    t_timer->stop();
    t_gifTimer->stop();
    t_newcolor->stop();
    bcalculate = false;

    direction_.setX(0);direction_.setY(0);
    DiaStyle ds;
    PDFDia pdfdia(nullptr);
    pdfdia.setStyle(&ds);
    if(pdfdia.getError()){
        QMessageBox::critical(nullptr, tr("Fehler #gv10"),
                              tr("Informationen wurden nicht gefunden. "
                                 "Wenn das Problem weiterhin besteht melden Sie sich bitte beim Support. "
                                 "Der Spielfluß ist hier von nicht betroffen."));
    }
    else{
        pdfdia.exec();
    }

    bcalculate = true;
    t_timer->start(10);
    t_gifTimer->start(100);
    t_newcolor->start(6000);
}
