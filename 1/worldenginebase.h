#ifndef WORLDENGINEBASE_H
#define WORLDENGINEBASE_H

class QQuaternion;
class QVector3D;
class QMatrix4x4;
class QOpenGLShaderProgram;
class QOpenGLFunctions;


class WorldEngineBase {

public:

    virtual void Rotate(const QQuaternion &r) = 0;

    virtual void Translate(const QVector3D &t) = 0;

    virtual void Scale(const float &s) = 0;

    //метод, который выставляет локальную систему координат в пределах глобальной
    virtual void SetGlobalTransform(const QMatrix4x4 &q) = 0; //задаёт позицию и поворот в локальной СК в пределах родительской для неё глобальной СК

    virtual void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions, bool usingTextures = true) = 0;

};

#endif // WORLDENGINEBASE_H
