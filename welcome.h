#ifndef WELCOME_H
#define WELCOME_H
#include <QDialog>
#include <QListWidget>
#include <QLabel>
#include <QGroupBox>
#include <QBoxLayout>

class GameView;
class Optionen;
class GIFButton;

class Welcome : public QDialog {

    Q_OBJECT

public:

    explicit Welcome(QWidget* parent = nullptr, GameView* gView = nullptr);
    ~Welcome();

    QRectF getGeo() const;
    bool getError() const;

    QPushButton* const getSaveButton() const;
    QPushButton* const getLoadButton() const;
    QPushButton* const getExitButton() const;
    QPushButton* const getNewButton() const;

protected:

    virtual void closeEvent(QCloseEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void showEvent(QShowEvent* event) override;

private slots:

    void onSaveButtonClicked();
    void onLoadButtonClicked();
    void onButtonOptClicked();
    void onNewButtonClicked();
    void onExitButtonClicked();
    void onBack2ButtonClicked();

private:

    QSharedPointer<Optionen> m_optionen;
    GameView* m_gameView = nullptr;

    QPushButton* m_loadButton = nullptr;
    QPushButton* m_saveButton = nullptr;
    QPushButton* m_optButton = nullptr;
    QPushButton* m_newButton = nullptr;
    QPushButton* m_exitButton = nullptr;
    QPushButton* m_back2Button = nullptr;

    QVBoxLayout* mainLayoutIn1Spalte = nullptr;

    QGroupBox* m_vertiLabelGroupBox = nullptr;
    QGroupBox* m_vertiViewListGroupBox = nullptr;
    QGroupBox* m_vertibuttons3lsoGroupBox = nullptr;
    QGroupBox* m_vertibuttons3bneGroupBox = nullptr;

    QHBoxLayout *layout3lsoButtons = nullptr;
    QHBoxLayout *layout3bneButtons = nullptr;
    QHBoxLayout *layout2Labels = nullptr;
    QVBoxLayout *layoutViewList = nullptr;

    QListWidget* m_listbox = nullptr;
    QLabel* m_score = nullptr;
    QLabel* m_level = nullptr;
    QLabel* m_scoret = nullptr;
    QLabel* m_levelt = nullptr;

    bool berror;
    bool bcontrolls;

    void connections();

    void createListBox() const;
    void createLabels() const;

    void picBackground(QString path);
    void setWinHints();
    void paintTitlebar();
    void paintRoundBackground(QString path);
    void adjustLayout() const;
    
    void ifErrorDisableControls(bool owenErrorMessage = false) const;

public slots:

    void onDisableOptionButton();
    void onEnableOptionButton();
    void onSetNewFileInfo();

private: 

    signals:

        void newGame();
        void loadGame();
};
#endif //WELCOME_H

