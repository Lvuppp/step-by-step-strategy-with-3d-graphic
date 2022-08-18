#include "group.h"

Group::Group() : scale(1.0)
{

}

Group::~Group()
{

}

void Group::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions, bool usingTextures)
{
    for (qsizetype i = 0; i < objects.size(); ++i)
        objects[i]->draw(program, functions, usingTextures);

}

void Group::Rotate(const QQuaternion &r)
{
    rotate = r * rotate;

    for (qsizetype i = 0; i < objects.size(); ++i)
        objects[i]->SetGlobalTransform(getLocalMatrix());

}

void Group::Translate(const QVector3D &t)
{
    translate += t;

    for (qsizetype i = 0; i < objects.size(); ++i)
        objects[i]->SetGlobalTransform(getLocalMatrix());
}

void Group::Scale(const float &s)
{
    scale *= s;

    for (qsizetype i = 0; i < objects.size(); ++i)
        objects[i]->SetGlobalTransform(getLocalMatrix());
}

void Group::SetGlobalTransform(const QMatrix4x4 &q)
{
    globalTransforms = q;

    for (qsizetype i = 0; i < objects.size(); ++i)
        objects[i]->SetGlobalTransform(getLocalMatrix());
}

void Group::addObject(WorldEngineBase *obj)
{
    objects.append(obj);

    objects.last()->SetGlobalTransform(getLocalMatrix());
}

void Group::delObject(WorldEngineBase *obj)
{
    objects.removeAll(obj); //удаляет все объекты. removeOne(obj) - удаляет первый попавшийся такой объект (обход с начала вектора)
}

void Group::delObject(const int &&index)
{
    objects.remove(index);
}

WorldEngineBase *Group::getObj(qsizetype index)
{
    return objects[index];
}

QMatrix4x4 Group::getLocalMatrix()
{
    QMatrix4x4 localMatrix;
    localMatrix.setToIdentity();
    localMatrix.translate(translate);
    localMatrix.rotate(rotate);
    localMatrix.scale(scale);
    localMatrix = globalTransforms * localMatrix;

    return localMatrix;
}
