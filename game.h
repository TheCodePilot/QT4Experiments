#ifndef GAME_H
#define GAME_H
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>

class GIFs;
class GameView;
struct Items4Game;

class Game : public QApplication {

public:

    Game(int& argc, char** argv);
    ~Game();

    QGraphicsScene* const getScene() const;
    GameView* const getView() const;
    GIFs* const getPlayer() const;

    void createEnemies();
    void playNext();
    
    qreal getGIF_X() const;
    qreal getGIF_Y() const;
    qreal getGIF_W() const;
    qreal getGIF_H() const;

    int getT_InfoUpdate() const;
    int getT_InfoNextText() const;

    QString getAPP_Font() const;
    QString getAPP_FontName() const;

    QString getAPP_Back_Start() const;
    QString getAPP_Back_Gover() const;
    QString getAPP_Back_Bravo() const;
    QString getAPP_Back_LV1() const;

    QString getAPP_GIF_Player() const;
    QString getAPP_GIF_Enemy_LV1() const;
    QString getAPP_Icon_Shild() const;
    QString getAPP_Icon() const;

    QString getDIA_Back() const;
    QString getDIA_TBar() const;
    QString getDIA_Controlls() const;
    QString getDIA_NoControlls() const;
    QString getDIA_GIF() const;

    QString getMBox_Back4Text_Gover() const; 
    QString getMBox_Back4Text_Energy() const;
    QString getMBox_Back4Text_Bravo() const; 
    QString getMBox_Back_Gover() const; 
    QString getMBox_Back_Energy() const; 

    QString getLBox_Back() const;

    int getAPP_MAX_DLEVEL() const;
    qreal getOPAC() const;

    bool getEnemysCreated() const;
    void setEnemysCreated(bool created);

    qreal randomNumber(int lowest, int highest);

    QSize getPlace4Scene() const;
    QSize getDisplaySize() const;
    qreal getScaleF() const;

    void setError();

protected:

    virtual bool notify(QObject* receiver, QEvent* event) override;

private:

    QSharedPointer<QGraphicsScene> m_gameScene;
    QSharedPointer<GameView> m_gameView;
    QSharedPointer<GIFs> m_thePlayer;

    const qreal X_SCMin = 1024.0;
    const qreal Y_SCMin = 720.0;

    const qreal X_SCRef = 1680.0;
    const qreal Y_SCRef = 1050.0;

    const qreal GIF_X = 0.0;
    const qreal GIF_Y = 0.0;
    const qreal GIF_W = 50.0;
    const qreal GIF_H = 50.0;

    const qreal OPAC = 155.0;

    const QString APP_FontName = ---XXX---
    const QString APP_Font = ---XXX---

    const QString APP_Back_Start = ":/images/splash.png";
    const QString APP_Back_Gover = ":/images/gover.png";
    const QString APP_Back_Bravo = ":/images/bravo.png";
    const QString APP_Back_LV1 = ":/images/backlv1.png";

    const QString APP_GIF_Player = ":/images/player.gif";
    const QString APP_GIF_Enemy_LV1 = ":/images/enemy.gif";
    const QString APP_Icon_Shild = ":/images/shild.png";
    const QString APP_Icon = ":/images/icon64x64.png";

    const QString DIA_Back = ":/images/rmor.png";
    const QString DIA_TBar = ":/images/gold.png";
    const QString DIA_Controlls = ":/images/cont.png";
    const QString DIA_NoControlls = ":/images/nocont.png";
    const QString DIA_GIF = ":/images/blink.gif";

    const QString MBox_Back4Text_Gover = ":/images/mbgover.png";
    const QString MBox_Back4Text_Energy = ":/images/lbgree.png";
    const QString MBox_Back4Text_Bravo = ":/images/mmbr.png";
    const QString MBox_Back_Gover = ":/images/mbbgover.png";
    const QString MBox_Back_Energy = ":/images/mbbey.png";

    const QString LBox_Back = ":/images/lbgree.png";

    const int APP_MAX_DLEVEL = 12;
    const int T_InfoUpdate = 100;
    const int T_InfoNextText = 32;

    bool bEnemysCreated;
    bool berror;

    void createScene();
    void createView();

    void ini() const;

private slots:

    void handleScreenGeometryChanged(const QRect& newGeometry);

};
#endif //GAME_H

