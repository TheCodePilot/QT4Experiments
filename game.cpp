#include "game.h"
#include "gameview.h"
#include "gifs.h"
#include "pdfdia.h"
#include <QRandomGenerator>
#include <QScreen>
#include <QMessageBox>
#include <QInputMethodQueryEvent>


Game::Game(int& argc, char** argv)
    : QApplication(argc, argv), bEnemysCreated(false),berror(false){

    ini();
    QScreen* screen = primaryScreen();
    if((screen->size().width() < X_SCMin)||(screen->size().height() < Y_SCMin)){

        QMessageBox::information(nullptr, tr("Information"),
                 tr("Bildschirmauflösungen kleiner als") + QString::number(X_SCMin) + tr(" in der Breite oder"
                    " kleiner als ") + QString::number(Y_SCMin) + tr(" in der Höhe werden nicht unterstützt."
                    " Das Progamm beendet sich nachdem Sie mit OK bestätigt haben."
                    " Bitte starten Sie das Spiel erneut mit einer größeren"
                    " Bildchirmauflösung." ));
        QTimer::singleShot(100, qApp, &QApplication::quit);
        return;
    }

    try {
         createScene();
        }
    catch (const std::exception &e) {
        Q_UNUSED(e)
        QMessageBox::critical(nullptr, tr("Fehler #ga01"), tr(" Das Progamm beendet sich nachdem Sie mit OK bestätigt haben."));
        QTimer::singleShot(100, qApp, &QApplication::quit);
        return;
    }


    try {
        createView();

    }
    catch (const std::exception &e) {
        Q_UNUSED(e)
        QMessageBox::critical(nullptr, tr("Fehler #ga02"), tr(" Das Progamm beendet sich nachdem Sie mit OK bestätigt haben."));
        QTimer::singleShot(100, qApp, &QApplication::quit);
        return;
    }


    PDFDia pdfdia(nullptr);
    DiaStyle sia;
    pdfdia.setStyle(&sia);
    if(pdfdia.getError()){
        berror = true;
    }
    else if(!berror)
    {
        pdfdia.exec();

        try {
            getView()->theView();
        }
        catch (const std::exception e) {
            Q_UNUSED(e)
            QMessageBox::critical(nullptr, tr("Fehler #ga03"), tr(" Das Progamm beendet sich nachdem Sie mit OK bestätigt haben."));
            QTimer::singleShot(100, qApp, &QApplication::quit);
            return;
        }

        QScreen* primaryScreen = QGuiApplication::primaryScreen();

        QMetaObject::Connection connection = QObject::connect(primaryScreen, &QScreen::geometryChanged, this, &Game::handleScreenGeometryChanged);
        if (!connection) {
            setError();
        }
    }

    if(berror){

        QMessageBox::critical(nullptr, QObject::tr("Fehler #ga04"),
        QObject::tr("Bitte starten Sie das Spiel erneut. "
                    "Wenn das Problem weiterhin besteht melden Sie sich bitte beim Support. "
                    "Die Tastatureingabe wird nun bis Programmende gesperrt"));
        return;
    }

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout,getScene(), &QGraphicsScene::advance);
    timer.start(1000 / 33);

}


Game::~Game(){
}


void Game::setError(){
    berror = true;
}


QGraphicsScene* const Game::getScene() const {
    return m_gameScene.data();
}


GameView* const Game::getView() const {
    return m_gameView.data();
}


GIFs* const Game::getPlayer() const {
    return m_thePlayer.data();
}


qreal Game::getGIF_X() const {
    return GIF_X;
}


qreal Game::getGIF_Y() const {
    return GIF_Y;
}


qreal Game::getGIF_W() const {
    return GIF_W;
}


qreal Game::getGIF_H() const {
    return GIF_H;
}


qreal Game::getOPAC() const {
    return OPAC;
}


int Game::getT_InfoUpdate() const {
    return T_InfoUpdate;
}

int Game::getT_InfoNextText() const {
    return T_InfoNextText;
}


int Game::getAPP_MAX_DLEVEL() const {
    return APP_MAX_DLEVEL;
}


QString Game::getAPP_FontName() const {
    return APP_FontName;
}


QString Game::getAPP_Font() const {
    return APP_Font;
}


QString Game::getAPP_Back_Start() const {
    return APP_Back_Start;
}


