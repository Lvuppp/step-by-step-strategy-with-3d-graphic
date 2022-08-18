#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

#include "worldenginebase.h"
#include "material.h"

struct VertexData {

    VertexData() {}

    VertexData(QVector3D p, QVector2D t, QVector3D n) : position (p),
        texCoord(t), normal(n) {}

    QVector3D position;
    QVector2D texCoord;
    QVector3D normal;

    QVector3D tangent,
              bitangent;
};

class Object3D : public WorldEngineBase
{
public:
    Object3D();

    Object3D(const QVector<VertexData> &data,
                   const QVector<GLuint> &indexes, const QImage &texture);

    Object3D(const QVector<VertexData> &data, const QVector<GLuint> &indexes, Material *material);

    virtual ~Object3D();

    void Init(const QVector<VertexData> &data,
              const QVector<GLuint> &indexes, const QImage &texture);


    void Init(const QVector<VertexData> &data,
              const QVector<GLuint> &indexes, Material *material);

    const QVector3D &GetLocation() const;

    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions, bool usingTextures = true);

    void Rotate(const QQuaternion &r);

    void Translate(const QVector3D &t);

    void Scale(const float &s);

    void SetGlobalTransform(const QMatrix4x4 &q);

private:
    QOpenGLBuffer VertexBuffer,
                  IndexBuffer; //чтоб было понятно, как пройтись треугольниками по объявленному выше массиву с данными

    QOpenGLTexture *diffuseMap,
                    *normalMap;

    //составляющие модельно-видовой матрицы:
    QQuaternion rotate;
    QVector3D translate;
    float scale;
    QMatrix4x4 globalTransforms;

    Material *material;
};

#endif // OBJECT3D_H
