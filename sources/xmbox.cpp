#include "xmbox.h"
#include "game.h"
#include <QApplication>
#include <QDialogButtonBox>

#include <QMessageBox>
#include <QTimer>


MBox::MBox(QWidget* parent,int opt)
    : QDialog(parent), m_option(opt), berror(false) {

    QString strInfo="";
    QString strButtonOK = tr("OK");
    QString strButtonNOK = tr("Nein");
    QString text = tr("Das gleiche nochmal");

    Game* game = nullptr;
    game = (Game*)qApp;

    m_info_layout = new QVBoxLayout();
    m_info_label = new QLabel(strInfo);

    QFrame* info_frame = nullptr;
    info_frame = new QFrame();

    QDialogButtonBox* buttonBox = nullptr;
    buttonBox = new QDialogButtonBox(Qt::Horizontal, this);

    QVBoxLayout* layout = nullptr;
    layout = new QVBoxLayout();

    if(game && info_frame && buttonBox && layout && m_info_layout && m_info_label){
        setWinHints();
        m_info_label->setStyleSheet ("font-weight: bold; color: white; font-size: 20px");
    }
    else{
        berror = true;
    }

    if(!berror && (m_option <= 1)){

        strInfo = tr("Gklückwunsch zum Erreichen des nächsten Levels. "
                     "Zum nächsten Level oder zum Hauptmenü zurück ? ");

        if(m_option==1){
            strInfo = tr("Gklückwunsch zum Erreichen des nächsten Levels. "
                         "Zum nächsten Level oder zum Hauptmenü zurück ? "
                         "Das Spiel wird an dieser Stelle automatisch gespeichert. ");
        }

        strButtonOK = tr("nächstes level");
        strButtonNOK = tr("Startbilschirm");

        m_info_label->setStyleSheet ("font-weight: bold; color: orange; font-size: 20px");
        m_pathPicBackg = game->getMBox_Back4Text_Bravo(); 
        picBackground(m_pathPicBackg);
    }

    if(!berror && (m_option == 2)){

        strInfo = tr("Das Spiel ist zu ende. Möchten Sie zum Startbildschirm oder das Spiel verlassen ?");
        strButtonOK = tr("Startbilschirm");
        strButtonNOK = tr("Spiel verlassen");

        m_info_label->setStyleSheet ("font-weight: bold; color: black; font-size: 20px");
        m_pathPicBackg = game->getMBox_Back4Text_Gover(); 
        picBackground(m_pathPicBackg);
    }

    if(!berror && (m_option == 3)){

        strInfo = tr("Die Energie ist verbraucht. Ein Leben für neue 100% Energie einsetzen oder Spiel abbrechen ?");
        strButtonOK = tr("1 Leben nutzen");
        strButtonNOK = tr("Spielabruch");

        m_info_label->setStyleSheet ("font-weight: bold; color: yellow; font-size: 20px");
        m_pathPicBackg = game->getMBox_Back4Text_Energy();
        picBackground(m_pathPicBackg);
    }

    if(!berror){

        m_StartButton = new GIFButton(game->getDIA_GIF(),strButtonOK);
        if(!m_StartButton || (m_StartButton && m_StartButton->getError() ) ) {
            berror = true;
        }

        if(m_option == 2){
            m_sameAgainButton = new GIFButton(game->getDIA_GIF(),text);
            if(!m_sameAgainButton || (m_sameAgainButton && m_sameAgainButton->getError() ) ) {
                berror = true;
            }
        }

        m_cancelButton = new GIFButton(game->getDIA_GIF(),strButtonNOK,Qt::white,Qt::red);
        if(!m_cancelButton || (m_cancelButton && m_cancelButton->getError() ) ) {
            berror = true;
        }
    }

    if(!berror){

        QPixmap pixmap(m_pathPicBackg);

        m_info_label->setText(strInfo);
        m_info_label->setWordWrap(true);

        info_frame->setFixedSize(540,220);
        m_info_layout->addSpacerItem(new QSpacerItem(0, 50));
        m_info_layout->addWidget(m_info_label);
        QPalette palette = m_info_label->palette();

        QPixmap scaledPixmap = pixmap.scaled(info_frame->size(), Qt::IgnoreAspectRatio);
        palette.setBrush(QPalette::Window, QBrush(scaledPixmap));

        m_info_label->setAutoFillBackground(true);
        m_info_label->setPalette(palette);
        info_frame->setLayout(m_info_layout);

        buttonBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        m_StartButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        m_StartButton->setFixedSize(170,50);
        buttonBox->addButton(m_StartButton, QDialogButtonBox::AcceptRole);

        if(m_option == 2){

            m_sameAgainButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            m_sameAgainButton->setFixedSize(170,50);
            buttonBox->addButton(m_sameAgainButton, QDialogButtonBox::AcceptRole);
        }

        m_cancelButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        m_cancelButton->setFixedSize(170,50);
        buttonBox->addButton(m_cancelButton, QDialogButtonBox::RejectRole);

        layout->addWidget(info_frame);
        layout->addWidget(buttonBox);
        layout->setAlignment(buttonBox, Qt::AlignHCenter);
        setLayout(layout);

        QMetaObject::Connection connection = connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
        if (!connection) {
            berror = true;
        }

        connection = connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
        if (!connection) {
            berror = true;
        }
    }

    if(berror){

        if(info_frame){
            delete info_frame;
        }

        if(buttonBox){
            delete buttonBox;
        }

        if(layout){
            delete layout;
        }

    }

}


MBox::~MBox(){
}


bool MBox::getError() const {
    return berror;
}


GIFButton* const MBox::getSameAgainButton() const {
    return m_sameAgainButton;
}

GIFButton* const MBox::getStartButton() const {
    return m_StartButton;
}


void MBox::showEvent(QShowEvent* event){

    if(berror){
    return;
    }

    QDialog::showEvent(event);
}

void MBox::paintEvent(QPaintEvent* event)
{
    if(berror){
        return;
    }

    Game* game = (Game*)qApp;

    QString path(game->getDIA_Back());
    QDialog::paintEvent(event);

    paintTitlebar();
    paintRoundBackground(path);

}


void MBox::picBackground(QString& path) {

    QPalette pal = this->palette();
    pal.setBrush(QPalette::Window, QBrush(QPixmap(path).scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
    this->setPalette(pal);
}

