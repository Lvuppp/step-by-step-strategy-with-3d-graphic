#ifndef CHARACTER_H
#define CHARACTER_H

#include "materiallibrary.h"
#include "object3d.h"

#include <QVector>

#include "worldenginebase.h"

#include <QFile>
#include <QFileInfo>

class Character : public WorldEngineBase
{
public:


    /*
        Думаю, это что-то вроде типа персонажей.
        В дальнейшем, в зависимости от него будет применяться
        та или иная модель и разное количество ходов

        Разумеется, нужно переназвать
    */
    enum Type {

        type0 = 0,
        type1 = 1
    };

    Character();

    Character(const Type &type);

    const QString &GetObj() const;

    const Type &GetType() const;

    QVector<qsizetype> AvailableSteps(const qsizetype& CurFloorPos, const qsizetype &FloorSquare);

    void SetChoosen(); //методы, которые могут пригодиться при выборе хода
    void SetNotChoosen();

    const bool &IsChoosen() const;

    virtual ~Character() {

        for(auto object: objects)
            delete object;

    }

    void loadObjectFromFile(const QString &path);
    void addObject(Object3D *object );
    Object3D *getObject(quint32 index);

    void calculateTBN(QVector<VertexData> &data); //для карты нормалей

    const QVector3D& GetLocation() const;



    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions, bool usingTextures = true);

    void Rotate(const QQuaternion &r);

    void Translate(const QVector3D &t);

    void Scale(const float &s);

    void SetGlobalTransform(const QMatrix4x4 &q);

private:

    QVector<qsizetype> CalculateForType0(const qsizetype& i, const qsizetype &s);

    QVector<Object3D *> objects;

    MaterialLibrary materialLibrary;


    Type CharacterType;

    bool choosen;

    QString ObjPath;
};
#endif // CHARACTER_H
