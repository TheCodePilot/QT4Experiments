#ifndef GIFS_H
#define GIFS_H
#include <QGraphicsEllipseItem>
#include <QMovie>
#include <QTimer>

struct Items4Game
{
	---XXX---
};


class GIFs : public QObject, public QGraphicsEllipseItem {

    Q_OBJECT

public:

    explicit GIFs(qreal x, qreal y, qreal w, qreal h, bool enemy=false);
    ~GIFs();

    QTimer* const getTimer4Delay() const;

    void setShild(bool shild);
    bool getShild() const;

    void setRandomColor(bool lilaflag);

    QString getColorString() const;
    QColor getColor() const;

    int getEnergy() const;
    void setEnergy(int energy);
  
    int getLevel() const;
    void setLevel(int level);
    
    int getLives() const;
    void setLives(int lives);

    bool getError() const;
 
protected:

    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

private:

    QMovie m_movie;

    QTimer* t_delay = nullptr;
    QTimer* t_2beBig = nullptr;

    int m_energy = -1;
    int m_lives = -1;
    int m_level = -1;

    QString m_color = "keine";
    QColor m_boundingColor;

    int m_once = 0;
    bool benemy;
    bool bshild;

    bool berror;

    qreal randomNumber(int lowest,int highest);
    void farblos();
    void gruen();
    void rot();
    void lila();

    void paintThePlayer(QPainter* painter);
    void movie();

private slots:

    void reSize2Normal();
    void makeAfterDelay();

};
#endif //GIFS_H

