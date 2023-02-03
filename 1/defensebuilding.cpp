#include "defensebuilding.h"

DefenseBuilding::DefenseBuilding(const BuildingType &type, QVector3D pos, const int &position)
    : Building(type, position)
{
    if (type == common){
        ObjPath = ":/buildingTextures/ArcherTower.obj";
        this->Scale(3.0f);
    }
    else if(type == rare){
        ObjPath = ":/burger/burger_merged.obj";
        this->Scale(0.25f);
    }
    else if(type == epic){
        ObjPath = ":/burger/burger_merged.obj";
        this->Scale(0.25f);
    }

    this->loadObjectFromFile(this->GetObj());
    this->Translate(QVector3D(pos.x(), -6.0f, pos.z()));

    levelOfDefense = type + 1;
    cost = type * 15;
}

QVector<qsizetype> DefenseBuilding::GetProtectedBlocks(const qsizetype &floorSquare, const QVector<QVector<qsizetype> > mapMatrix)
{
    QVector<qsizetype> defensedBlock;

    qsizetype startRowIndex = floorSquare / floorSquare;
    qsizetype startLineIndex = floorSquare % floorSquare;
    // т.к у нас основная матрица "окружена" нулями, то мы прибавлем 1 чтобы получить истинное расположение персонажа

    for (qsizetype i = startRowIndex; i < startRowIndex + 3; i++) {
        for (qsizetype j = startLineIndex; j < startLineIndex + 3; j++){

            defensedBlock.emplaceBack((i - 1) * floorSquare + j - 1);

        }
    }

    return defensedBlock;
}
