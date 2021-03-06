/*
 * CPSC 453 - Introduction to Computer Graphics
 * Assignment 1
 *
 * Renderer - OpenGL widget for drawing scene
 */

#ifndef RENDERER_H
#define RENDERER_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>
#include <QMouseEvent>
#include <QTimer>

#include "Cube.h"

class Game;


using namespace std;

class Renderer : public QOpenGLWidget, protected QOpenGLFunctions
{

    // informs the qmake that a Qt moc_* file will need to be generated
    Q_OBJECT

public:
    // constructor
    Renderer(QWidget *parent = 0);

    // destructor
    virtual ~Renderer();

    //Initialize the game or reset it
    void CreateNewGame();

    //Ticks the game
    void Tick();

    // Methods for rotating the block clockwise and counter clockwise
    void RotateBlockCW();
    void RotateBlockCCW();

    // Methods for moving the blocks left and right
    void MoveBlockLeft();
    void MoveBlockRight();

    // Drop the piece
    void DropPiece();

    // Draw modes
    void WireframeMode();
    void FaceMode();
    void MulticoloredMode();

    // Reset the view
    void ResetView();

    // This is copy of the game board state from the game object
    QList<int> GameBoardState();

    // This is used only for newtwork playing updates game board state of the other player
    void SetGameBoardState(QList<int> gameBoardState);

signals:
    // Emitted when your own game board state changes (to send the new game state through the network)
    void GameBoardStateChanged(QList<int>);

protected:

    // override fundamental drawing functions

    // Called when OpenGL is first initialized
    void initializeGL();

    // Called when the window is resized
    void resizeGL(int w, int h);

    // Called when the window needs to be redrawn
    void paintGL();

    // override mouse event functions

    // Called when a mouse button is pressed
    virtual void mousePressEvent(QMouseEvent * event);

    // Called when a mouse button is released
    virtual void mouseReleaseEvent(QMouseEvent * event);

    // Called when the mouse moves
    virtual void mouseMoveEvent(QMouseEvent * event);

private slots:
    // a slot for persistence rotation
    void rotateView();
private:

    // member variables for shader manipulation
    GLuint m_programID;
    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_norAttr;
    GLuint m_MMatrixUniform; // model matrix
    GLuint m_VMatrixUniform; // view matrix
    GLuint m_PMatrixUniform; // projection matrix

    GLuint vao;
    GLuint vbo[3];          // for position color normal

    QMap<int, GLuint> colorVbos;

    QOpenGLShaderProgram *m_program;

    GLenum polygonMode;
    bool isMulticolored;

    // for storing triangle vertices and colours
    vector<GLfloat> triVertices;
    vector<GLfloat> triColours;
    vector<GLfloat> triNormals;

    Game* m_game;
    QList<int> m_gameBoard;
    Cube cube;

    QTimer* timer;
    QTimer* persistenceTimer;

    int m_mouseStart;
    int m_mouseEnd;
    Qt::MouseButton m_button;
    Qt::KeyboardModifiers m_modifier;

    float angle;
    float x,y,z;
    QMatrix4x4 roataionMatrix;

    int totalScaleFactor;
    QMatrix4x4 scalingMatrix;

    // helper function for loading shaders
    GLuint loadShader(GLenum type, const char *source);

    // helper function for drawing bordering triangles
    void generateBorderTriangles();
    void setupCube();
    void setupColorVbo(int id);
    void switchColorVbo(int id);
    void changeCubeColor(int id);
    void setMultipleColors(int id);

    void rotateView(int start, int end, float x = 0.0, float y = 0.0, float z = 0.0);
    void scaleView(int start, int end);

    void copyGameState();
};

#endif // RENDERER_H
