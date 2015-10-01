#include "renderer.h"
#include "Cube.h"
#include "game.h"
#include <QTextStream>
#include <QOpenGLBuffer>
#include <cmath>

// constructor
Renderer::Renderer(QWidget *parent)
    : QOpenGLWidget(parent)
{
    _game = NULL;
}

// constructor
Renderer::~Renderer()
{

}

void Renderer::CreateNewGame()
{
    if(_game == NULL)
    {
        _game = new Game(10, 25);
    }
    else
    {
        _game->reset();
    }
}

void Renderer::Tick()
{
    if(_game == NULL)
    {
        return;
    }
    _game->tick();

    update();
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

    generateCube();
    glGenBuffers(3, vbo);        // size of vbo

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, quadVertices.size()*sizeof(GLfloat), &quadVertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, quadColors.size()*sizeof(GLfloat), &quadColors[0], GL_STATIC_DRAW);
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, quadNormals.size()*sizeof(GLfloat), &quadNormals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(m_norAttr, 3, GL_FLOAT, GL_FALSE, 0, 0);

}

// called by the Qt GUI system, to allow OpenGL drawing commands
void Renderer::paintGL()
{
    // Clear the screen buffers

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set the current shader program

    glUseProgram(m_programID);

    // Modify the current projection matrix so that we move the
    // camera away from the origin.  We'll draw the game at the
    // origin, and we need to back up to see it.

    QMatrix4x4 view_matrix;
    view_matrix.translate(0.0f, 0.0f, -40.0f);
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
    if (quadVertices.size() > 0)
    {
        glEnableVertexAttribArray(m_posAttr);
        glEnableVertexAttribArray(m_colAttr);
        glEnableVertexAttribArray(m_norAttr);

        // translated back once along the x axis since we start with a translation
        model_matrix.translate(-1.0, 0.0, 0.0);
        for (int i = 0; i < 10; i++)
        {
            model_matrix.translate(1.0, 0.0, 0.0);
            glUniformMatrix4fv(m_MMatrixUniform, 1, false, model_matrix.data());
            glDrawArrays(GL_QUADS, 0, quadVertices.size()/3); // 3 coordinates per vertex
        }

        for (int i = 0; i < 20; i++)
        {
            model_matrix.translate(0.0, 1.0, 0.0);
            glUniformMatrix4fv(m_MMatrixUniform, 1, false, model_matrix.data());
            glDrawArrays(GL_QUADS, 0, quadVertices.size()/3); // 3 coordinates per vertex
        }

        // Reset back to the bottom left corner (reversing from the previous two loops
        model_matrix.translate(-9.0, -20.0, 0.0);
        for (int i = 0; i < 20; i++)
        {
            model_matrix.translate(0.0, 1.0, 0.0);
            glUniformMatrix4fv(m_MMatrixUniform, 1, false, model_matrix.data());
            glDrawArrays(GL_QUADS, 0, quadVertices.size()/3); // 3 coordinates per vertex
        }

        // reset it back to 0,1,0  (the one is an offset so the game blocks doesn't get clipped with the game border
        model_matrix.translate(0.0, -19.0, 0.0);

        for (int i = 0; i<24; i++)
        {
            for (int j = 0; j<10; j++)
            {
                if (_game != NULL && _game->get(i, j) != -1)
                {
                    model_matrix.translate((float)j, (float)i, 0.0);
                    glUniformMatrix4fv(m_MMatrixUniform, 1, false, model_matrix.data());
                    glDrawArrays(GL_QUADS, 0, quadVertices.size()/3); // 3 coordinates per vertex
                    model_matrix.translate((float)-j, (float)-i, 0.0);
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

void Renderer::generateCube()
{
    quadVertices.clear();
    quadColors.clear();

    Cube shape;
    vector<float> cube = shape.CubeVertices();
    vector<float> color = shape.CubeColor();
    vector<float> normal = shape.CubeNormals();

    quadVertices.insert(quadVertices.end(), cube.begin(), cube.end()); // 36 items in array NOTE the 8 was a 4

    // shader supports per-vertex colour; add colour for each vertex add colours to colour list - use current colour

    quadColors.insert(quadColors.end(), color.begin(), color.end());
    quadNormals.insert(quadNormals.end(), normal.begin(), normal.end());
}

// override mouse press event
void Renderer::mousePressEvent(QMouseEvent * event)
{
    QTextStream cout(stdout);
    cout << "Stub: Button " << event->button() << " pressed.\n";
}

// override mouse release event
void Renderer::mouseReleaseEvent(QMouseEvent * event)
{
    QTextStream cout(stdout);
    cout << "Stub: Button " << event->button() << " pressed.\n";
}

// override mouse move event
void Renderer::mouseMoveEvent(QMouseEvent * event)
{
    QTextStream cout(stdout);
    cout << "Stub: Motion at " << event->x() << ", " << event->y() << ".\n";
}
