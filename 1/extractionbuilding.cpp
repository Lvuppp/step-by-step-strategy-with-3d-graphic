#include "extractionbuilding.h"

ExtractionBuilding::ExtractionBuilding(const BuildingType &type, QVector3D pos, const int &position)
    : Building(type, position)
{
    if (type == common){
        ObjPath = ":/burger/burger_merged.obj";
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

    income = type * 7;
    cost = type * 12;
}
