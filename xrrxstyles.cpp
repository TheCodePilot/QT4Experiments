#include "xrrxstyles.h"
#include "game.h"
#include <QApplication>
#include <QPaintDevice>
#include <QMessageBox>
GIFButtonStyle::GIFButtonStyle(const QString &fileName)
    : messageBoxAppeared(false), berror(false)
{
    Game* game = nullptr;
    game = (Game*)qApp;
    
    m_scene = QSharedPointer<QGraphicsScene>(new QGraphicsScene);
    m_textItem = QSharedPointer<QGraphicsTextItem>(new QGraphicsTextItem);
    
    if(!game || !m_scene || !m_textItem){

        berror = true;
    }

    if(!berror){

        Movie(fileName);

        QFontDatabase::addApplicationFont(game->getAPP_Font());
        m_font.setFamily(game->getAPP_FontName());
        m_font.setPointSize(14);

        m_textItem.data()->setFont(m_font);
        m_scene.data()->addItem(m_textItem.data());

        m_effect.setOffset(1, 1);
        m_effect.setColor(QColor("blue"));
        m_textItem.data()->setGraphicsEffect(&m_effect);

    }
}


GIFButtonStyle::~GIFButtonStyle(){

    if(m_movie.isValid()){
        m_movie.stop();
    }
}


bool GIFButtonStyle::getError() const {
    return berror;
}


void GIFButtonStyle::Movie(const QString &fileName){

    m_movie.setFileName(fileName); 

    if (!m_movie.isValid())
    {
        QMessageBox::critical(nullptr, tr("Fehler #st01"),
            tr("Eine Animation konnte Im Installationsverzeichnis nicht gefunden werden."
               "Die Darstellung einiger Bedienelemente wird qualitativ schlechter sein. "
               "Wenn das Problem weiterhin besteht melden Sie sich bitte beim Support. "
               "Der Spielfluß selbst bleibt in gleicher Qualität."));
    }
    else
    {
        m_movie.start();
    }
}


void GIFButtonStyle::setBlinkColor(const QColor& color){
    m_blinkColor = color;
}


void GIFButtonStyle::setBlink(bool enable){
    bblink = enable;
}

bool GIFButtonStyle::getBlink() const {
    return bblink;
}


void GIFButtonStyle::setText(const QString& txt, const QColor& color)
{
    m_text = txt;
    m_textColor = color;
}

