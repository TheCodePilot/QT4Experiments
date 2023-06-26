#ifndef FDIA_H
#define FDIA_H
#include <QFileDialog>
#include <QTreeView>
#include <QLabel>
#include <QListView>
#include <QLineEdit>
#include <QFileSystemModel>

class GameView;

class FDialog : public QFileDialog{

    Q_OBJECT

public:

    FDialog(QWidget* parent, GameView* view);
    ~FDialog();

    void setError();
    bool getError() const;


protected:

    virtual void paintEvent(QPaintEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;
    virtual void showEvent(QShowEvent* event) override;
    virtual void accept() override;

private:

    GameView* m_view = nullptr;

    QFileSystemModel* m_model = nullptr;

    QListView* m_listView = nullptr;
    QPushButton* m_back2Button = nullptr;
    QPushButton* m_cancelButton = nullptr;
    QLineEdit* m_lineEdit = nullptr;

    QString m_path;

    bool bpaintRoundBackground;
    bool bdialogmodified;
    bool berror;
    bool bSingleshot;
    bool messageBoxAppeared;

    void picBackground(QString path);
    void setWinHints();
    void paintTitlebar();
    void paintRoundBackground(QString path);
    void adjustLayout() const;

    bool eventFilter(QObject* obj, QEvent* event)override;
    void disableAllControls() const;
    void bottomUI() const;

private slots:

    void onDirectoryEntered(const QString& directory);
    void onBack2ButtonClicked();
    void onCancelButtonClicked();

private:
    
    signals:
        
        void updatePath();

};
#endif //FDIA_H
