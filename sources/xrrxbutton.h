#ifndef XRRXBUTTON_H
#define XRRXBUTTON_H
#include "xrrxstyles.h"
#include <QPushButton>

class GIFButton : public QPushButton
{
    Q_OBJECT

public:

    explicit GIFButton(const QString& fileName,QString& text,QColor textcolor = Qt::white,QColor bcolor = Qt::green);
    ~GIFButton();

    bool getError() const;

private:

    QSharedPointer<GIFButtonStyle> m_style;
    QSharedPointer<QTimer> m_timer; 

    bool berror;

private slots:

    void onTimeout();
};
#endif //XRRXBUTTON_H
