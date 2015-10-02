#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "renderer.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // sets up the ui file created by Qt Designer
    ui->setupUi(this);

    m_drawGroup = new QActionGroup(this);
    m_drawGroup->setExclusive(true);

    QList<QAction*> actions = ui->menuDraw->actions();
    foreach(QAction* action, actions)
    {
        m_drawGroup->addAction(action);
    }

    m_timer = new QTimer(this);
    m_timer->setInterval(500);
    m_paused = false;

    // Connects actions that were created in Qt Designer to appropriate slots
    connectActions();

    // Create the main drawing object
    m_renderer = new Renderer();

    // Setup the application's widget collection
    QVBoxLayout* layout = new QVBoxLayout();

    //add renderer
    layout->addWidget(m_renderer);
    m_renderer->setMinimumSize(300, 600);

    QWidget* mainWidget = new QWidget(this);
    mainWidget->setLayout(layout);
    setCentralWidget(mainWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if(!m_paused)
    {
        if(event->key() == Qt::Key_Left)
        {
            m_renderer->MoveBlockLeft();
        }

        else if(event->key() == Qt::Key_Right)
        {
            m_renderer->MoveBlockRight();
        }

        else if(event->key() == Qt::Key_Up)
        {
            m_renderer->RotateBlockCCW();
        }

        else if(event->key() == Qt::Key_Down)
        {
            m_renderer->RotateBlockCW();
        }

        else if(event->key() == Qt::Key_Space)
        {
            m_renderer->DropPiece();
        }

        else if(event->key() == Qt::Key_Backspace)
        {
            m_renderer->Tick();
        }
    }
}

void MainWindow::connectActions()
{
    connect(ui->actionQuit, &QAction::triggered, qApp, &QCoreApplication::quit);
    connect(ui->actionNewGame, &QAction::triggered, this, &MainWindow::startNewGame);
    connect(ui->actionWireframe, &QAction::triggered, this, &MainWindow::wireframeMode);
    connect(ui->actionFace, &QAction::triggered, this, &MainWindow::faceMode);
    connect(ui->actionSpeedUp, &QAction::triggered, this, &MainWindow::increaseSpeed);
    connect(ui->actionSlowDown, &QAction::triggered, this, &MainWindow::decreaseSpeed);
}

void MainWindow::startNewGame()
{
    // initialize the game
    m_renderer->CreateNewGame();
    connect(m_timer, &QTimer::timeout, this, &MainWindow::tick, Qt::UniqueConnection);
    connect(ui->actionPause, &QAction::triggered, this, &MainWindow::pause);
    m_timer->start();
}

void MainWindow::tick()
{
    // tick the game if it's not paused
    m_renderer->Tick();
}

void MainWindow::pause()
{
    if(m_timer->isActive())
    {
        m_timer->stop();
    }
    else
    {
        m_timer->start();
    }
    m_paused = !(m_timer->isActive());
}

void MainWindow::wireframeMode()
{
    m_renderer->WireframeMode();
}

void MainWindow::faceMode()
{
    m_renderer->FaceMode();
}

void MainWindow::increaseSpeed()
{
    int time = m_timer->interval() - 50;
    if(time > 0)
    {
       m_timer->setInterval(time);
    }
}

void MainWindow::decreaseSpeed()
{
    int time = m_timer->interval() + 50;
    m_timer->setInterval(time);
}
