#include "fdia.h"
#include "game.h"
#include "gameview.h"
#include <QVBoxLayout>
#include <QResizeEvent>
#include <QPainterPath>
#include <QPainter>
#include <QFileIconProvider>
#include <QIcon>
#include <QLabel>
#include <QDialogButtonBox>
#include <QFileSystemModel>
#include <QLineEdit>

#include <QApplication>
#include <QPushButton>
#include <QMessageBox>
#include <QComboBox>
#include <QTimer>

FDialog::FDialog(QWidget* parent, GameView* view)
    : QFileDialog(parent), m_view(view), bpaintRoundBackground(!false), bdialogmodified(false),
    berror(false), bSingleshot(false), messageBoxAppeared(false){

    setWinHints();
    picBackground(m_view->getGame()->getDIA_Back());

    setStyleSheet("QLabel, QAbstractButton, QGroupBox, QRadioButton, QCheckBox { color: magenta; font-size: 15px; font-weight: bold; }");

    m_back2Button = new QPushButton(tr("Akzeptieren"), this);
    m_cancelButton = new QPushButton(tr("Stornieren"), this);

    if(m_view->getController()->getBackground()){
        m_path = m_view->getController()->getBackground()->getPath();
    }
    else{
        m_path = QCoreApplication::applicationDirPath();
        berror = true;
    }

    QMetaObject::Connection connection = connect(m_back2Button, &QPushButton::clicked, this, &FDialog::onBack2ButtonClicked);
    if (!connection) {
        m_view->getGame()->setError();
    }

    connection = connect(m_cancelButton, &QPushButton::clicked, this, &FDialog::onCancelButtonClicked);
    if (!connection) {
        m_view->getGame()->setError();
    }

    connection = connect(this, &QFileDialog::directoryEntered, this, &FDialog::onDirectoryEntered);
    if (!connection) {
        m_view->getGame()->setError();
    }
}


FDialog::~FDialog(){
    if(m_lineEdit){
        m_lineEdit->removeEventFilter(this);}
}


void FDialog::setError(){
    berror = true;
}


bool FDialog::getError() const {
    return berror;
}


void FDialog::accept(){
    //do nothing here, we don't need QDialog::accept()
}


bool FDialog::eventFilter(QObject* obj, QEvent* event){

    if (obj->objectName() == "fileNameEdit" && event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
        {
            QLineEdit* lineEdit = qobject_cast<QLineEdit*>(obj);
            if (lineEdit)
            {
                QString path = lineEdit->text();
                QFileInfo fileInfo(path);
                if (fileInfo.isDir())
                {
                    QComboBox* comboBox = this->findChild<QComboBox*>("lookInCombo");
                    if (comboBox)
                    {
                        int currentIndex = comboBox->currentIndex();
                        comboBox->clear();

                        //ComboBox update
                        m_listView->setRootIndex(m_model->index(path));
                        comboBox->setEditable(true);
                        comboBox->addItem(path);

                        //Restore the original index if possible
                        if (currentIndex >= 0 && currentIndex < comboBox->count()){
                            comboBox->setCurrentIndex(currentIndex);
                        }

                        return true;
                    }
                }
            }
        }
    }
    return QObject::eventFilter(obj, event);
}


void FDialog::onBack2ButtonClicked(){

    m_view->getController()->getBackground()->setPath(m_path);
    emit updatePath();
    QDialog::accept();
}


void FDialog::onCancelButtonClicked(){

    QDialog::reject();
}


void FDialog::onDirectoryEntered(const QString& directory){
    m_path = directory;
    if(m_lineEdit){
            m_lineEdit->setText(m_path);
    }
}


void FDialog::disableAllControls() const {

    const QObjectList& children = this->children();
    for (QObject* child : children) {
            QWidget* widget = qobject_cast<QWidget*>(child);
            if (widget) {
                widget->setEnabled(false);
            }
    }
    if(m_back2Button){
        m_back2Button->setVisible(false);
    }

    if(m_cancelButton){
        m_cancelButton->setEnabled(true);
    }
}


