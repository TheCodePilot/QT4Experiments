#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "diastyle.h" 
#include "welcome.h"
#include "back.h"

#include <QObject>

class GameView;

class Controller : public QObject {

    Q_OBJECT

public:

    explicit Controller(GameView* sender);
    ~Controller();

    Welcome* const getWelcome() const;
    Background* const getBackground() const;

    void showWelcome() const;
    bool getError() const;

private:

    QSharedPointer<Welcome> m_saveDialog;
    QSharedPointer<Background> m_background;

    DiaStyle* m_ds = nullptr; 
    mutable bool berror;

public slots:

    //case: class GameView send signal
    void onGamePaused() const;
    void onNoMoreEnemies() const;
    void onSameAgain() const;

};
#endif //CONTROLLER_H


