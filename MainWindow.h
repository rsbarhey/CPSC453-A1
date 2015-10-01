#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QVBoxLayout>

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

private:
    Ui::MainWindow *ui;
    Renderer* _renderer;

    // Connects ui actions
    void connectActions();
    void startNewGame();
    void tick();                // Tick the game
};

#endif // MAINWINDOW_H
