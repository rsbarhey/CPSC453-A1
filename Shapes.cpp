#include "Shapes.h"
#include <QColor>

Shapes::Shapes(QObject *parent) : QObject(parent)
{
    float cube[] = {
        0.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        1.0, 1.0, 0.0,
        0.0, 1.0, 0.0,

        0.0, 0.0, 0.0,
        0.0, 0.0, 1.0,
        1.0, 0.0, 1.0,
        1.0, 0.0, 0.0,

        0.0, 0.0, 1.0,
        1.0, 0.0, 1.0,
        1.0, 1.0, 1.0,
        0.0, 1.0, 1.0,

        0.0, 0.0, 0.0,
        0.0, 0.0, 1.0,
        0.0, 1.0, 1.0,
        0.0, 1.0, 0.0,

        0.0, 1.0, 0.0,
        0.0, 1.0, 1.0,
        1.0, 1.0, 1.0,
        1.0, 1.0, 0.0,

        1.0, 1.0, 1.0,
        1.0, 0.0, 1.0,
        1.0, 0.0, 0.0,
        1.0, 1.0, 0.0
    };

    m_cube.insert(m_cube.end(), cube, cube + sizeof(cube) / sizeof(float));

    QColor borderColour = Qt::darkGray;
    float colourList [] = { (float)borderColour.redF(), (float)borderColour.greenF(), (float)borderColour.blueF() };
    for (int v = 0; v < m_cube.size()/3; v++)
    {
        // to change color change them every four iteration
        m_cubeColor.insert(m_cubeColor.end(), colourList, colourList + 3); // 3 coordinates per vertex
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
