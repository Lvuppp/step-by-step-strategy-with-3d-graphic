#ifndef GROUP_H
#define GROUP_H


#include "worldenginebase.h"

#include <QVector>
#include <QQuaternion>
#include <QVector3D>

#include <QMatrix4x4>


class Group : public WorldEngineBase
{
public:
    Group();
    virtual ~Group();

    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions, bool usingTextures = true);

    void Rotate(const QQuaternion &r);

    void Translate(const QVector3D &t);

    void Scale(const float &s);

    void SetGlobalTransform(const QMatrix4x4 &q);

    void addObject(WorldEngineBase *obj);

    void delObject(WorldEngineBase *obj);
    void delObject(const int &&index);
    WorldEngineBase *getObj(qsizetype index);

private:

    QQuaternion rotate;
    QVector3D translate;
    float scale;
    QMatrix4x4 globalTransforms;

    QVector<WorldEngineBase *> objects;

protected:

    QMatrix4x4 getLocalMatrix();
};

#endif // GROUP_H
