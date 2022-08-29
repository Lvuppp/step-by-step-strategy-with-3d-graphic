#ifndef BLOCK_H
#define BLOCK_H

#include "materiallibrary.h"
#include "object3d.h"

#include <QVector>

#include "worldenginebase.h"

#include <QFile>
#include <QFileInfo>

class Block : public WorldEngineBase
{
public:


    /*
        Думаю, это что-то вроде типа персонажей.
        В дальнейшем, в зависимости от него будет применяться
        та или иная модель и разное количество ходов

        Разумеется, нужно переназвать
    */
    enum BlockType {

        rock = 0,
        gravel = 1,
        sand = 2,
        ground = 3,
        grass = 4
    };

    Block(const QImage &texture);
    Block(const BlockType &type);


    virtual ~Block() {

        for(auto object: objects)
            delete object;

    }

    const QString &GetObj() const;
    const BlockType &GetType() const;


    void ChangeAvailableStatus(); // изменения состояние выделения
    const bool &IsAvailable() const;


    void loadObjectFromFile(const QString &path);
    void addObject(Object3D *object );


    Object3D *getObject(quint32 index);
    const QVector3D& GetLocation() const;
    void ChangeMainTexture();
    QImage *getMainTexture();


    void calculateTBN(QVector<VertexData> &data); //для карты нормалей
    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions, bool usingTextures = true);
    void Rotate(const QQuaternion &r);
    void Translate(const QVector3D &t);
    void Scale(const float &s);
    void SetGlobalTransform(const QMatrix4x4 &q);

private:

    QVector<Object3D *> objects;
    MaterialLibrary materialLibrary;
    QImage mainTexture;

    BlockType blockType;
    bool isAvailable;

};

#endif // BLOCK_H
