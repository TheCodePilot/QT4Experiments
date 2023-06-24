#include "optionen.h"
#include "diastyle.h"
#include "game.h"
#include "gameview.h"
#include "fdia.h"

#include <QHBoxLayout>
#include <QPainterPath>
#include <QApplication>
#include <QTextEdit>
#include <QMessageBox>

Optionen::Optionen(GameView* view, int min, int max, QWidget* parent)
    : QDialog(parent), m_view(view), berror(false) {

    setWinHints();
    picBackground(m_view->getGame()->getDIA_Back());

    setStyleSheet("QLabel, QAbstractButton, QGroupBox, QRadioButton, QCheckBox  { color: magenta; font-size: 15px; font-weight: bold; } "
                  "QTextEdit  { color: blue; font-size: 12px; font-weight: bold; }" );

    m_info = new QLabel(this);
    m_textEdit = new QTextEdit(this);
    m_dLevel = new QSpinBox(this);
    m_nEnemies = new QSpinBox(this);
    m_autoSave = new QCheckBox(tr("Speicher aktiv   "), this);
    m_label1 = new QLabel(tr("Schwierigkeit:"));
    m_label2 = new QLabel(tr("Feinde:"));
    m_FDiaButton = new QPushButton(tr("Speicherort der Daten"));
    m_okButton = new QPushButton(tr("zurück"));

    if(!m_info || !m_textEdit || !m_dLevel || !m_nEnemies || !m_autoSave ||
        !m_label1 || !m_label2 || !m_FDiaButton || !m_okButton ){
        berror = true;
    }

    if(!berror){

        m_dLevel->setRange(0, m_view->getGame()->getAPP_MAX_DLEVEL());
        m_nEnemies->setRange(min, max);

        QHBoxLayout* layoutLabel = new QHBoxLayout();
        QHBoxLayout* layoutEdit = new QHBoxLayout();
        QHBoxLayout* layout = new QHBoxLayout();
        QHBoxLayout* layoutButton = new QHBoxLayout();

        m_vertiLabelGroupBox = new QGroupBox(tr(""));
        m_vertiEditGroupBox = new QGroupBox(tr(""));
        m_vertiOptionsGroupBox = new QGroupBox(tr(""));
        m_vertiButtonGroupBox = new QGroupBox(tr(""));
        m_mainLayoutIn1Spalte = new QVBoxLayout;

        m_info->setText(tr("Speicherort der Spieldaten:"));
        layoutLabel->addWidget(m_info);

        m_textEdit->setText(m_view->getController()->getBackground()->getPath());
        m_textEdit->setMaximumHeight(2 * m_textEdit->fontMetrics().height());
        m_textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        m_textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        m_textEdit->setReadOnly(true);
        layoutEdit->addWidget(m_textEdit);

        layout->addWidget(m_label1);
        layout->addWidget(m_dLevel);
        layout->addWidget(m_autoSave);
        layout->addWidget(m_label2);
        layout->addWidget(m_nEnemies);

        layoutButton->addWidget(m_FDiaButton);
        layoutButton->addWidget(m_okButton);

        m_vertiLabelGroupBox ->setLayout(layoutLabel);
        m_vertiEditGroupBox ->setLayout(layoutEdit);
        m_vertiOptionsGroupBox->setLayout(layout);
        m_vertiButtonGroupBox->setLayout(layoutButton);

        m_mainLayoutIn1Spalte->addWidget(m_vertiLabelGroupBox);
        m_mainLayoutIn1Spalte->addWidget(m_vertiEditGroupBox);
        m_mainLayoutIn1Spalte->addWidget(m_vertiOptionsGroupBox);
        m_mainLayoutIn1Spalte->addWidget(m_vertiButtonGroupBox);

        setLayout(m_mainLayoutIn1Spalte);
    
        if(m_view->getController()->getBackground()->getautoSaveNew() == 1){
            m_autoSave->setCheckState(Qt::Checked);}
        else{
            m_autoSave->setCheckState(Qt::Unchecked);}

        m_dLevel->setValue(m_view->getController()->getBackground()->getdLevelNew());
        m_nEnemies->setValue(m_view->getController()->getBackground()->getnEnemiesNew());
     
        QMetaObject::Connection connection = QObject::connect(this, &Optionen::nEnemiesChanged, m_view, &GameView::onOptionNenemiesChanged);
        if (!connection) {
            Game* game = (Game*)qApp;
            game->setError();
        }

        connection = QObject::connect(m_okButton, &QPushButton::clicked, this, &Optionen::onOkButtonClicked);
        if (!connection) {
            Game* game = (Game*)qApp;
            game->setError();
        }

        connection = QObject::connect(m_FDiaButton, &QPushButton::clicked, this, &Optionen::onDiaButtonClicked);
        if (!connection) {
            Game* game = (Game*)qApp;
            game->setError();
        }
    }
}

