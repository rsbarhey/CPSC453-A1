#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "renderer.h"
#include "TCPServer.h"
#include "TCPClient.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // sets up the ui file created by Qt Designer
    ui->setupUi(this);

    //Establish QActionGroup
    m_drawGroup = new QActionGroup(this);
    //This means any checkable actions in this group will not be checked in the same time
    m_drawGroup->setExclusive(true);

    //Fill in the actiongroup with draw items (only one mode will be active at a time)
    QList<QAction*> actions = ui->menuDraw->actions();
    foreach(QAction* action, actions)
    {
        m_drawGroup->addAction(action);
    }

    //This is the game timera and by default it's 500ms
    m_timer = new QTimer(this);
    m_timer->setInterval(500);

    //This is the auto increment mode and it will increase the game speed every 5 seconds
    m_autoIncreaseTimer = new QTimer(this);
    m_autoIncreaseTimer->setInterval(5000);
    connect(m_autoIncreaseTimer, &QTimer::timeout, this, &MainWindow::increaseSpeed);
    m_autoIncrease = false;

    // Connects actions that were created in Qt Designer to appropriate slots
    connectActions();

    // Create the main drawing object
    m_renderer = new Renderer();

    // mini drawing object for multiplayer
    m_secondRenderer = NULL;

    // Setup the application's widget collection
    layout = new QVBoxLayout();

    //add renderer
    layout->addWidget(m_renderer);
    m_renderer->setMinimumSize(300, 600);

    QWidget* mainWidget = new QWidget(this);
    mainWidget->setLayout(layout);
    setCentralWidget(mainWidget);

    // a TCP server/client
    server = NULL;
    client = NULL;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    //If the game is not paused handle the following key presses
    if(!m_paused)
    {
        switch(event->key())
        {
            case Qt::Key_Left:
                m_renderer->MoveBlockLeft();
                break;

            case Qt::Key_Right:
                m_renderer->MoveBlockRight();
                break;

            case Qt::Key_Up:
                m_renderer->RotateBlockCCW();
                break;

            case Qt::Key_Down:
                m_renderer->RotateBlockCW();
                break;

            case Qt::Key_Space:
                m_renderer->DropPiece();
                break;

            case Qt::Key_Backspace:
                m_renderer->Tick();
                break;

            default:
                break;
        }
    }
}

void MainWindow::connectActions()
{
    connect(ui->actionQuit, &QAction::triggered, qApp, &QCoreApplication::quit);
    connect(ui->actionNewGame, &QAction::triggered, this, &MainWindow::startNewGame);
    connect(ui->actionWireframe, &QAction::triggered, this, &MainWindow::wireframeMode);
    connect(ui->actionFace, &QAction::triggered, this, &MainWindow::faceMode);
    connect(ui->actionMulticolor, &QAction::triggered, this, &MainWindow::multicoloredMode);
    connect(ui->actionSpeedUp, &QAction::triggered, this, &MainWindow::increaseSpeed);
    connect(ui->actionSlowDown, &QAction::triggered, this, &MainWindow::decreaseSpeed);
    connect(ui->actionAutoIncrease, &QAction::triggered, this, &MainWindow::autoIncreaseMode);
    connect(ui->actionReset, &QAction::triggered, this, &MainWindow::resetView);

    connect(ui->actionHost, &QAction::triggered, this, &MainWindow::setupServer);
    connect(ui->actionGuest, &QAction::triggered, this, &MainWindow::setupConnection);
}

void MainWindow::startNewGame()
{
    // initialize the game
    m_renderer->CreateNewGame();
    //connect the timeout to ticking the game, needs to be other wise if you clicked new game multiple times the tick will get called as many times
    connect(m_timer, &QTimer::timeout, this, &MainWindow::tick, Qt::UniqueConnection);
    connect(ui->actionPause, &QAction::triggered, this, &MainWindow::pause, Qt::UniqueConnection);
    m_timer->start();
    m_paused = false;
}

void MainWindow::tick()
{
    //tick the game
    m_renderer->Tick();
}

void MainWindow::pause()
{
    // if timer is running pause the game, as well as the autoIncrease timer, in case it's in that mode, (we don't want the game speed to increase when it's paused)
    if(m_timer->isActive())
    {
        m_timer->stop();
        m_autoIncreaseTimer->stop();
    }

    // Otherwise unpause the game
    else
    {
        m_timer->start();
        // If the game is in auto increase mode unpause this timer
        if(m_autoIncrease)
        {
            m_autoIncreaseTimer->start();
        }
    }
    // This is used for checking in other methods
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

void MainWindow::multicoloredMode()
{
    m_renderer->MulticoloredMode();
}

void MainWindow::increaseSpeed()
{
    // decrease time, in other words increase the game speed
    int time = m_timer->interval() - 50;
    // There's no such thing as negative time (unless we can time travel)
    if(time > 0)
    {
        m_timer->setInterval(time);
    }
}

void MainWindow::decreaseSpeed()
{
    int time = m_timer->interval() + 50;
    // decrease the speed and the limit is 2 seconds per tick (which is really really slow)
    if(time < 2000)
    {
        m_timer->setInterval(time);
    }
}

void MainWindow::autoIncreaseMode()
{
    if(m_autoIncreaseTimer->isActive())
    {
        m_autoIncreaseTimer->stop();
        m_autoIncrease = false;
    }
    else
    {
        m_autoIncreaseTimer->start();
        m_autoIncrease = true;
    }
}

void MainWindow::resetView()
{
    m_renderer->ResetView();
}

void MainWindow::setupServer()
{
    if(server == NULL)
    {
        server = new TCPServer(this);
    }
    connect(server, &TCPServer::ClientConnected, this, &MainWindow::connectionEstablished, Qt::UniqueConnection);
    connect(m_renderer, &Renderer::GameBoardStateChanged, server, &TCPServer::SendGameState, Qt::UniqueConnection);
    connect(server, &TCPServer::RecievedGameState, this, &MainWindow::setGameState, Qt::UniqueConnection);
}

void MainWindow::setupConnection()
{
    if(client == NULL)
    {
        client = new TCPClient(this);
    }
    connect(client, &TCPClient::ConnectedToServer, this, &MainWindow::connectionEstablished, Qt::UniqueConnection);
    connect(m_renderer, &Renderer::GameBoardStateChanged, client, &TCPClient::SendGameState, Qt::UniqueConnection);
    connect(client, &TCPClient::RecievedGameState, this, &MainWindow::setGameState, Qt::UniqueConnection);
    client->EstablishConnection();
}

void MainWindow::connectionEstablished()
{
    // initialize the second drawing object if it's null
    if(m_secondRenderer == NULL)
    {
        m_secondRenderer = new Renderer();
        layout->addWidget(m_secondRenderer);
        m_secondRenderer->setMinimumSize(150, 300);
    }
    // start new game when connection is fine
    startNewGame();
}

void MainWindow::setGameState(QList<int> gameState)
{
    // this is used for multiplayer mode
    m_secondRenderer->SetGameBoardState(gameState);
}
