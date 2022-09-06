#include "block.h"

#include <QDir>

Block::Block(QImage *texture, const BlockType &blockType, Player *owner)
    : mainTexture(*texture),isAvailableToStep(false), blockType(blockType), currentBlockOwner(owner)
{
    InitBlock(texture);
}

Block::Block(QImage *texture, QImage *normalMap, const BlockType &blockType ,Player *owner)
    : mainTexture(*texture), isAvailableToStep(false), blockType(blockType), currentBlockOwner(owner)
{
    InitBlock(texture, normalMap);
}

const Block::BlockType &Block::GetType() const
{
    return blockType;
}

Player *Block::GetOwner()
{
    return currentBlockOwner;
}

void Block::ChangeAvailableToStepStatus()
{
    isAvailableToStep = !isAvailableToStep;
}

void Block::ChangeLevelOfDefense(const int &levelOfAttack)
{
    levelOfDefense = levelOfAttack;
}

bool Block::IsAvailableToStep(const int &levelOfAttack) const
{
    return (levelOfAttack >= levelOfDefense);
}

bool Block::IsOwned() const
{
    return (currentBlockOwner != nullptr);
}

const bool &Block::IsOccupied() const
{
    return isOccupied;
}

void Block::InitBlock(QImage *diffuseMap, QImage *normalMap)
{

    QVector<VertexData> vertexes;

    vertexes.append(VertexData(QVector3D(-WIDTH, HEIGHT, DEPTH), QVector2D(0.0, 1.0), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(-WIDTH, -HEIGHT, DEPTH), QVector2D(0.0, 0.0), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(WIDTH, HEIGHT, DEPTH), QVector2D(1.0, 1.0), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(WIDTH, -HEIGHT, DEPTH), QVector2D(1.0, 0.0), QVector3D(0.0, 0.0, 1.0)));

    vertexes.append(VertexData(QVector3D(WIDTH, HEIGHT, DEPTH), QVector2D(0.0, 1.0), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(WIDTH, -HEIGHT, DEPTH), QVector2D(0.0, 0.0), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(WIDTH, HEIGHT, -DEPTH), QVector2D(1.0, 1.0), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(WIDTH, -HEIGHT, -DEPTH), QVector2D(1.0, 0.0), QVector3D(1.0, 0.0, 0.0)));

    vertexes.append(VertexData(QVector3D(WIDTH, HEIGHT, DEPTH), QVector2D(0.0, 1.0), QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(WIDTH, HEIGHT, -DEPTH), QVector2D(0.0, 0.0), QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-WIDTH, HEIGHT, DEPTH), QVector2D(1.0, 1.0), QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-WIDTH, HEIGHT, -DEPTH), QVector2D(1.0, 0.0), QVector3D(0.0, 1.0, 0.0)));

    vertexes.append(VertexData(QVector3D(WIDTH, HEIGHT, -DEPTH), QVector2D(0.0, 1.0), QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(WIDTH, -HEIGHT, -DEPTH), QVector2D(0.0, 0.0), QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(-WIDTH, HEIGHT, -DEPTH), QVector2D(1.0, 1.0), QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(-WIDTH, -HEIGHT, -DEPTH), QVector2D(1.0, 0.0), QVector3D(0.0, 0.0, -1.0)));

    vertexes.append(VertexData(QVector3D(-WIDTH, HEIGHT, DEPTH), QVector2D(0.0, 1.0), QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-WIDTH, HEIGHT, -DEPTH), QVector2D(0.0, 0.0), QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-WIDTH, -HEIGHT, DEPTH), QVector2D(1.0, 1.0), QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-WIDTH, -HEIGHT, -DEPTH), QVector2D(1.0, 0.0), QVector3D(-1.0, 0.0, 0.0)));

    vertexes.append(VertexData(QVector3D(-WIDTH, -HEIGHT, DEPTH), QVector2D(0.0, 1.0), QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-WIDTH, -HEIGHT, -DEPTH), QVector2D(0.0, 0.0), QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(WIDTH, -HEIGHT, DEPTH), QVector2D(1.0, 1.0), QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(WIDTH, -HEIGHT, -DEPTH), QVector2D(1.0, 0.0), QVector3D(0.0, -1.0, 0.0)));

    QVector<GLuint> indexes;

    for (short i = 0; i < 24; i += 4) {

        indexes.append(i + 0);
        indexes.append(i + 1);
        indexes.append(i + 2);
        indexes.append(i + 2);
        indexes.append(i + 1);
        indexes.append(i + 3);
    }

    Material* mtl = new Material;


    if (diffuseMap)
        mtl->setDiffuseMap(*diffuseMap);
    else
        mtl->setDiffuseMap(":/pantone-very-peri-2022.jpg");


    if (normalMap)
        mtl->setNormalMap(*normalMap);
    else
        mtl->setNormalMap(":/ice_texture.jpg");

    mainTexture = *diffuseMap;

    mtl->setShinnes(96.0);
    mtl->setDiffuseColor(QVector3D(1.0, 1.0, 1.0));
    mtl->setSpecularColor(QVector3D(1.0, 1.0, 1.0));
    mtl->setAmbienceColor(QVector3D(1.0, 1.0, 1.0));

    CalculateTBN(vertexes);
    object = new Object3D(vertexes, indexes, mtl);

}

