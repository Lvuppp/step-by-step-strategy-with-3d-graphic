#ifndef BUILDING_H
#define BUILDING_H

#include "materiallibrary.h"
#include "object3d.h"

#include <QVector>

#include "worldenginebase.h"

#include <QFile>
#include <QFileInfo>


class Building : public WorldEngineBase
{
public:
    enum Type {

        type0 = 0,
        type1 = 1
    };

    Building();
    Building(const Type &type, const int& position);

    virtual ~Building() {

        for (int i = 0; i < objects.size(); ++i) {
            delete objects[i];
        }

    }

    QVector<qsizetype> GetProtectedBlocks(const qsizetype& CurFloorPos, const qsizetype &FloorSquare);
    void loadObjectFromFile(const QString &path);
    void addObject(Object3D *object);
    void CalculateTBN(QVector<VertexData> &data); //для карты нормалей
    void SetBlockPosition(const int& blockNumber);

    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions, bool usingTextures = true);
    void Rotate(const QQuaternion &r);
    void Translate(const QVector3D &t);
    void Scale(const float &s);
    void SetGlobalTransform(const QMatrix4x4 &q);


    const QVector3D& GetLocation() const;
    const int& GetBlockPosition() const;
    const QString &GetObj() const;
    const Type &GetType() const;
    const int &GetCost() const;
    Object3D *GetObject(quint32 index);

private:

    QVector<Object3D *> objects;
    MaterialLibrary materialLibrary;
    QString ObjPath;

    int positionOnBlock;

    int cost;

    Type BuildingType;
};

#endif // BUILDING_H
