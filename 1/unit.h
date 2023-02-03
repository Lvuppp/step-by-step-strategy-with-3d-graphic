#ifndef UNIT_H
#define UNIT_H

#include "materiallibrary.h"
#include "object3d.h"

#include <QVector>

#include "worldenginebase.h"

#include <QFile>
#include <QFileInfo>

class Unit : public WorldEngineBase
{
public:

    /*
        Думаю, это что-то вроде типа персонажей.
        В дальнейшем, в зависимости от него будет применяться
        та или иная модель и разное количество ходов

        Разумеется, нужно переназвать
    */

    enum UnitType {
        knight,
        archer,
        cleric,
        mage,
        golem
    };

    Unit();
    Unit(const UnitType &type,QVector3D pos, const int& position);

    virtual ~Unit() {

        for (int i = 0; i < objects.size(); ++i) {
            delete objects[i];
        }

    }

    void RestoreStamina();
    QVector<qsizetype> AvailableSteps(const qsizetype &floorSquare,const QVector<QVector<qsizetype>> mapMatrix);
    void loadObjectFromFile(const QString &path);
    void addObject(Object3D *object);
    void CalculateTBN(QVector<VertexData> &data); //для карты нормалей
    void SetBlockPosition(const int& blockNumber);
    void DicreaseStamina();

    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions, bool usingTextures = true);
    void Rotate(const QQuaternion &r);
    void Translate(const QVector3D &t);
    void Scale(const float &s);
    void SetGlobalTransform(const QMatrix4x4 &q);

    const int &GetLevelOfAttack() const;
    const int &GetLevelOfDefense() const;
    const QVector3D& GetLocation() const;
    const int &GetBlockPosition() const;
    const QString &GetObj() const;
    const UnitType &GetType() const;
    const int &GetCost() const;
    const int &GetStamina() const;
    Object3D *GetObject(quint32 index);

private:

    QVector<Object3D *> objects;
    MaterialLibrary materialLibrary;
    QString ObjPath;

    int positionOnBlock;

    UnitType unitType;
    int stamina = 1;
    int cost;

    int levelOfAttack;
    int levelOfDefense;
};
#endif // UNIT_H
