#ifndef SKYBOX_H
#define SKYBOX_H

#include "worldenginebase.h"
#include "object3d.h"

class SkyBox : public WorldEngineBase
{
public:
    SkyBox(float width, const QImage &texture);
    virtual ~SkyBox();

    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions, bool usingTextures = true);

    void Rotate(const QQuaternion &r);

    void Translate(const QVector3D &t);

    void Scale(const float &s);

    void SetGlobalTransform(const QMatrix4x4 &q);


private:

    Object3D *cube;
};

#endif // SKYBOX_H
