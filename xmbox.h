#ifndef XMBOX_H
#define XMBOX_H
#include "xrrxbutton.h"
#include <QDialog>
#include <QLabel>
#include <QPainterPath>
#include <QVBoxLayout>

class MBox: public QDialog {

    Q_OBJECT  

public:

    explicit MBox(QWidget* parent = nullptr, int opt = 0);
    ~MBox();

    GIFButton* const getSameAgainButton() const;
    GIFButton* const getStartButton() const;

    bool getError() const;

protected:

    virtual void showEvent(QShowEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;

private:

    QLabel* m_info_label = nullptr;
    QVBoxLayout* m_info_layout = nullptr;
    GIFButton* m_sameAgainButton = nullptr;
    GIFButton* m_StartButton = nullptr;
    GIFButton* m_cancelButton = nullptr;

    QString m_pathPicBackg;

    int m_option;
    bool berror;

    void picBackground(QString& path);
    void setWinHints();
    void paintTitlebar();
    void paintRoundBackground(QString& path);
};
#endif // XMBOX_H

