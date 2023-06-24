#include "diastyle.h"
#include "game.h"
#include <QApplication>


DiaStyle::DiaStyle() : berror(false)
{
    m_game = (Game*)qApp;  

    bool b1 = !m_diaControllsImage.load(m_game->getDIA_Controlls());
    bool b2 = !m_diaNoControllsImage.load(m_game->getDIA_NoControlls()); 

    if (!m_game || b1 || b2) {
        berror = true;
        return;
    }
}

DiaStyle::~DiaStyle() {

}


bool DiaStyle::getError() const {
    return berror;
}


void DiaStyle::drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget1) const
{
    if(!berror && element == CE_PushButtonBevel)
    {
        const QRect& rect = option->rect;
        QImage image = widget1->isEnabled() ? m_diaControllsImage : m_diaNoControllsImage;
        painter->drawImage(rect, image);

    }
    else if (element != QStyle::CE_SizeGrip) {
        QProxyStyle::drawControl(element, option, painter, widget1);
    }
}



