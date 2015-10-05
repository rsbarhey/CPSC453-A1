#include "Cube.h"

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
        m_cubeColor.insert(m_cubeColor.end(), colourList, colourList + 3); // 3 coordinates per vertex
        m_cubeNormals.insert(m_cubeNormals.end(), normalList, normalList + 3); // 3 coordinates per vertex
    }
    // generate it once!
    generateUniqueRandomColor();
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
    m_cubeColor.clear();
    QColor color = colorList[id];

    float colourList [] = { (float)color.redF(), (float)color.greenF(), (float)color.blueF() };

    for (int v = 0; v < m_cube.size()/3; v++)
    {
        m_cubeColor.insert(m_cubeColor.end(), colourList, colourList + 3); // 3 coordinates per vertex
    }
}

void Cube::SetMutlipleColors(int id)
{
    m_cubeColor.clear();
    QColor color;
    int colorIndex = id * 6;

    for (int v = 0; v < m_cube.size()/3; v++)
    {
        if(v % 4 == 0)
        {
            color = colorList[colorIndex];
            ++colorIndex;
        }

        float colourList [] = { (float)color.redF(), (float)color.greenF(), (float)color.blueF() };
        m_cubeColor.insert(m_cubeColor.end(), colourList, colourList + 3); // 3 coordinates per vertex
    }
}

void Cube::generateUniqueRandomColor()
{
    colorList.clear();
    QList<int> redChannel, greenChannel, blueChannel;

    constructColorList(redChannel);
    constructColorList(greenChannel);
    constructColorList(blueChannel);

    int redIndex, greenIndex, blueIndex;
    for(int i = 0; i < 42; i++)
    {
        redIndex = redChannel[rand() % redChannel.size()];
        redChannel.removeAt(redIndex);

        greenIndex = greenChannel[rand() % greenChannel.size()];
        greenChannel.removeAt(greenIndex);

        blueIndex = blueChannel[rand() % blueChannel.size()];
        blueChannel.removeAt(blueIndex);

        colorList.append(QColor(redIndex, greenIndex, blueIndex));
    }
}

void Cube::constructColorList(QList<int> &list)
{
    for(int i = 0; i< 256 ; i++)
    {
        list.append(i);
    }
}
