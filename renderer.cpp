#include "renderer.h"
#include "game.h"
#include <QTextStream>
#include <QOpenGLBuffer>
#include <cmath>

#define MAX_SCALE_FACTOR         2125
#define MIN_SCALE_FACTOR         -2125

// constructor
Renderer::Renderer(QWidget *parent)
    : QOpenGLWidget(parent)
{
    m_game = NULL;
    polygonMode = GL_FILL;
    isMulticolored = false;

    timer = new QTimer(this);
    timer->setInterval(20);
    connect(timer, &QTimer::timeout, timer, &QTimer::stop);

    persistenceTimer = new QTimer(this);
    persistenceTimer->setInterval(20);
    connect(persistenceTimer, SIGNAL(timeout()), this, SLOT(rotateView()));

    angle = 0.0;
    totalScaleFactor = 0;
}

// constructor
Renderer::~Renderer()
{
    delete m_game;
}

void Renderer::CreateNewGame()
{
    if(m_game == NULL)
    {
        m_game = new Game(10, 24);
    }
    else
    {
        m_game->reset();
    }
}

void Renderer::Tick()
{
    if(m_game == NULL)
    {
        return;
    }
    m_game->tick();

    update();
}

void Renderer::RotateBlockCW()
{
    m_game->rotateCW();
    update();
}

void Renderer::RotateBlockCCW()
{
    m_game->rotateCCW();
    update();
}

void Renderer::MoveBlockLeft()
{
    m_game->moveLeft();
    update();
}

void Renderer::MoveBlockRight()
{
    m_game->moveRight();
    update();
}

void Renderer::DropPiece()
{
    if(m_game->drop())
    {
        update();
    }
}

void Renderer::WireframeMode()
{
    // For some reason this doesn't work and needs to be toggled in the paint function
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    polygonMode = GL_LINE;
    update();
}

void Renderer::FaceMode()
{
    polygonMode = GL_FILL;
    isMulticolored = false;
    update();
}

void Renderer::MulticoloredMode()
{
    polygonMode = GL_FILL;
    isMulticolored = true;
}

// called once by Qt GUI system, to allow initialization for OpenGL requirements
void Renderer::initializeGL()
{
    // Qt support for inline GL function calls
	initializeOpenGLFunctions();

    // sets the background clour
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

    // links to and compiles the shaders, used for drawing simple objects
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Shaders/Phong/per-fragment-phong.vs.glsl");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Shaders/Phong/per-fragment-phong.fs.glsl");
    m_program->link();
    m_posAttr = m_program->attributeLocation("position_attr");
    m_colAttr = m_program->attributeLocation("colour_attr");
    m_norAttr = m_program->attributeLocation("normal_attr");
    m_PMatrixUniform = m_program->uniformLocation("proj_matrix");
    m_VMatrixUniform = m_program->uniformLocation("view_matrix");
    m_MMatrixUniform = m_program->uniformLocation("model_matrix");
    m_programID = m_program->programId();

    setupCube();
}

