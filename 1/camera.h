#ifndef CAMERA_H
#define CAMERA_H

#include "worldenginebase.h"

#include <QQuaternion>
#include <QVector3D>

#include <QMatrix4x4>

#include <QOpenGLShaderProgram>

class Camera : public WorldEngineBase
{
public:
    Camera();

    virtual ~Camera() {}

    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions = 0, bool usingTextures = true);

    void Rotate(const QQuaternion &r);

    void Translate(const QVector3D &t);

    void Scale(const float &s);

    void SetGlobalTransform(const QMatrix4x4 &q);

    void addObject(WorldEngineBase *obj);

    void RotateX(const QQuaternion &r);
    void RotateY(const QQuaternion &r);

    const QMatrix4x4 &getViewMatrix() const;

private:

    QQuaternion rotate,
                rotateX,
                rotateY;

    QVector3D translate;

    float scale;

    QMatrix4x4 globalTransforms,
                ViewMatrix;

protected:

    void updateViewMatrix();

};

#endif // CAMERA_H
