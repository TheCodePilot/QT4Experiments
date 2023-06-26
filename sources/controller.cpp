#include "controller.h"
#include "diastyle.h"
#include "gameview.h"
#include <QPushButton>
#include <QMessageBox>
#include <QTimer>
#include <QApplication>

Controller::Controller(GameView* sender)
    :m_saveDialog(new Welcome(nullptr, sender)), m_background(new Background),
    m_ds(new DiaStyle), berror(false) 
{
    if(!m_background){
        berror = true;
    }

    if(m_background && m_background->getError()){
        berror = true;
    }

    QMetaObject::Connection connection = QObject::connect(sender, &GameView::signalKeyPpressed, this, &Controller::onGamePaused);
    if (!connection) {
        berror = true;
    }

    connection = QObject::connect(sender, &GameView::viewNoMoreEnemies, this, &Controller::onNoMoreEnemies);
    if (!connection) {
        berror = true;
    }

    if(!m_saveDialog){
        berror = true;
    }

    if (!m_ds){ 
        berror = true;
    }

    if(m_saveDialog && m_saveDialog->getError()){
        berror = true;
    }
}


Controller::~Controller(){

    if (m_ds) {
        delete m_ds;
    }
}


bool Controller::getError() const {
    return berror;
}


Welcome* const Controller::getWelcome() const {
    return m_saveDialog.data();
}


Background* const Controller::getBackground() const {
    return m_background.data();
}


void Controller::showWelcome() const {

    if (!m_saveDialog || !m_ds) {

        berror = true;
        return;
    }

    if (m_ds && m_ds->getError()) {

        berror = true;
        return;
    }

    QMetaObject::Connection connection = QObject::connect(qobject_cast<Welcome*>(m_saveDialog.data())->getSaveButton(), SIGNAL(clicked()), m_saveDialog.data(), SLOT(onSaveButtonClicked()));
    if (!connection) {

        berror = true;
    }

    connection = QObject::connect(qobject_cast<Welcome*>(m_saveDialog.data())->getLoadButton(), SIGNAL(clicked()), m_saveDialog.data(), SLOT(onLoadButtonClicked()));
    if (!connection) {

        berror = true;
    }

    if (m_saveDialog && m_saveDialog->getError()) { 
        berror = true;
    }
    else if(m_saveDialog && !m_saveDialog->getError())
    {
        m_saveDialog.data()->getLoadButton()->setEnabled(true);
        m_saveDialog.data()->getSaveButton()->setEnabled(true);

        m_saveDialog->setStyle(m_ds);
        m_saveDialog->exec();
    }
    
}


void Controller::onGamePaused() const {
    showWelcome();
}


void Controller::onNoMoreEnemies() const {
    m_background->saveContinuePoint();
}


void Controller::onSameAgain() const {
    m_background->setSameAgain(true);
}


