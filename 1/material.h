#ifndef MATERIAL_H
#define MATERIAL_H

#include <QString>
#include <QVector3D>
#include <QImage>

/*
  Ka - амбиент
  Ks - спекьюлар
  Kd - диффьюз составляющие материала

  Ns - шины (вместе с Ks используется, отвечает за блик  : чем больше это число, тем меньше диаметр бликов на материале)
  map_Kd - текстура для Kd
  map_Ks, map_Ka
*/

class Material
{
public:

    Material();

    void setName(const QString &&_mtlName);
    const QString &MtlName() const;


    void setDiffuseColor(const QVector3D &&_diffuseColor);
    const QVector3D &getDiffuseColor() const;


    void setAmbienceColor(const QVector3D &&_ambienceColor);
    const QVector3D &AmbienceColor() const;

    void setSpecularColor(const QVector3D &&_specularColor);
    const QVector3D &SpecularColor() const;

    void setShinnes(const qreal &&shinnes);
    const float &getShinnes() const;

    void setDiffuseMap(const QString &&path);
    void setDiffuseMap(const QImage &image);
    const QImage &getDiffuseMap() const;

    bool isDiffuseMapSet() const;

    void setNormalMap(const QString &&path);
    void setNormalMap(const QImage &image);
    const QImage &getNormalMap() const;

    bool isNormalMapSet() const;

private:

    QString mtlName;

    //описание самого материала
    QVector3D diffuseColor,
                ambienceColor,
                specularColor;

    float shinnes;

    QImage diffuseMap,
            normalMap;

    bool isUsingDiffuseMap,
            isUsingNormalMap;
};

#endif // MATERIAL_H
