#ifndef PDFDIA_H
#define PDFDIA_H
#include <QDialog>
#include <QPushButton>
#include <QGroupBox>
#include <QBoxLayout>
#include <QTextEdit>
#include <QLabel>
#include <QScrollArea>


class Game;

class PDFDia : public QDialog
{
    Q_OBJECT

public:

    explicit PDFDia(QWidget* parent = nullptr);

    bool getError() const;

protected:

    virtual void showEvent(QShowEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;

private:

    Game* m_game = nullptr;

    QPushButton* m_okButton = nullptr;
    QLabel* m_label = nullptr;

    QLabel* m_info = nullptr;
    QScrollArea* m_scrollArea = nullptr;

    QHBoxLayout* m_layoutLabel = nullptr;
    QHBoxLayout* m_layoutPdf = nullptr;
    QHBoxLayout* m_layoutButton = nullptr;

    QGroupBox* m_vertiLabelGroupBox = nullptr;
    QGroupBox* m_vertiEditGroupBox = nullptr;
    QGroupBox* m_vertiButtonGroupBox = nullptr;

    QVBoxLayout* m_mainLayoutIn1Spalte = nullptr;

    QImage m_qfirstPixmap;
    QImage m_qsecondPixmap;
    
    bool berror;

    void picBackground(QString path);
    void setWinHints();
    void paintTitlebar();
    void paintRoundBackground(QString path);
    void adjustLayout() const;

    void convert2Pic(int pagenr);

private slots:

    void onOkButtonClicked();
    void onScrollValueChanged(int value);

protected slots:

};
#endif // PDFDIA_H

