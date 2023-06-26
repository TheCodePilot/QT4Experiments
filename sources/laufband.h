#ifndef LAUFBAND_H
#define LAUFBAND_H
#include <QGraphicsScene>

struct GameWorld;
class Game;

class Laufband : public QObject{

    Q_OBJECT

public:

    explicit Laufband(QGraphicsScene* scene = nullptr);
    ~Laufband();

    QTimer* const getUpdateTextTimer() const;
    QTimer* const getUpdateInfoTimer() const;

    void setGameWorlddata(GameWorld& data);
    bool getError() const;

private:

    GameWorld* m_gameWorld = nullptr;
    Game* m_thegame = nullptr;

    QGraphicsScene* m_scene = nullptr;
    QGraphicsTextItem* m_textItem = nullptr;

    QTimer* t_updateInfo = nullptr;
    QTimer* t_updateText = nullptr;

    QString m_text;

    qreal m_textPosition;
    qreal m_textSpeed;

    bool berror;

    void updateTextPosition();

private slots:

    void updateText();
};

#endif //LAUFBAND_H