// called by the Qt GUI system, to allow OpenGL drawing commands
void Renderer::paintGL()
{
    // Clear the screen buffers

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    // Set the current shader program

    glUseProgram(m_programID);

    // Modify the current projection matrix so that we move the
    // camera away from the origin.  We'll draw the game at the
    // origin, and we need to back up to see it.

    QMatrix4x4 view_matrix;
    view_matrix.translate(0.0f, 0.0f, -40.0f);

    view_matrix = view_matrix * scalingMatrix;
    view_matrix = view_matrix * roataionMatrix;


    glUniformMatrix4fv(m_VMatrixUniform, 1, false, view_matrix.data());

    // Not implemented: set up lighting (if necessary)

    // Not implemented: scale and rotate the scene

    QMatrix4x4 model_matrix;

    // You'll be drawing unit cubes, so the game will have width
    // 10 and height 24 (game = 20, stripe = 4).  Let's translate
    // the game so that we can draw it starting at (0,0) but have
    // it appear centered in the window.

    model_matrix.translate(-5.0f, -12.0f, 0.0f);
    // Not implemented: actually draw the current game state.
    // Here's some test code that draws red triangles at the
    // corners of the game board.
    // draw border

    changeCubeColor(7);
    if (cube.CubeVertices().size() > 0)
    {
        glEnableVertexAttribArray(m_posAttr);
        glEnableVertexAttribArray(m_colAttr);
        glEnableVertexAttribArray(m_norAttr);

        QMatrix4x4 boxMatrix(model_matrix);
        // translated back once along the x axis since we start with a translation
        boxMatrix.translate(-2.0, 0.0, 0.0);
        for (int i = 0; i < 12; i++)
        {
            boxMatrix.translate(1.0, 0.0, 0.0);
            glUniformMatrix4fv(m_MMatrixUniform, 1, false, boxMatrix.data());
            glDrawArrays(GL_QUADS, 0, cube.CubeVertices().size()/3); // 3 coordinates per vertex
        }

        for (int i = 0; i < 24; i++)
        {
            boxMatrix.translate(0.0, 1.0, 0.0);
            glUniformMatrix4fv(m_MMatrixUniform, 1, false, boxMatrix.data());
            glDrawArrays(GL_QUADS, 0, cube.CubeVertices().size()/3); // 3 coordinates per vertex
        }

        // Reset back to the bottom left corner (reversing from the previous two loops
        boxMatrix.translate(-11.0, -24.0, 0.0);
        for (int i = 0; i < 24; i++)
        {
            boxMatrix.translate(0.0, 1.0, 0.0);
            glUniformMatrix4fv(m_MMatrixUniform, 1, false, boxMatrix.data());
            glDrawArrays(GL_QUADS, 0, cube.CubeVertices().size()/3); // 3 coordinates per vertex
        }

        //reset the model back to 0,0,0
        boxMatrix.translate(0.0, -24.0, 0.0);

        // transelate one step in x and y to avoid border overlap
        boxMatrix.translate(1.0, 1.0, 0.0);

        for (int i = 0; i<24; i++)
        {
            for (int j = 0; j<10; j++)
            {
                if (m_game != NULL && m_game->get(i, j) != -1)
                {
                    if(isMulticolored)
                    {
                        setMultipleColors(m_game->get(i, j));
                    }
                    else
                    {
                        changeCubeColor(m_game->get(i, j));
                    }

                    boxMatrix.translate((float)j, (float)i, 0.0);
                    glUniformMatrix4fv(m_MMatrixUniform, 1, false, boxMatrix.data());
                    glDrawArrays(GL_QUADS, 0, cube.CubeVertices().size()/3); // 3 coordinates per vertex
                    boxMatrix.translate((float)-j, (float)-i, 0.0);
                }
            }
        }

        glDisableVertexAttribArray(m_norAttr);
        glDisableVertexAttribArray(m_colAttr);
        glDisableVertexAttribArray(m_posAttr);
    }

    // deactivate the program
    m_program->release();

}

// called by the Qt GUI system, to allow OpenGL to respond to widget resizing
void Renderer::resizeGL(int w, int h)
{
    // width and height are better variables to use
    Q_UNUSED(w); Q_UNUSED(h);

    // update viewing projections
    glUseProgram(m_programID);

    // Set up perspective projection, using current size and aspect
    // ratio of display
    QMatrix4x4 projection_matrix;
    projection_matrix.perspective(40.0f, (GLfloat)width() / (GLfloat)height(),
                                  0.1f, 1000.0f);
    glUniformMatrix4fv(m_PMatrixUniform, 1, false, projection_matrix.data());

    glViewport(0, 0, width(), height());

}

// computes the vertices and corresponding colours-per-vertex for a quadrilateral
// drawn from (x1, y1) to (x2, y2)
// Note: the magic numbers in the vector insert commands should be better documented
void Renderer::generateBorderTriangles()
{
    // make sure array lists are clear to start with
    triVertices.clear();
    triColours.clear();

    // add vertices to rectangle list
    float vectList [] = {
        0.0, 0.0, 0.0,  // bottom left triangle
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,

        9.0, 0.0, 0.0,  // bottom right triangle
        10.0, 0.0, 0.0,
        10.0, 1.0, 0.0,

        0.0, 19.0, 0.0, // top left triangle
        1.0, 20.0, 0.0,
        0.0, 20.0, 0.0,

        10.0, 19.0, 0.0,    // top right triangle
        10.0, 20.0, 0.0,
        9.0, 20.0, 0.0,
    };
    triVertices.insert(triVertices.end(), vectList, vectList + 3*4*3); // 36 items in array NOTE the 8 was a 4

    // shader supports per-vertex colour; add colour for each vertex add colours to colour list - use current colour
    QColor borderColour = Qt::red;
    float colourList [] = { (float)borderColour.redF(), (float)borderColour.greenF(), (float)borderColour.blueF() };
    float normalList [] = { 0.0f, 0.0f, 1.0f }; // facing viewer
    for (int v = 0; v < 4 * 3; v++)
    {
        triColours.insert(triColours.end(), colourList, colourList + 3); // 3 coordinates per vertex
        triNormals.insert(triNormals.end(), normalList, normalList + 3); // 3 coordinates per vertex
    }

}

