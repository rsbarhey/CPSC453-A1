#include "Cube.h"
#include <QColor>

Cube::Cube(QObject *parent) : QObject(parent)
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
    float normalList [] = { 0.0f, 0.0f, 1.0f }; // facing viewer
    for (int v = 0; v < m_cube.size()/3; v++)
    {
        // to change color change them every four iteration
        m_cubeColor.insert(m_cubeColor.end(), colourList, colourList + 3); // 3 coordinates per vertex
        m_cubeNormals.insert(m_cubeNormals.end(), normalList, normalList + 3); // 3 coordinates per vertex
    }
}

const vector<float>& Cube::CubeVertices()
{
    return m_cube;
}

const vector<float>& Cube::CubeColor()
{
    return m_cubeColor;
}

const vector<float>& Cube::CubeNormals()
{
    return m_cubeNormals;
}

void Cube::ChangeCubeColor(int id)
{
    Q_UNUSED(id);
    m_cubeColor.clear();
    QColor color = Qt::red;
    float colourList [] = { (float)color.redF(), (float)color.greenF(), (float)color.blueF() };

    for (int v = 0; v < m_cube.size()/3; v++)
    {
        // to change color change them every four iteration
        m_cubeColor.insert(m_cubeColor.end(), colourList, colourList + 3); // 3 coordinates per vertex
    }
}
