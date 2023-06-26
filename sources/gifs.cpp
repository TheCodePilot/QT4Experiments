#include "gifs.h"
#include "game.h"
#include <QPainter>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QGraphicsItem>

typedef QSharedPointer<Items4Game> I4GPtr;

GIFs::GIFs(qreal x, qreal y, qreal w, qreal h, bool enemy) :
    QGraphicsEllipseItem(x, y, w, h), benemy(enemy), berror(false), bshild(false){

    t_delay = new QTimer(this);
    if(!t_delay){
        berror = true;
        return;
    }

    t_2beBig = new QTimer(this);
    if(!t_2beBig){
        berror = true;
        return;
    }

    QMetaObject::Connection connection = connect(t_delay, SIGNAL(timeout()), this, SLOT(makeAfterDelay()));
    if (!connection) {
        Game* game = (Game*)qApp;
        game->setError();
    }

    connection = connect(t_2beBig, SIGNAL(timeout()), this, SLOT(reSize2Normal()));
    if (!connection) {
        Game* game = (Game*)qApp;
        game->setError();
    }
    movie();
}

GIFs::~GIFs(){

    if(t_2beBig){
        if(t_2beBig->isActive())
        {
            t_2beBig->stop();
            delete t_2beBig;
        }
    }

    if(t_delay){
        if(t_delay->isActive())
        {
            t_delay->stop();
            delete t_delay;
        }
    }
}

bool GIFs::getError() const {
    return berror;
}

void GIFs::movie(){

    Game* game = (Game*)qApp;

    if(!benemy){
        m_movie.setFileName(game->getAPP_GIF_Player());
    }
    else
    {
        m_movie.setFileName(game->getAPP_GIF_Enemy_LV1());
    }

    if (!m_movie.isValid())
    {
        game->setError();
    }
    else
    {
        m_movie.start();  
    }
}

QString GIFs::getColorString() const {
    return m_color;
}

QColor GIFs::getColor() const {
    return m_boundingColor;
}

int GIFs::getEnergy() const {
    return m_energy;
}

void GIFs::setEnergy(int energy){
    m_energy = energy;
}

int GIFs::getLevel() const {
    return m_level;
}

void GIFs::setLevel(int level){
    m_level = level;
}

void GIFs::setLives(int lives){
    m_lives = lives;
}

int GIFs::getLives() const {
    return m_lives;
}

void GIFs::setShild(bool shild){
    bshild = shild;
    update();
}

bool GIFs::getShild() const {
    return bshild;
}

QTimer* const GIFs::getTimer4Delay() const {
    return t_delay;
}





void GIFs::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {

    QGraphicsEllipseItem::paint(painter, option, widget);

    if(benemy){

        QRectF rectEnemy = boundingRect();
        painter->drawImage(rectEnemy, m_movie.currentImage());

        painter->setPen(QPen(m_boundingColor, 5));
        QPainterPath path;
        path.addEllipse(rectEnemy);
        painter->setClipPath(path);
        painter->drawPath(path);

    }
    else{
        paintThePlayer(painter);
    }
}


void GIFs::paintThePlayer(QPainter* painter){

    Game* game = (Game*)qApp;
    QPainterPath path;
    QRectF rectPlayer = boundingRect();
    path.addEllipse(rectPlayer);
    painter->setClipPath(path);

    if(bshild){
        painter->drawImage(rectPlayer, m_movie.currentImage());
        painter->drawImage(QRectF(rectPlayer.x() +5, rectPlayer.y() +5, rectPlayer.width()-10, rectPlayer.height()-10),
                           QImage(game->getAPP_Icon_Shild()));
    }
    else{
        painter->drawImage(rectPlayer, m_movie.currentImage());
    }
}


qreal GIFs::randomNumber(int lowest,int highest)
{
    int randomValue = QRandomGenerator::global()->bounded(lowest, highest);
    return (qreal)randomValue;
}


void GIFs::farblos() {

    if (!benemy) {
        return;
    }

    m_energy = 0;
    m_boundingColor = QColor(255, 255, 255, 0);
    m_color = "farblos";
    update(this->boundingRect());
}

void GIFs::gruen() {

    if (!benemy) {
        return;
    }

    m_energy = 10;
    m_boundingColor = QColor(0, 255, 0, 128);
    m_color = "gruen";
    update(this->boundingRect());
}

void GIFs::rot() {
    if (!benemy) {
        return;
    }

    m_energy = -10;
    m_boundingColor = QColor(Qt::red);
    m_color = "rot";
    update(this->boundingRect());
}

void GIFs::lila() {
    if (!benemy) {
        return;
    }

    m_energy = -27;
    m_boundingColor = QColor(Qt::darkBlue);
    m_color = "lila";
    update(this->boundingRect());
}

void GIFs::setRandomColor(bool lilaflag){
    int colorNum;

    if(lilaflag){
        colorNum = randomNumber(1,4);
    }
    else{
        colorNum = randomNumber(1,5);
    }

    switch (colorNum)
    {
        case 2:
        {
            gruen();
            break;}
        case 3:
        {
            rot();
            break;}
        case 4:
        {
            lila();
            break;}
        default:
        {
            farblos();
            break;}
    }
}