void Renderer::setupCube()
{
    glGenBuffers(3, vbo);        // size of vbo

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, cube.CubeVertices().size()*sizeof(GLfloat), &cube.CubeVertices()[0], GL_STATIC_DRAW);
    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, cube.CubeColor().size()*sizeof(GLfloat), &cube.CubeColor()[0], GL_STATIC_DRAW);
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, cube.CubeNormals().size()*sizeof(GLfloat), &cube.CubeNormals()[0], GL_STATIC_DRAW);
    glVertexAttribPointer(m_norAttr, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void Renderer::setupColorVbo(int id)
{
    if(!colorVbos.contains(id))
    {
        GLuint colorVbo[1];
        glGenBuffers(1, colorVbo);
        colorVbos.insert(colorVbos.end(), id, colorVbo[0]);
    }
}

void Renderer::switchColorVbo(int id)
{
    if(colorVbos.contains(id))
    {
        glBindBuffer(GL_ARRAY_BUFFER, colorVbos.value(id));
        glBufferData(GL_ARRAY_BUFFER, cube.CubeColor().size()*sizeof(GLfloat), &cube.CubeColor()[0], GL_STATIC_DRAW);
        glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }
}

void Renderer::changeCubeColor(int id)
{
    cube.ChangeCubeColor(id);
    setupColorVbo(id);      // bind correct colorVbo
    switchColorVbo(id);
}

void Renderer::setMultipleColors(int id)
{
    cube.SetMutlipleColors(id);
    setupColorVbo(9+id);
    switchColorVbo(9+id);
}

// override mouse press event
void Renderer::mousePressEvent(QMouseEvent * event)
{
    persistenceTimer->stop();
    QTextStream cout(stdout);
    cout << "Stub: Button " << event->button() << " pressed.\n";
    m_mouseStart = event->x();
    m_button = event->button();
    m_modifier = event->modifiers();
}

// override mouse release event
void Renderer::mouseReleaseEvent(QMouseEvent * event)
{
    QTextStream cout(stdout);
    cout << "Stub: Button " << event->button() << " released.\n";
    m_button = Qt::NoButton;
    if(timer->isActive() && m_modifier != Qt::ShiftModifier)
    {
        // implement presistent rotation here
        cout << timer->remainingTime() << "Time between move and release is 15ms.\n";
        persistenceTimer->start();
    }
}

// override mouse move event
void Renderer::mouseMoveEvent(QMouseEvent * event)
{
    QTextStream cout(stdout);
    cout << "Stub: Motion at " << event->x() << ", " << event->y() << ".\n";
    timer->start();
    m_mouseEnd = event->x();

    if (m_button == Qt::LeftButton && m_modifier != Qt::ShiftModifier)
    {
        rotateView(m_mouseStart, m_mouseEnd, 1);
    }

    else if(m_button == Qt::MiddleButton && m_modifier != Qt::ShiftModifier)
    {
        rotateView(m_mouseStart, m_mouseEnd, 0.0, 1.0);
    }

    else if(m_button == Qt::RightButton && m_modifier != Qt::ShiftModifier)
    {
        rotateView(m_mouseStart, m_mouseEnd, 0.0, 0.0, 1.0);
    }

    else if(m_button == Qt::LeftButton  && m_modifier == Qt::ShiftModifier)
    {
        scaleView(m_mouseStart, m_mouseEnd);
    }

    m_mouseStart = m_mouseEnd;
}

void Renderer::rotateView(int start, int end, float x, float y, float z)
{
    if(start > end)
    {
        angle = -5;
    }
    else if(start < end)
    {
        angle = 5;
    }
    this->x = x;
    this->y = y;
    this->z = z;

    roataionMatrix.rotate(angle, x, y, z);
    update();
}

void Renderer::rotateView()
{
    roataionMatrix.rotate(angle, x, y, z);
    update();
}

void Renderer::ResetView()
{
    persistenceTimer->stop();
    roataionMatrix = QMatrix4x4();
    scalingMatrix = QMatrix4x4();
    update();
}

void Renderer::scaleView(int start, int end)
{
    int scale;
    if(start > end)
    {
        scale = -25;
    }
    else if(start < end)
    {
        scale = 25;
    }

    if(totalScaleFactor + scale >= MIN_SCALE_FACTOR && totalScaleFactor + scale <= MAX_SCALE_FACTOR)
    {
        totalScaleFactor += scale;
        scalingMatrix.scale((float)scale/1000 + 1.0);
        update();
    }
}
