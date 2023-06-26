#ifndef XRRXSTYLES_H
#define XRRXSTYLES_H

#include <QProxyStyle>
#include <QMovie>
#include <QFontDatabase>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsDropShadowEffect>
#include <QStyleOption>
#include <QPainter>

class GIFButtonStyle : public QProxyStyle
{
    Q_OBJECT 

public:
    explicit GIFButtonStyle(const QString& fileName);
    ~GIFButtonStyle();

    void setBlinkColor(const QColor& color);
    void setBlink(bool enable);
    bool getBlink() const;
    void setText(const QString& txt, const QColor& color);

    bool getError() const;

protected:

    virtual void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const override;

private:

    QSharedPointer<QGraphicsScene> m_scene;
    QSharedPointer<QGraphicsTextItem> m_textItem;

    QMovie m_movie;
    QGraphicsDropShadowEffect m_effect; 
    
    QColor m_blinkColor = Qt::red;
    QString m_text;
    QColor m_textColor = Qt::black;
    QFont m_font;

    mutable bool bconnected = false;
    bool bblink = false;
    mutable bool messageBoxAppeared;
    bool berror;

    void Movie(const QString &fileName);
};
#endif //XRRXSTYLES_H