QString Game::getAPP_Back_Gover() const {
    return APP_Back_Gover;
}


QString Game::getAPP_Back_Bravo() const {
    return APP_Back_Bravo;
}


QString Game::getAPP_Back_LV1() const {
    return APP_Back_LV1;
}


QString Game::getDIA_Back() const {
    return DIA_Back;
}


QString Game::getDIA_TBar() const {
    return DIA_TBar;
}


QString Game::getDIA_Controlls() const {
    return DIA_Controlls;
}


QString Game::getDIA_NoControlls() const {
    return DIA_NoControlls;
}


QString Game::getDIA_GIF() const {
    return DIA_GIF;
}

QString Game::getAPP_GIF_Player() const {
    return APP_GIF_Player;
}


QString Game::getAPP_GIF_Enemy_LV1() const {
    return APP_GIF_Enemy_LV1;
}


QString Game::getAPP_Icon() const {
    return APP_Icon;
}


QString Game::getAPP_Icon_Shild() const {
    return APP_Icon_Shild;
}


QString Game::getMBox_Back_Energy() const {
    return MBox_Back_Energy;
}


QString Game::getMBox_Back_Gover() const {
    return MBox_Back_Gover;
}


QString Game::getMBox_Back4Text_Gover() const {
    return MBox_Back4Text_Gover;
}


QString Game::getMBox_Back4Text_Energy() const {
    return MBox_Back4Text_Energy;
}
 

QString Game::getMBox_Back4Text_Bravo() const {
    return MBox_Back4Text_Bravo;
}


QString Game::getLBox_Back() const {
    return LBox_Back;
}


bool Game::getEnemysCreated() const {
    return bEnemysCreated;
}


void Game::setEnemysCreated(bool created){
    bEnemysCreated = created;
}


bool Game::notify(QObject* receiver, QEvent* event){
    Q_UNUSED(receiver);

    if (berror && event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

        if (keyEvent) {
            
            int key = keyEvent->key();  

            // Lock the "B" and "P" keys and all letter and arrow keys
            if ((key >= Qt::Key_A && key <= Qt::Key_Z) || key == Qt::Key_B || key == Qt::Key_P ||
                key == Qt::Key_Left || key == Qt::Key_Right || key == Qt::Key_Up || key == Qt::Key_Down) {

                return true;
            }
        }
        else {
            QTimer::singleShot(100, qApp, &QApplication::quit);
        }
    }

    return QApplication::notify(receiver, event);
}


void Game::createScene(){

    if(!m_gameScene.data()){
        m_gameScene = QSharedPointer<QGraphicsScene>(new QGraphicsScene);}
    getScene()->setSceneRect(0, 0, getPlace4Scene().width(),getPlace4Scene().height());
    getScene()->setItemIndexMethod(QGraphicsScene::NoIndex);

    if(!m_thePlayer.data()){
        m_thePlayer = QSharedPointer<GIFs>(new GIFs(getGIF_X(), getGIF_Y(), getGIF_W(), getGIF_H(),false));
        getPlayer()->setPos(getDisplaySize().width()/2.0,getDisplaySize().height()/2.0);
        QSharedPointer<Items4Game>player0(new Items4Game{0,0,"Player",100,QColor(255, 255, 255, 0)});
        getPlayer()->setData(0,QVariant::fromValue(player0));
        getScene()->addItem(getPlayer());}
}


void Game::createEnemies(){

    if(bEnemysCreated){
        return;
    }

    qreal x, y;
    qreal velX, velY;
    int countOfEnemys = getView()->getController()->getBackground()->getCurrentEnemies(); 
    float levelfaktor = getView()->getController()->getBackground()->getlevel();

    for (int i = 0; i < countOfEnemys; i++){

        GIFs* ind = new GIFs(getGIF_X(), getGIF_Y(), getGIF_W(), getGIF_H(),true); 
        if (ind) {

            x = randomNumber(getGIF_W() + 1, getPlace4Scene().width() - getGIF_W() - 1);
            y = randomNumber(getGIF_H() + 1, getPlace4Scene().height() - getGIF_H() - 1);
            velX = randomNumber(-6 - levelfaktor, 6 + levelfaktor);
            velY = randomNumber(-6 - levelfaktor, 6 + levelfaktor);
            ind->setPos(x, y);
            ind->setRandomColor(false);

            QSharedPointer<Items4Game>enemy0(new Items4Game{ velX,velY,"enemy",ind->getEnergy(),ind->getColor(),ind->getColorString() });
            ind->setData(0, QVariant::fromValue(enemy0));
            getScene()->addItem(ind);
        }
        else
        {
            berror = true;
            return;
        }
    }

    bEnemysCreated = true;
}


