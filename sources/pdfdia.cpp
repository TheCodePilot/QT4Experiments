#include "pdfdia.h"
#include "game.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QScrollBar>
#include <QLabel>
#include <QPixmap>
#include <QHBoxLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QPainterPath>
#include <QPainter>
#include <QApplication>
#include <QTimer>
#include <QMessageBox>
#include <QStyle>

#include <QPdfDocument>
#include <QPdfDocumentRenderOptions>

PDFDia::PDFDia(QWidget* parent)
    : QDialog(parent),berror(false)
{
    m_game = (Game*)qApp;

    if(!m_game) {
        berror = true;
        return;
    }
    else
    {
        m_mainLayoutIn1Spalte = new QVBoxLayout;

        m_vertiLabelGroupBox = new QGroupBox(tr(""));
        m_layoutLabel = new QHBoxLayout();
        m_info = new QLabel(this);

        m_vertiEditGroupBox = new QGroupBox(tr(""));
        m_layoutPdf = new QHBoxLayout();
        m_scrollArea = new QScrollArea(this);
        m_label = new QLabel(m_scrollArea);

        m_vertiButtonGroupBox = new QGroupBox(tr(""));
        m_layoutButton = new QHBoxLayout();
        m_okButton = new QPushButton(tr("Verstanden"),this);

    }

    if(!m_mainLayoutIn1Spalte || !m_vertiLabelGroupBox || !m_layoutLabel ||
       !m_vertiEditGroupBox || !m_layoutPdf || !m_scrollArea || !m_label ||
       !m_vertiButtonGroupBox || !m_layoutButton || !m_okButton){

        berror = true;
    }
    else
    {
        setWinHints();
        picBackground(m_game->getDIA_Back());

        setStyleSheet("QLabel, QAbstractButton, QGroupBox, QRadioButton, QCheckBox  { color: magenta; font-size: 15px; font-weight: bold; } "
                      "QTextEdit  { color: blue; font-size: 12px; font-weight: bold; }" );

        m_info->setText(tr("Information zu dEmO:"));
        m_info->setAlignment(Qt::AlignHCenter);

        m_layoutLabel->addWidget(m_info);
        m_vertiLabelGroupBox ->setLayout(m_layoutLabel);

        m_label->adjustSize();
        m_scrollArea->setWidget(m_label);
        m_scrollArea->setWidgetResizable(true);
        m_scrollArea->verticalScrollBar()->setValue(30);

        m_layoutPdf->addWidget(m_scrollArea);
        m_vertiEditGroupBox->setLayout(m_layoutPdf);

        m_layoutButton->addWidget(m_okButton);
        m_vertiButtonGroupBox ->setLayout(m_layoutButton);

        m_mainLayoutIn1Spalte->addWidget(m_vertiLabelGroupBox);
        m_mainLayoutIn1Spalte->addWidget(m_vertiEditGroupBox);
        m_mainLayoutIn1Spalte->addWidget(m_vertiButtonGroupBox);

        setLayout(m_mainLayoutIn1Spalte);

        QMetaObject::Connection connection = QObject::connect(m_okButton, &QPushButton::clicked, this, &PDFDia::onOkButtonClicked);
        if (!connection) {
            berror = true;
        }

        connection = QObject::connect(m_scrollArea->verticalScrollBar(), &QScrollBar::valueChanged, this, &PDFDia::onScrollValueChanged);
        if (!connection) {
            berror = true;
        }
    }
}


bool PDFDia::getError() const {
    return berror;
}


void PDFDia::onOkButtonClicked() {
    accept();
}


void PDFDia::showEvent(QShowEvent* event){

    QDialog::showEvent(event);

    if (m_label->isVisible()) {
        convert2Pic(1);
        int middleValue = (m_scrollArea->horizontalScrollBar()->minimum() + m_scrollArea->horizontalScrollBar()->maximum()) / 2;
        m_scrollArea->horizontalScrollBar()->setValue(middleValue);
    }
}


void PDFDia::paintEvent(QPaintEvent* event)
{
    QDialog::paintEvent(event);

    paintTitlebar();
    paintRoundBackground(m_game->getDIA_Back());
    adjustLayout();
}


void PDFDia::onScrollValueChanged(int value)
{
    if (value == m_scrollArea->verticalScrollBar()->maximum()) {

        m_label->clear();

        convert2Pic(2);
        m_label->adjustSize();
        m_scrollArea->update();
        m_scrollArea->verticalScrollBar()->setValue(30);

    } else if (value == 0) {

        m_label->clear();
        convert2Pic(1);
        m_label->adjustSize();
        m_scrollArea->update();
    }
}

