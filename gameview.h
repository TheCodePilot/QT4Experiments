#ifndef GAMEVIEW_H
#define GAMEVIEW_H
#include "controller.h"
#include <QGraphicsView>

class Laufband;
class Game;
class GIFs;
class MBox;

class GameView : public QGraphicsView {

    Q_OBJECT

public:

    explicit GameView(QGraphicsScene* scene);
    ~GameView();

    Controller* const getController() const;
    Game* const getGame() const;

    void theView();

    void scaleTheScene(qreal scale);

    void clearAllEnemys();

    void setGameOver();

    void sendSignalKeyPpressed();
    void sendSignalEnableOption();

protected:

    virtual void moveEvent(QMoveEvent* event) override;
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void keyReleaseEvent(QKeyEvent* event) override;

private:

    std::unique_ptr<Controller> m_con;
    QSharedPointer<Laufband> m_laufband;

    Game* m_theGame = nullptr;
    QGraphicsScene* m_scene = nullptr;
    QGraphicsPixmapItem* gameOverItem = nullptr;
    QGraphicsPixmapItem* bravoItem = nullptr;

    QMetaObject::Connection m_connectionBravo;

    GIFs* m_player = nullptr;
    MBox* m_mbox = nullptr;

    QTimer* t_timer = nullptr;
    QTimer* t_gifTimer = nullptr;

    QTimer* t_newcolor = nullptr;
    QTimer* t_shieldTimer = nullptr;

    QPoint fixed_pos_;
    QPoint direction_;

    int m_currentenemies;

    double m_score = 0.0;
    double m_scorepreview = 0.0;
    int m_count4Score = 0;
  
    bool bplayerReady;
    bool bgameOver;

    bool bshowDialog;
    bool bcalculate;

    bool berror;

    void createTimer();

    double score();
    void setData4LaufBand(QString str);

    void updateGif();
    void updateGifPosition();

    bool ifEnergy2Zero();
    void gameOver();

    int  messageBox() const;
    void startTimer();

    void congrats();
    void setViewdata4Score();

    void noMoreEnemies();
    void keyPisPressed();
    void keyHisPressed();

public slots:

    void changeEnemyColor();
    void qIfExit();
    void removeBravoItem();
    void inGameNewGame();
    void inGameLoadGame();
    void onOptionNenemiesChanged();

private:

    signals:

        void signalKeyPpressed();
        void signalEnableOption();

        void viewRemoveBravoItem();
        void viewQifExit();
        void viewSignal4Shild();
        void viewNoMoreEnemies();

private slots:

    void onTimerTimeout(); 
    void onViewSignal4Shild(); 

};
#endif // GAMEVIEW_H





