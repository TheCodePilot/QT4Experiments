#include "xrrxbutton.h"
#include <QTimer>


GIFButton::GIFButton(const QString& fileName, QString& text, QColor textcolor, QColor blinkcolor)
    : berror(false)
{
    m_style = QSharedPointer<GIFButtonStyle>(new GIFButtonStyle(fileName));

    if (m_style && !m_style.data()->getError())
    {
        m_style.data()->setText(text, textcolor);
        m_style.data()->setBlinkColor(blinkcolor);

        setStyle(m_style.data());

        m_timer = QSharedPointer<QTimer>(new QTimer(this));
        connect(m_timer.data(), &QTimer::timeout, this, &GIFButton::onTimeout);
        m_timer.data()->start(500);
    }
    else
    {
        berror = true;
    }
}


GIFButton::~GIFButton() {
   
}


bool GIFButton::getError() const {
    return berror;
}


void GIFButton::onTimeout()
{
    m_style.data()->setBlink(!m_style.data()->getBlink());
    update();
}