void Block::ChangeTexture(QImage *diffuseMap, QImage *normalMap)
{
    Material* mtl = new Material;


    if (diffuseMap)
        mtl->setDiffuseMap(*diffuseMap);
    else
        mtl->setDiffuseMap(":/pantone-very-peri-2022.jpg");


    if (normalMap)
        mtl->setNormalMap(*normalMap);
    else
        mtl->setNormalMap(":/ice_texture.jpg");

    mtl->setShinnes(96.0);
    mtl->setDiffuseColor(QVector3D(1.0, 1.0, 1.0));
    mtl->setSpecularColor(QVector3D(1.0, 1.0, 1.0));
    mtl->setAmbienceColor(QVector3D(1.0, 1.0, 1.0));

    object->ChangeMaterial(mtl);
}

void Block::ChangeOwner(Player *newOwner)
{
    currentBlockOwner = newOwner;
    mainTexture = currentBlockOwner->GetPlayerTexture();
}

void Block::CalculateTBN(QVector<VertexData> &data)
{
    for (qsizetype i = 0; i < data.size(); i += 3) {

        QVector3D &v1 = data[i].position;
        QVector3D &v2 = data[i+1].position;
        QVector3D &v3 = data[i+2].position;

        //обход против часовой стрелки
        QVector2D &uv1 = data[i].texCoord;
        QVector2D &uv2 = data[i+1].texCoord;
        QVector2D &uv3 = data[i+2].texCoord;

        /*
          Откуда берётся основная формула?
          Если любой  отр. против часовой стрели треугольник наложить на карту текстуры,
          то слева направо по карте текстуры будет тангент, а снизу вверх - битангент
          Причём тангент и битангент - единичные вектора
          А нормаль к этой поверхности - ось z
          Чтобы найти тангент и битангент, нужно решить следующие уравнения:
          deltaPos1 = deltaUV1.x * T + deltaUV1.y * B
          deltaPos2 = deltaUV2.x * T + deltaUV2.y * B
        */

        QVector3D deltaPos1 = v2 - v1,
                deltaPos2 = v3 - v1;

        QVector2D deltaUV1 = uv2 - uv1,
                deltaUV2 = uv3 - uv1;

        float r = 1.0f / (deltaUV1.x() * deltaUV2.y() - deltaUV1.y() * deltaUV2.x());
        QVector3D tangent = (deltaPos1 * deltaUV2.y() - deltaPos2 * deltaUV1.y()) * r;
        QVector3D bitangent = (deltaPos2 * deltaUV1.x() - deltaPos1 * deltaUV2.x()) * r;

        data[i].tangent = data[i + 1].tangent = data[i + 2].tangent = std::move(tangent);
        data[i].bitangent = data[i + 1].bitangent = data[i + 2].bitangent = std::move(bitangent);

    }
}

const QVector3D &Block::GetLocation() const
{
    return object->GetLocation();
}

QImage *Block::getMainTexture()
{
    return &mainTexture;
}

void Block::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions, bool usingTextures)
{
    object->draw(program, functions, usingTextures);
}

void Block::Rotate(const QQuaternion &r)
{
    object->Rotate(r);
}

void Block::Translate(const QVector3D &t)
{
    object->Translate(std::move(t));
}

void Block::Scale(const float &s)
{
    object->Scale(s);
}

void Block::SetGlobalTransform(const QMatrix4x4 &q)
{
    object->SetGlobalTransform(q);
}
