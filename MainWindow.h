#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QTimer>
#include <QActionGroup>

namespace Ui {
class MainWindow;
}

class Renderer;

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
    Renderer* m_renderer;
    QTimer* m_timer;

    QActionGroup* m_drawGroup;

    bool m_paused;

    // Connects ui actions
    void connectActions();
    void startNewGame();
    
    void tick();                // Tick the game
    void pause();
    void increaseSpeed();
    void decreaseSpeed();

    //Draw modes
    void wireframeMode();
    void faceMode();
    void multicoloredMode();
};

#endif // MAINWINDOW_H