Optionen::~Optionen() {

}


bool Optionen::getError() const {
    return berror;
}


GameView* const Optionen::getView() const {
    return m_view;
}


QCheckBox* const Optionen::getAutoNext() const {
    return m_autoSave;
}


QSpinBox* const Optionen::getDlevel() const {
    return m_dLevel;
}


QSpinBox* const Optionen::getEnemies() const {
    return m_nEnemies;
}


void Optionen::onCheckedAutoNext(int dummy) const {

    Q_UNUSED(dummy)
        m_view->getController()->getBackground()->setautoSaveNew(m_autoSave->isChecked()); 
}


void Optionen::onDlevelClicked(int dummy) const {

    Q_UNUSED(dummy)
        m_view->getController()->getBackground()->setdLevelNew(m_dLevel->value()); 
}


void Optionen::onNenemiesClicked(int dummy) {

    Q_UNUSED(dummy)
        m_view->getController()->getBackground()->setnEnemiesNew(m_nEnemies->value()); 
}


void Optionen::onOkButtonClicked() {

    m_view->getController()->getWelcome()->setVisible(true);
    accept();
    emit nEnemiesChanged();
    
}


void Optionen::onDiaButtonClicked() {

    const QStringList filters({ "Text files (*.json)" });

    if (getView()) {

        m_fdia = QSharedPointer<FDialog>(new FDialog(nullptr, getView()));
    }
    else
    {
        QMessageBox::critical(nullptr, tr("Fehler #op01"),
            tr("Sie können keine Änderung des Speicherorts vornehmen. Bitte versuchen sie es "
                "erneut. Wenn das Problem weiterhin besteht melden Sie sich bitte beim Support. "));
        return;
    }

    if (!m_fdia) {

        QMessageBox::critical(nullptr, tr("Fehler #op02"),
            tr("Sie können keine Änderung des Speicherorts vornehmen. Bitte versuchen sie es "
                "erneut. Wenn das Problem weiterhin besteht melden Sie sich bitte beim Support. "));
        return;
    }

    QMetaObject::Connection connection = QObject::connect(m_fdia.data(), &FDialog::updatePath, this, &Optionen::onUpdatePath);

    if (!connection) {
        m_fdia.data()->setError();
    }

    if (m_fdia && m_fdia->getError()) {

        QMessageBox::critical(nullptr, tr("Fehler #op03"),
            tr("Sie können keine Änderung des Speicherorts vornehmen. Bitte versuchen sie es "
                "erneut. Wenn das Problem weiterhin besteht melden Sie sich bitte beim Support. "));
        return;
    }

    m_fdia.data()->setOptions(FDialog::DontUseNativeDialog);
    m_fdia.data()->setViewMode(FDialog::List);
    m_fdia.data()->setDirectory(m_view->getController()->getBackground()->getPath());
    DiaStyle ds;
    m_fdia.data()->setStyle(&ds);
    m_fdia.data()->setNameFilters(filters);
    m_fdia.data()->exec();
}

void Optionen::onUpdatePath() {
    m_textEdit->setText(m_view->getController()->getBackground()->getPath());

}


void Optionen::showEvent(QShowEvent* event){

    QDialog::showEvent(event);

    QRectF re = m_view->getController()->getWelcome()->getGeo();
    qreal xDiff = re.width() - this->width();
    qreal yDiff = re.height() - this->height();

    this->move(re.x() + xDiff/2.0, re.y() + yDiff/2.0);
    m_view->getController()->getWelcome()->setVisible(false);
}


void Optionen::paintEvent(QPaintEvent* event)
{
    QDialog::paintEvent(event);

    paintTitlebar();
    paintRoundBackground(m_view->getGame()->getDIA_Back());
    adjustLayout();

}





