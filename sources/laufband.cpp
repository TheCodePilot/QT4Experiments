#include "laufband.h"
#include "back.h"
#include "gifs.h"
#include "game.h"

#include <QFontDatabase>
#include <QGraphicsDropShadowEffect>
#include <QScreen>
#include <QMessageBox>

Laufband::Laufband(QGraphicsScene* scene ):
    m_scene(scene),m_textPosition(0),m_textSpeed(12), berror(false)
{
    m_text = "";

    m_thegame = (Game*)qApp;
    if(!m_thegame){

        berror = true;
        return;
    }

    m_gameWorld = new GameWorld();
    if(!m_gameWorld){

        berror = true;
        return;
    }

    if(!m_scene){

        berror = true;
        return;
    }

    m_textItem = new QGraphicsTextItem();
    if(!m_textItem){

        berror = true;
        return;
    }

    t_updateInfo = new QTimer(this);
    if(!t_updateInfo){

        berror = true;
        return;
    }

    t_updateText = new QTimer(this);
    if(!t_updateText){

        berror = true;
        return;
    }

    QMetaObject::Connection connection = QObject::connect(t_updateInfo, &QTimer::timeout, this, &Laufband::updateText);
    if (!connection) {

        m_thegame->setError();
    }

    t_updateInfo->start(m_thegame->getT_InfoUpdate());

    QFontDatabase::addApplicationFont(m_thegame->getAPP_Font());
    m_textItem->setFont(QFont(m_thegame->getAPP_FontName(), 45));
    m_textItem->setPlainText(m_text);
    m_textItem->setDefaultTextColor(QColor(255, 255, 255,m_thegame->getOPAC()));
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect;

    if(effect){
        effect->setOffset(3, 3);
        effect->setColor(QColor("blue"));
        m_textItem->setGraphicsEffect(effect);
    }
    else
    {
        QMessageBox::critical(nullptr, tr("Fehler #lb01"),
                                       tr("Die Darstellung Der Laufschrift wird qualitativ schlechter sein. "
                                          "Wenn das Problem weiterhin besteht melden Sie sich bitte beim Support. "
                                          "Der Spielfluß selbst bleibt in gleicher Qualität."));

    }

    QSharedPointer<Items4Game>SI4G(new Items4Game{0,0,"Laufband"});
    m_textItem->setData(0,QVariant::fromValue(SI4G));
    m_scene->addItem(m_textItem);

    connection = QObject::connect(t_updateText, &QTimer::timeout, this, &Laufband::updateTextPosition);
    if (!connection) {
        m_thegame->setError();
    }

    t_updateText->start(m_thegame->getT_InfoNextText());
}


Laufband::~Laufband()
{
    if(t_updateInfo)
    { delete t_updateInfo;}
    if(t_updateText)
    { delete t_updateText;}
    if(m_gameWorld)
    {delete m_gameWorld;}
}

bool Laufband::getError() const {
    return berror;
}


QTimer* const Laufband::getUpdateTextTimer() const {
    return t_updateText;
}


QTimer* const Laufband::getUpdateInfoTimer() const {
    return t_updateInfo;
}


void Laufband::setGameWorlddata(GameWorld& data){

    m_gameWorld->level = data.level;
    m_gameWorld->lives = data.lives;
    m_gameWorld->energy = data.energy;
    m_gameWorld->score = data.score;
    m_gameWorld->currentenemies = data.currentenemies;
}












