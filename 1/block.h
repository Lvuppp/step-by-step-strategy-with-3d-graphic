#ifndef BLOCK_H
#define BLOCK_H

#include "materiallibrary.h"
#include "object3d.h"

#include <QVector>

#include "worldenginebase.h"
#include "player.h"

#include <QFile>
#include <QFileInfo>

class Block : public WorldEngineBase
{
public:

    // класс описывает блоки по которым передвигаются персонажи

    enum BlockType {
        rock,
        gravel,
        sand,
        ground,
        grass
    };

    Block(QImage *texture, const int &type, Player *owner = nullptr);
    Block(QImage *texture, QImage *diffuseMap, const int &type, Player *owner = nullptr);


    virtual ~Block() {
        delete object;
    }

    const QString &GetObj() const;
    const BlockType &GetType() const;
    Player *GetOwner();
    int GetIncome();

    void ChangeAvailableToStepStatus();
    void ChangeLevelOfDefense(const int &levelOfAttack);

    bool IsAvailableToStep(const int& levelOfAttack) const;
    bool IsOwned() const;
    const bool &IsOccupied() const;

    void InitBlock(QImage *diffuseMap = nullptr, QImage *normalMap = nullptr);
    void ChangeTexture(QImage *diffuseMap = nullptr, QImage *normalMap = nullptr);

    void ChangeOwner(Player *newOwner);

    const QVector3D& GetLocation() const;
    QImage *getMainTexture();

    void CalculateTBN(QVector<VertexData> &data); //для карты нормалей

    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions, bool usingTextures = true);
    void Rotate(const QQuaternion &r);
    void Translate(const QVector3D &t);
    void Scale(const float &s);
    void SetGlobalTransform(const QMatrix4x4 &q);

private:

    Object3D * object;
    MaterialLibrary materialLibrary;
    QImage *mainTexture;

    const float WIDTH = 3.0f / 2.0f;
    const float HEIGHT = 3.0f / 2.0f;
    const float DEPTH = 3.0f / 2.0f;

    bool isAvailableToStep = false;
    bool isOccupied;
    BlockType blockType = sand;
    int levelOfDefense = 0;
    int moneyIncome;

    Player *currentBlockOwner = nullptr;

};

#endif // BLOCK_H