void FDialog::showEvent(QShowEvent *event){

    QFileDialog::showEvent(event);

    if(bdialogmodified){
        return;
    }

    bool error = false;

    if(!berror){
        QListView* treeView = this->findChild<QListView*>("listView");
        if (treeView) {
            QAbstractItemModel* model = treeView->model();
            if (model) {
                if (model->inherits("QFileSystemModel")) {
                    QFileSystemModel* fileSystemModel = qobject_cast<QFileSystemModel*>(model);
                    if (fileSystemModel) {
                        m_model = fileSystemModel;
                        m_listView = treeView;
                    }
                    else{
                        error = true;}
                }
                else{
                    error = true;}
            }
            else{
                error = true;}
        }
        else{
            error = true;}
    }
    else
    {
        error = true;}


    if(error){
        disableAllControls();
    }

    QWidget* widget = this->findChild<QWidget*>("fileNameEdit");
    if (widget)
    {
        QLineEdit* textline = dynamic_cast<QLineEdit*>(widget);
        if(textline) {
            m_lineEdit = textline;
            m_lineEdit->setCompleter(nullptr);
            m_lineEdit->setText(m_path);
            m_lineEdit->installEventFilter(this);
        }
    }

    widget = this->findChild<QWidget*>("listModeButton");
    if (widget) {
        widget->hide();
    }

    widget = this->findChild<QWidget*>("detailModeButton");
    if (widget) {
        widget->hide();
    }

    widget = this->findChild<QWidget*>("fileTypeLabel");
    if (widget) {
        widget->hide();
    }

    widget = this->findChild<QWidget*>("fileTypeCombo");
    if (widget) {
        widget->setFixedSize(0,0);
    }

    widget = this->findChild<QWidget*>("fileNameLabel");
    if (widget) {
        widget->hide();
    }

    QDialogButtonBox* buttonBox = this->findChild<QDialogButtonBox*>();

    if (buttonBox) {

        QAbstractButton* acceptButton = buttonBox->button(QDialogButtonBox::Open);
        if (acceptButton) {
            buttonBox->removeButton(acceptButton);
        }

        QAbstractButton* cancelButton = buttonBox->button(QDialogButtonBox::Cancel);
        if (cancelButton) {
            buttonBox->removeButton(cancelButton);
        }

        QLayout* oldLayout = buttonBox->layout();
        if (oldLayout) {
            delete oldLayout;
        }
        buttonBox->setFixedSize(0,0);
    }



    if(!messageBoxAppeared && (!m_lineEdit || !m_cancelButton || !m_back2Button)){

        QMessageBox::critical(nullptr, tr("Fehler #fd01"),
                                       tr("Sie können keine Änderung des Speicherorts vornehmen. Bitte versuchen sie es "
                                          "erneut. Wenn das Problem weiterhin besteht melden Sie sich bitte beim Support. "

                                          ));
        messageBoxAppeared = true;
        return;
    }
    else if(!messageBoxAppeared && berror)
    {
        QMessageBox::critical(nullptr, tr("Fehler #fd02"),
                                       tr("Sie können keine Änderung des Speicherorts vornehmen. Bitte versuchen sie es "
                                          "erneut. Wenn das Problem weiterhin besteht melden Sie sich bitte beim Support. "
                                          "Dieses Bedienfeld sperrt nun alles bis auf die Schaltfläche Stonieren. "
                                          "Es beendet sich nachdem Sie mit Stonieren bestätigt haben."
                                          ));
    }

    bdialogmodified = true;
}

void FDialog::paintEvent(QPaintEvent* event){

    if(!m_lineEdit || !m_cancelButton || !m_back2Button){

        if(!bSingleshot){
            bSingleshot = true;
            QTimer::singleShot(100, this, &QDialog::reject);
            return;
        }
    }

    if(!bdialogmodified){
        return;
    }

    QFileDialog::paintEvent(event);

    Game* game = (Game*)qApp;
    paintTitlebar();
    paintRoundBackground(game->getDIA_Back());
    if(bdialogmodified){
        adjustLayout();
    }
}


void FDialog::resizeEvent(QResizeEvent* event){

    QSize fixedSize(540, 570);
    resize(fixedSize);
    event->ignore();
}

