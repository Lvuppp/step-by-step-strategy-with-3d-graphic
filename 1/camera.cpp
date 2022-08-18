#include "camera.h"

Camera::Camera() : scale(1.0)
{
    globalTransforms.setToIdentity();
}

void Camera::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions, bool usingTextures)
{
    Q_UNUSED(usingTextures);

    if (functions)
        return;

    program->setUniformValue("u_viewMatrix", ViewMatrix);
}

void Camera::Rotate(const QQuaternion &r)
{
    rotate = r * rotate;

    updateViewMatrix();
}

void Camera::Translate(const QVector3D &t)
{
    translate += t;

    updateViewMatrix();
}

void Camera::Scale(const float &s)
{
    scale *= s;

    updateViewMatrix();
}

void Camera::SetGlobalTransform(const QMatrix4x4 &q)
{
    globalTransforms = q;

    updateViewMatrix();
}

void Camera::RotateX(const QQuaternion &r)
{
    rotateX = r * rotateX;
    rotate = rotateX * rotateY;

    updateViewMatrix();
}

void Camera::RotateY(const QQuaternion &r)
{
    rotateY = r * rotateY;
    rotate = rotateX * rotateY;

    updateViewMatrix();
}

const QMatrix4x4 &Camera::getViewMatrix() const
{
    return ViewMatrix;
}

void Camera::updateViewMatrix()
{
    ViewMatrix.setToIdentity();
    ViewMatrix.translate(translate);
    ViewMatrix.rotate(rotate);
    ViewMatrix.scale(scale);
    ViewMatrix = ViewMatrix * globalTransforms.inverted();
}
