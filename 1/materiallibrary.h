#ifndef MATERIALLIBRARY_H
#define MATERIALLIBRARY_H

#include <QVector>

#include "material.h"

#include <QFile>
#include <QTextStream>
#include <QFileInfo>

#include <QDebug>

class MaterialLibrary
{
public:

    MaterialLibrary();

    void addMaterial(Material* material);
    void loadMaterialsFromFile(const QString &path);

    Material* getMaterial(quint32 index);
    Material* getMaterial(const QString &mtlName);
    quint32 getCountMaterials();


private:
    QVector<Material *> materials;
};

#endif // MATERIALLIBRARY_H
