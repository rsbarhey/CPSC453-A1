#include "Shapes.h"
#include <QColor>

Shapes::Shapes(QObject *parent) : QObject(parent)
{
    float cube[] = {
        0.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        1.0, 1.0, 0.0,              //First Trinagle of the back face

        1.0, 1.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 0.0,              //Second Trinagle of the back face


        0.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        0.0, 0.0, 1.0,              //First Triangle of the bottom face

        0.0, 0.0, 1.0,
        1.0, 0.0, 1.0,
        1.0, 0.0, 0.0,              //Second Triangle of the bottom face


        0.0, 0.0, 0.0,
        0.0, 0.0, 1.0,
        0.0, 1.0, 1.0,              //Frist Triangle of the left face

        0.0, 1.0, 1.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 0.0,              //Second Triangle of the left face


        0.0, 0.0, 1.0,
        1.0, 0.0, 1.0,
        1.0, 1.0, 1.0,              //First trinagle of the front face

        1.0, 1.0, 1.0,
        0.0, 1.0, 1.0,
        0.0, 0.0, 1.0,              //Second triangel of the front face


        0.0, 1.0, 1.0,
        1.0, 1.0, 1.0,
        1.0, 1.0, 0.0,

        1.0, 1.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 1.0, 1.0,

        1.0, 1.0, 1.0,
        1.0, 1.0, 0.0,
        1.0, 0.0, 0.0,

        1.0, 0.0, 0.0,
        1.0, 0.0, 1.0,
        1.0, 1.0, 1.0
    };

    m_cube.insert(m_cube.end(), cube, cube + sizeof(cube) / sizeof(float));

    QColor borderColour = Qt::red;
    float colourList [] = { (float)borderColour.redF(), (float)borderColour.greenF(), (float)borderColour.blueF() };
    int triCount = 1;
    for (int v = 0; v < m_cube.size()/3; v++)
    {
        m_cubeColor.insert(m_cubeColor.end(), colourList, colourList + 3); // 3 coordinates per vertex
        triCount++;
    }
}

vector<float> Shapes::Cube()
{
    return m_cube;
}

vector<float> Shapes::CubeColor()
{
    return m_cubeColor;
}
