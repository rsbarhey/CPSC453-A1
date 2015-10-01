#ifndef SHAPES_H
#define SHAPES_H

#include <QObject>

using namespace std;

class Cube : public QObject
{
    Q_OBJECT
public:
    explicit Cube(QObject *parent = 0);

    vector<float> CubeVertices();
    vector<float> CubeColor();
    vector<float> CubeNormals();

signals:

public slots:

private:
    vector<float> m_cube;
    vector<float> m_cubeColor;
    vector<float> m_cubeNormals;
};

#endif // SHAPES_H
