#include "skybox.h"

#include <QImage>

SkyBox::SkyBox(float width, const QImage &texture)
{
    float w_by2 = width / 2.0f;


    //Здесь натягивается текстура на фон

    QVector<VertexData> vertexes;
    //позади нас
    vertexes.append(VertexData(QVector3D(-w_by2, w_by2, w_by2), QVector2D(1.0f, 2.0f/3.0f), QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(-w_by2, -w_by2, w_by2), QVector2D(1.0f, 1.0f/3.0f), QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(w_by2, w_by2, w_by2), QVector2D(3.0f/4.0f, 2.0f/3.0f), QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(w_by2, -w_by2, w_by2), QVector2D(3.0f/4.0f, 1.0f/3.0f), QVector3D(0.0, 0.0, -1.0)));

    //справа от нас
    vertexes.append(VertexData(QVector3D(w_by2, w_by2, w_by2), QVector2D(3.0f/4.0f, 2.0f/3.0f), QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(w_by2, -w_by2, w_by2), QVector2D(3.0f/4.0f, 1.0f/3.0f), QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(w_by2, w_by2, -w_by2), QVector2D(2.0f/4.0f, 2.0f/3.0f), QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(w_by2, -w_by2, -w_by2), QVector2D(2.0f/4.0f, 1.0f/3.0f), QVector3D(-1.0, 0.0, 0.0)));

    //над нами
    vertexes.append(VertexData(QVector3D(w_by2, w_by2, w_by2), QVector2D(2.0f/4.0f, 1.0f), QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(w_by2, w_by2, -w_by2), QVector2D(2.0f/4.0f, 2.0f/3.0f), QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-w_by2, w_by2, w_by2), QVector2D(1.0f/4.0f, 1.0f), QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-w_by2, w_by2, -w_by2), QVector2D(1.0f/4.0f, 2.0f/3.0f), QVector3D(0.0, -1.0, 0.0)));

    //перед нами
    vertexes.append(VertexData(QVector3D(w_by2, w_by2, -w_by2), QVector2D(2.0f/4.0f, 2.0f/3.0f), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(w_by2, -w_by2, -w_by2), QVector2D(2.0f/4.0f, 1.0f/3.0f), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(-w_by2, w_by2, -w_by2), QVector2D(1.0f/4.0f, 2.0f/3.0f), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(-w_by2, -w_by2, -w_by2), QVector2D(1.0f/4.0f, 1.0f/3.0f), QVector3D(0.0, 0.0, 1.0)));

    //слева от нас
    vertexes.append(VertexData(QVector3D(-w_by2, w_by2, w_by2), QVector2D(0.0, 2.0f/3.0f), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-w_by2, w_by2, -w_by2), QVector2D(1.0f/4.0f, 2.0f/3.0f), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-w_by2, -w_by2, w_by2), QVector2D(0.0f, 1.0f/3.0f), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-w_by2, -w_by2, -w_by2), QVector2D(1.0f/4.0f, 1.0f/3.0f), QVector3D(1.0, 0.0, 0.0)));

    //под нами
    vertexes.append(VertexData(QVector3D(-w_by2, -w_by2, w_by2), QVector2D(1.0f/4.0f, 0.0f), QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-w_by2, -w_by2, -w_by2), QVector2D(1.0f/4.0f, 1.0f/3.0f), QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(w_by2, -w_by2, w_by2), QVector2D(2.0f/4.0f, 0.0f), QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(w_by2, -w_by2, -w_by2), QVector2D(2.0f/4.0f, 1.0f/3.0f), QVector3D(0.0, 1.0, 0.0)));

    QVector<GLuint> indexes;

    for (short i = 0; i < 24; i += 4) {

        indexes.append(i + 0);
        indexes.append(i + 2);
        indexes.append(i + 1);

        indexes.append(i + 2);
        indexes.append(i + 3);
        indexes.append(i + 1);
    }


    Material* mtl = new Material;
    mtl->setDiffuseMap(texture);
    mtl->setShinnes(96.0);
    mtl->setDiffuseColor(QVector3D(1.0, 1.0, 1.0));
    mtl->setSpecularColor(QVector3D(1.0, 1.0, 1.0));
    mtl->setAmbienceColor(QVector3D(1.0, 1.0, 1.0));

    cube = new Object3D(vertexes, indexes, mtl);

}

SkyBox::~SkyBox()
{
    delete cube;
}

void SkyBox::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions, bool usingTextures)
{
    cube->draw(program, functions, usingTextures);
}

void SkyBox::Rotate(const QQuaternion &r)
{
    //Q_UNUSED можно использовать только если класс унаследован от Q_OBJECT
    (void)r;
}

void SkyBox::Translate(const QVector3D &t)
{
    (void)t;
}

void SkyBox::Scale(const float &s)
{
    (void)s;
}

void SkyBox::SetGlobalTransform(const QMatrix4x4 &q)
{
    (void)q;
}