void Game::createView(){

    if(!m_gameView.data()){
        m_gameView = QSharedPointer<GameView>(new GameView(getScene()));}
    getView()->setRenderHint(QPainter::Antialiasing);
    getView()->setBackgroundBrush(QPixmap(APP_Back_LV1));

    getView()->setCacheMode(QGraphicsView::CacheBackground);
    getView()->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    getView()->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    getView()->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    getView()->setWindowTitle(QT_TRANSLATE_NOOP(QGraphicsView, "dEmO")); 
    getView()->resize(getPlace4Scene().width(),getPlace4Scene().height());

    getView()->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);
    getView()->setFixedSize(getView()->size());
    setWindowIcon(QIcon(":/images/icon64x64.png"));
    getView()->show();
}


void Game::playNext(){

    createScene();
    getView()->theView();
    getView()->update();
}


qreal Game::randomNumber(int lowest, int highest)
{
    qreal randomValue = QRandomGenerator::global()->bounded(lowest, highest + 1);
    if(randomValue == 0){
        randomValue += 1.0;
    }
    return randomValue;
}


QSize Game::getDisplaySize() const {

    QScreen* screen = primaryScreen();
    return screen->size();
}


QSize Game::getPlace4Scene() const {

    QSize qs = getDisplaySize();
    int withofScrollbar = qApp->style()->pixelMetric(QStyle::PM_ScrollBarExtent);
    int spacebetween = qApp->style()->pixelMetric(QStyle::PM_ScrollView_ScrollBarSpacing);
    QSize place4Screen = QSize(qs.width(),qs.height() - withofScrollbar - spacebetween);
    return place4Screen;
}


void Game::handleScreenGeometryChanged(const QRect& newGeometry)
{
    if((newGeometry.width() < X_SCMin)||(newGeometry.height() < Y_SCMin)){

        getScene()->setSceneRect(QRectF());
        QPixmap gameOverPixmap(APP_Back_Gover);
        if (!gameOverPixmap.isNull()) {
            QGraphicsPixmapItem* gameOverItem = new QGraphicsPixmapItem(gameOverPixmap); 

            if (gameOverItem) {
                QSharedPointer<Items4Game>SI4G(new Items4Game{ 0,0,"badScreensize" });
                gameOverItem->setData(0, QVariant::fromValue(SI4G));
                getScene()->addItem(gameOverItem);
            }
        }
        getView()->resize(QSize());
        getView()->resize(getPlace4Scene().width(),getPlace4Scene().height());
        getView()->setFixedSize(QSize(getPlace4Scene().width(),getPlace4Scene().height()));

        QString tail = "/gameDataCurrent.json";
        getView()->getController()->getBackground()->setFileTail(tail);
        getView()->getController()->getBackground()->saveGameWorld();

        QMessageBox::information(nullptr, QObject::tr("information"),
        QObject::tr("Bildschirmauflösungen kleiner als ") + QString::number(X_SCMin) + tr(" in der Breite oder"
                    " kleiner als ") + QString::number(Y_SCMin) + tr(" in der Höhe werden nicht unterstützt."
                    " Das Programm speichert den aktuellen Spielstand im Zwischenspeicher ab und kann beim"
                    " nächsten Programmstart geladen werden. Die Tastatureingabe wird nun bis"
                    " Programmende gesperrt. Bitte starten Sie das Spiel erneut." ));

        getView()->setGameOver();
        berror = true;
    }
    else
    {
        getScene()->setSceneRect(QRectF());
        getScene()->setSceneRect(0,0,getPlace4Scene().width(),getPlace4Scene().height());

        getView()->resize(QSize());
        getView()->resize(getPlace4Scene().width(),getPlace4Scene().height());
        getView()->setFixedSize(QSize(getPlace4Scene().width(),getPlace4Scene().height()));
    }

}


qreal Game::getScaleF() const {

    QSizeF screenSize = getDisplaySize();
    qreal scaleFactor = qMin(screenSize.width() / X_SCRef, screenSize.height() / Y_SCRef);
    return scaleFactor;
}

