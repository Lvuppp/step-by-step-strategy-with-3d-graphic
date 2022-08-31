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

        rock = 0,
        gravel = 1,
        sand = 2,
        ground = 3,
        grass = 4
    };

    Block(QImage *texture, Player *owner = nullptr);
    Block(QImage *texture, QImage *diffuseMap, Player *owner = nullptr);
    //Block(BlockType &type, QImage *texture, Player *owner = nullptr);


    virtual ~Block() {
        delete object;

    }

    const QString &GetObj() const;
    const BlockType &GetType() const;

    void ChangeAvailableStatus(); // изменения состояние выделения
    const bool &IsAvailable() const;

    void InitBlock(QImage *diffuseMap = nullptr, QImage *normalMap = nullptr);
    void ChangeTexture(QImage *diffuseMap = nullptr, QImage *normalMap = nullptr);

    const QVector3D& GetLocation() const;
    QImage *getMainTexture();

    void calculateTBN(QVector<VertexData> &data); //для карты нормалей

    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions, bool usingTextures = true);
    void Rotate(const QQuaternion &r);
    void Translate(const QVector3D &t);
    void Scale(const float &s);
    void SetGlobalTransform(const QMatrix4x4 &q);

private:

    Object3D * object;
    MaterialLibrary materialLibrary;
    QImage mainTexture;

    const float WIDTH = 3.0f / 2.0f;
    const float HEIGHT = 3.0f / 2.0f;
    const float DEPTH = 3.0f / 2.0f;


    BlockType blockType;
    bool isAvailable;
    Player *currentBlockOwner = nullptr;

    int moneyIncome;

};

#endif // BLOCK_H
