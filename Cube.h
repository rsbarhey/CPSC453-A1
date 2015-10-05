#ifndef SHAPES_H
#define SHAPES_H

#include <QObject>
#include <QColor>

using namespace std;

class Cube : public QObject
{
    Q_OBJECT
public:
    explicit Cube(QObject *parent = 0);

    // returning a const reference (const because we don't want it changing outside this class, ref because we don't want a copy of the class members
    const vector<float>& CubeVertices();
    const vector<float>& CubeColor();
    const vector<float>& CubeNormals();

    // Changes the cube color depending on which
    void ChangeCubeColor(int id);
    // Changes the faces color depending on given id (refer to README for extensive explaination of the algorithm)
    void SetMutlipleColors(int id);

private:
    // This generate 42 color, all which are unique (refer to README for extensive explaination of the algorithm)
    void generateUniqueRandomColor();
    // This constructs a list of 0 - 255 (color channel)
    void constructColorList(QList<int>& list);

    QList<QColor> colorList;

    vector<float> m_cube;
    vector<float> m_cubeColor;
    vector<float> m_cubeNormals;
};

#endif // SHAPES_H
