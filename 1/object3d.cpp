#include "object3d.h"

Object3D::Object3D() :
    IndexBuffer(QOpenGLBuffer::IndexBuffer),  diffuseMap(0), scale(1.0)
{

}

Object3D::Object3D(const QVector<VertexData> &data, const QVector<GLuint> &indexes, const QImage &texture)
    : IndexBuffer(QOpenGLBuffer::IndexBuffer),  diffuseMap(0), scale(1.0)
{
    Init(data, indexes, texture);
}

Object3D::Object3D(const QVector<VertexData> &data, const QVector<GLuint> &indexes, Material *material)
    : IndexBuffer(QOpenGLBuffer::IndexBuffer), diffuseMap(0), scale(1.0)
{
    Init(data, indexes, material);
}

Object3D::~Object3D()
{
    if (VertexBuffer.isCreated())
        VertexBuffer.destroy();

    if (IndexBuffer.isCreated())
        IndexBuffer.destroy();
}

void Object3D::Init(const QVector<VertexData> &data, const QVector<GLuint> &indexes, const QImage &texture)
{
    if (VertexBuffer.isCreated())
        VertexBuffer.destroy();

    if (IndexBuffer.isCreated())
        IndexBuffer.destroy();

    if (diffuseMap != 0)
        if (diffuseMap->isCreated()) {
            delete diffuseMap;
            diffuseMap = 0;
        }

    VertexBuffer.create();
    VertexBuffer.bind();
    VertexBuffer.allocate(data.constData(), data.size() * sizeof(VertexData));
    VertexBuffer.release();

    IndexBuffer.create();
    IndexBuffer.bind();
    IndexBuffer.allocate(indexes.constData(), indexes.size() * sizeof(GLuint));
    IndexBuffer.release();

    diffuseMap = new QOpenGLTexture(texture.mirrored());

    diffuseMap->setMinificationFilter(QOpenGLTexture::Nearest);

    diffuseMap->setMinificationFilter(QOpenGLTexture::Linear);

    diffuseMap->setWrapMode(QOpenGLTexture::Repeat);
}

void Object3D::Init(const QVector<VertexData> &data, const QVector<GLuint> &indexes, Material *material)
{
    if (VertexBuffer.isCreated())
        VertexBuffer.destroy();

    if (IndexBuffer.isCreated())
        IndexBuffer.destroy();

    if (diffuseMap != 0)
        if (diffuseMap->isCreated()) {
            delete diffuseMap;
            diffuseMap = 0;
        }

    VertexBuffer.create();
    VertexBuffer.bind();
    VertexBuffer.allocate(data.constData(), data.size() * sizeof(VertexData));
    VertexBuffer.release();

    IndexBuffer.create();
    IndexBuffer.bind();
    IndexBuffer.allocate(indexes.constData(), indexes.size() * sizeof(GLuint));
    IndexBuffer.release();

    this->material = material;


    if (material->isDiffuseMapSet()) {

        diffuseMap = new QOpenGLTexture(material->getDiffuseMap().mirrored());

        diffuseMap->setMinificationFilter(QOpenGLTexture::Nearest);

        diffuseMap->setMinificationFilter(QOpenGLTexture::Linear);

        diffuseMap->setWrapMode(QOpenGLTexture::Repeat);

    }

    if (material->isNormalMapSet()) {

        normalMap = new QOpenGLTexture(material->getNormalMap().mirrored());

        normalMap->setMinificationFilter(QOpenGLTexture::Nearest);

        normalMap->setMinificationFilter(QOpenGLTexture::Linear);

        normalMap->setWrapMode(QOpenGLTexture::Repeat);

    }
}

const QVector3D &Object3D::GetLocation() const
{
    return translate;
}

void Object3D::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions, bool usingTextures)
{
    if (!VertexBuffer.isCreated() || !IndexBuffer.isCreated())
        return;

    QMatrix4x4 ModelMatrix;
    ModelMatrix.setToIdentity();

    //порядок следующий преобразований важен!! Реальные преобразования матрицы (внутри программы) делаются снизу вверх.
    ModelMatrix.translate(translate);
    ModelMatrix.rotate(rotate);
    ModelMatrix.scale(scale);
    ModelMatrix = globalTransforms * ModelMatrix;


    if (usingTextures) {

        if (material->isDiffuseMapSet()) {

            diffuseMap->bind(0);
            program->setUniformValue("u_diffuseMap", 0);
        }

        if (material->isNormalMapSet()) {

            normalMap->bind(1); //номер слота, на который биндим
            program->setUniformValue("u_normalMap", 1);
        }
    }

    program->setUniformValue("u_modelMatrix", ModelMatrix);

    program->setUniformValue("u_materialPropery.diffuseColor", material->getDiffuseColor());
    program->setUniformValue("u_materialPropery.ambienceColor", material->AmbienceColor());
    program->setUniformValue("u_materialPropery.specularColor", material->SpecularColor());
    program->setUniformValue("u_materialPropery.shinnes", material->getShinnes());

    program->setUniformValue("u_isUsingDiffuseMap", material->isDiffuseMapSet());
    program->setUniformValue("u_isUsingNormalMap", material->isNormalMapSet());

    VertexBuffer.bind();
    IndexBuffer.bind();

    int VertLoc = program->attributeLocation("a_position");
    program->enableAttributeArray(VertLoc);
    program->setAttributeBuffer(VertLoc, GL_FLOAT, 0, 3, sizeof(VertexData));

    int texLoc = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texLoc);
    program->setAttributeBuffer(texLoc, GL_FLOAT, sizeof(QVector3D), 2, sizeof(VertexData));


    int offset = sizeof(QVector3D) + sizeof(QVector2D);

    int normalLoc = program->attributeLocation("a_normal");
    program->enableAttributeArray(normalLoc);
    program->setAttributeBuffer(normalLoc, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);

    int tangentLoc = program->attributeLocation("a_tangent");
    program->enableAttributeArray(tangentLoc);
    program->setAttributeBuffer(tangentLoc, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);

    int BiTangentLoc = program->attributeLocation("a_bitangent");
    program->enableAttributeArray(BiTangentLoc);
    program->setAttributeBuffer(BiTangentLoc, GL_FLOAT, offset, 3, sizeof(VertexData));


    functions->glDrawElements(GL_TRIANGLES, IndexBuffer.size(), GL_UNSIGNED_INT, 0);

    VertexBuffer.release();
    IndexBuffer.release();

    if (usingTextures) {

        if (material->isDiffuseMapSet())
            diffuseMap->release();

        if (material->isNormalMapSet())
            normalMap->release();
    }
}

void Object3D::Rotate(const QQuaternion &r)
{
    rotate = r * rotate;
}

void Object3D::Translate(const QVector3D &t)
{
    translate += t;
}

void Object3D::Scale(const float &s)
{
    scale *= s;
}

void Object3D::SetGlobalTransform(const QMatrix4x4 &q)
{
    globalTransforms = q; //должен рассчитываться родителем и передаваться в готовом виде
}
