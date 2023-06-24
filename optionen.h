#ifndef OPTIONEN_H
#define OPTIONEN_H
#include <QSpinBox>
#include <QCheckBox>
#include <QDialog>
#include <QPushButton>
#include <QFileDialog>

#include <QGroupBox>
#include <QBoxLayout>
#include <QTextEdit>
#include <QLabel>

class Welcome;
class GameView;
class FDialog;

class Optionen : public QDialog {

    Q_OBJECT

public:

    explicit Optionen(GameView* view,int min, int max, QWidget* parent = nullptr);
    ~Optionen();

    QCheckBox* const getAutoNext() const;
    QSpinBox* const getDlevel() const;
    QSpinBox* const getEnemies() const;

    GameView* const getView() const;
    bool getError() const;

protected:

    virtual void paintEvent(QPaintEvent* event) override;
    virtual void showEvent(QShowEvent* event) override;

private:

    QSharedPointer<FDialog> m_fdia;

    GameView* m_view = nullptr;

    QCheckBox* m_autoSave = nullptr;
    QSpinBox* m_dLevel = nullptr;
    QSpinBox* m_nEnemies = nullptr;

    QPushButton* m_FDiaButton = nullptr;
    QPushButton* m_okButton = nullptr;
    QTextEdit* m_textEdit = nullptr;
    QLabel* m_info = nullptr;
    QLabel* m_label1 = nullptr;
    QLabel* m_label2 = nullptr;

    QVBoxLayout* m_mainLayoutIn1Spalte = nullptr;
    QGroupBox* m_vertiLabelGroupBox = nullptr;
    QGroupBox* m_vertiEditGroupBox = nullptr;
    QGroupBox* m_vertiOptionsGroupBox = nullptr;
    QGroupBox* m_vertiButtonGroupBox = nullptr;

    bool berror;

    void picBackground(QString path);
    void setWinHints();
    void paintTitlebar();
    void paintRoundBackground(QString path);
    void adjustLayout() const;

private slots:

    void onCheckedAutoNext(int dummy) const;
    void onDlevelClicked(int dummy) const;
    void onNenemiesClicked(int dummy);
    void onOkButtonClicked();
    void onDiaButtonClicked();
    void onUpdatePath();

private: 

    signals:

        void nEnemiesChanged();

};
#endif //OPTIONEN_H


