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

protected:
    virtual void keyPressEvent(QKeyEvent* event);

private:
    Ui::MainWindow *ui;

    TCPServer* server;
    TCPClient* client;

    Renderer* m_renderer;
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
};

#endif // MAINWINDOW_H
