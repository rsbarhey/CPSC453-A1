#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "renderer.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // sets up the ui file created by Qt Designer
    ui->setupUi(this);

    // Connects actions that were created in Qt Designer to appropriate slots
    connectActions();

    // Create the main drawing object
    _renderer = new Renderer();

    // Setup the application's widget collection
    QVBoxLayout* layout = new QVBoxLayout();

    //add renderer
    layout->addWidget(_renderer);
    _renderer->setMinimumSize(300, 600);

    QWidget* mainWidget = new QWidget(this);
    mainWidget->setLayout(layout);
    setCentralWidget(mainWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectActions()
{
    connect(ui->actionQuit, &QAction::triggered, qApp, &QCoreApplication::quit);
}
