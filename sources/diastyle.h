#ifndef DIASTYLE_H
#define DIASTYLE_H
#include <QProxyStyle>
#include <QStyleOption>
#include <QPainter>

class Game;

class DiaStyle : public QProxyStyle
{

public:

    DiaStyle();
    ~DiaStyle();

    virtual void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget1)const override;

    bool getError() const;

private:

    QImage m_diaControllsImage;
    QImage m_diaNoControllsImage;

    Game* m_game = nullptr;

    bool berror; 

};
#endif //DIASTYLE_H
