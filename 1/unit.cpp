#include "unit.h"

#include <QDir>

Unit::Unit()
{
}

Unit::Unit(const Type &type,const int& position) :  positionOnBlock(position)
{
    if (type == type0){
        ObjPath = ":/aloe_vera_plant/aloevera.obj";
        levelOfAttack = levelOfDefense = 1;
    }
}

void Unit::RestoreStamina()
{
    stamina = 1;
}

const QString &Unit::GetObj() const
{
    return ObjPath;
}

const int &Unit::GetStamina() const
{
    return stamina;
}

QVector<qsizetype> Unit::AvailableSteps(const qsizetype &floorSquare,const QVector<QVector<qsizetype>> mapMatrix)
{
    QVector<qsizetype> availableSteps;

    qsizetype startRowIndex = this->positionOnBlock / floorSquare;
    qsizetype startLineIndex = this->positionOnBlock % floorSquare;
    // т.к у нас основная матрица "окружена" нулями, то мы прибавлем 1 чтобы получить истинное расположение персонажа

    for (qsizetype i = startRowIndex; i < startRowIndex + 3; i++) {
        for (qsizetype j = startLineIndex; j < startLineIndex + 3; j++) {

            if(mapMatrix[i][j] == 0 || (i == startRowIndex + 1 && j == startLineIndex + 1)){
                continue;
            }

            availableSteps.emplaceBack((i - 1) * floorSquare + j - 1);

        }
    }

    return availableSteps;
}

void Unit::loadObjectFromFile(const QString &path)
{
    QFile objFile(path);

    if (!objFile.exists())
        return;

    objFile.open(QIODevice::ReadOnly);

    QTextStream input(&objFile);

    QVector<QVector3D> coords;
    QVector<QVector2D> texCoords;
    QVector<QVector3D> normals;


    QVector<VertexData> vertexes;
    QVector<GLuint> indexes;

    Object3D *object = 0;

    QString MaterialName;

    while (!input.atEnd()) {

        QString str = input.readLine();

        QStringList list = str.split(" ");

        if (list[0] == "mtllib") {
            QFileInfo info(std::move(path));
            materialLibrary.loadMaterialsFromFile(QString("%1/%2").arg(info.absolutePath()).arg(list[1]));
        }

        else if (list[0] == "v")

            coords.append(QVector3D(list[1].toFloat(), list[2].toFloat(), list[3].toFloat()));

        else if (list[0] == "vt")

            texCoords.append(QVector2D(list[1].toFloat(), list[2].toFloat()));

        else if (list[0] == "vn")

            normals.append(QVector3D(list[1].toFloat(), list[2].toFloat(), list[3].toFloat()));

        else if (list[0] == "f")

            for (int i = 1; i <= 3; ++i) {

                QStringList vert = list[i].split("/"); //индексы в obj файле начинаются с 1

                vertexes.append(VertexData(coords[abs(vert[0].toLong()) - 1], texCoords[abs(vert[1].toLong()) - 1], normals[abs(vert[2].toLong()) - 1]));

                indexes.append(indexes.size());
            }
        else if (list[0] == "usemtl") {

            if (object)
                object->Init(vertexes, indexes, materialLibrary.getMaterial(MaterialName));

            MaterialName = list[1];

            addObject(object);

            object = new Object3D;

            vertexes.clear();
            indexes.clear();
        }
    }

    if (object)
        object->Init(vertexes, indexes, materialLibrary.getMaterial(MaterialName));

    addObject(object);

    objFile.close();
}

void Unit::addObject(Object3D *object)
{
    if (!object)
        return;

    if (objects.contains(object))
        return;

    objects.append(object);
}

Object3D *Unit::GetObject(quint32 index)
{
    if (index < objects.size())
        return objects[index];
    else
        return nullptr;
}

void Unit::CalculateTBN(QVector<VertexData> &data)
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

const QVector3D &Unit::GetLocation() const
{
    return objects.last()->GetLocation();
}

const int &Unit::GetBlockPosition() const
{
    return positionOnBlock;
}

void Unit::SetBlockPosition(const int& blockNumber)
{
    positionOnBlock = blockNumber;
}

void Unit::DicreaseStamina()
{
    --stamina;
}

void Unit::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions, bool usingTextures)
{
    for (qsizetype i = 0; i < objects.size(); ++i)
        objects[i]->draw(program, functions, usingTextures);
}

void Unit::Rotate(const QQuaternion &r)
{
    for (qsizetype i = 0; i < objects.size(); ++i)
        objects[i]->Rotate(r);
}

void Unit::Translate(const QVector3D &t)
{
    for (qsizetype i = 0; i < objects.size(); ++i)
        objects[i]->Translate(std::move(t));
}

void Unit::Scale(const float &s)
{
    for (qsizetype i = 0; i < objects.size(); ++i)
        objects[i]->Scale(s);
}

void Unit::SetGlobalTransform(const QMatrix4x4 &q)
{
    for (qsizetype i = 0; i < objects.size(); ++i)
        objects[i]->SetGlobalTransform(q);
}

const int &Unit::GetLevelOfAttack() const
{
    return levelOfAttack;
}


const int &Unit::GetLevelOfDefense() const
{
    return levelOfDefense;
}

