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

    void ChangeCubeColor(int id);

    void SetMutlipleColors(int id);

private:
    void generateUniqueRandomColor();
    void constructColorList(QList<int>& list);

    QList<QColor> colorList;

    vector<float> m_cube;
    vector<float> m_cubeColor;
    vector<float> m_cubeNormals;
};

#endif // SHAPES_H
