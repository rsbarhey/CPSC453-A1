#ifndef SHAPES_H
#define SHAPES_H

#include <QObject>

using namespace std;

class Shapes : public QObject
{
    Q_OBJECT
public:
    explicit Shapes(QObject *parent = 0);

    vector<float> BorderShape();
    vector<float> SquareShape();
    vector<float> TShape();
    vector<float> LShape();
    vector<float> SShape();
    vector<float> Cube();
    vector<float> CubeColor();

signals:

public slots:

private:
    vector<float> m_cube;
    vector<float> m_cubeColor;
};

#endif // SHAPES_H
