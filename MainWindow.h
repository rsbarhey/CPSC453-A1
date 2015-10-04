#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QTimer>
#include <QActionGroup>
#include <QDialog>

namespace Ui {
class MainWindow;
}

class Renderer;
class TCPServer;
class TCPClient;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:

protected:
    virtual void keyPressEvent(QKeyEvent* event);

private slots:
    void setGameState(QList<int> gameState);

private:
    Ui::MainWindow *ui;

    TCPServer* server;
    TCPClient* client;

    Renderer* m_renderer;
    Renderer* m_secondRenderer;

    QVBoxLayout* layout;

    QTimer* m_timer;
    QTimer* m_autoIncreaseTimer;

    bool m_autoIncrease;

    QActionGroup* m_drawGroup;

    bool m_paused;

    // Connects ui actions
    void connectActions();
    void startNewGame();
    
    void tick();                // Tick the game
    void pause();
    void increaseSpeed();
    void decreaseSpeed();
    void autoIncreaseMode();

    //Draw modes
    void wireframeMode();
    void faceMode();
    void multicoloredMode();

    void resetView();

    void setupServer();
    void setupConnection();

    void connectionEstablished();
};

#endif // MAINWINDOW_H
