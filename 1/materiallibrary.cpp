#include "materiallibrary.h"

MaterialLibrary::MaterialLibrary()
{

}

inline void MaterialLibrary::addMaterial(Material *material)
{
    if (!material)
        return;

    if (materials.contains(material))
        return;

    materials.append(material);
}

void MaterialLibrary::loadMaterialsFromFile(const QString &path)
{
    QFile file(path);

    QTextStream inputStream(&file);

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "\nmtl file";
        return;
    }

    QFileInfo fileInfo(std::move(path));

    for (int i = 0; i < materials.size(); ++i)
        delete materials[i];
    materials.clear();

    Material *newMtl = 0;

    while (!inputStream.atEnd()) {

        QString str = inputStream.readLine();

        QStringList list = str.split(" ");

        if (list[0] == "newmtl") {

            addMaterial(newMtl);

            newMtl = new Material;
            newMtl->setName(std::move(list[1]));
        }

        else if (list[0] == "Ns")
            newMtl->setShinnes(list[1].toFloat());

        else if (list[0] == "Ka")
            newMtl->setAmbienceColor(QVector3D(list[1].toFloat(), list[2].toFloat(), list[3].toFloat()));

        else if (list[0] == "Kd")
            newMtl->setDiffuseColor(QVector3D(list[1].toFloat(), list[2].toFloat(), list[3].toFloat()));

        else if (list[0] == "Ks")
            newMtl->setSpecularColor(QVector3D(list[1].toFloat(), list[2].toFloat(), list[3].toFloat()));

        else if (list[0] == "map_Kd")
            newMtl->setDiffuseMap(std::move(QString("%1/%2").arg(fileInfo.absolutePath()).arg(std::move(list[1]))));

        else if (list[0] == "map_Normal")
            newMtl->setNormalMap(std::move(QString("%1/%2").arg(fileInfo.absolutePath()).arg(std::move(list[1]))));
    }

    addMaterial(newMtl);

    file.close();
}

Material *MaterialLibrary::getMaterial(quint32 index)
{
    if (index < materials.size())
        return materials[index];
    else
        return nullptr;
}

Material *MaterialLibrary::getMaterial(const QString &mtlName)
{
    for (int i = 0; i < materials.size(); ++i)
        if (materials[i]->MtlName() == mtlName)
            return materials[i];

    return nullptr;
}

quint32 MaterialLibrary::getCountMaterials()
{
    return materials.size();
}
