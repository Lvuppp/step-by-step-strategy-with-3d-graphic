#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QOpenGLContext>
#include <QOpenGLFramebufferObject>

#include "group.h"
#include "camera.h"
#include "skybox.h"
#include "character.h"
#include "block.h"

#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>

#include <QMap>

#include <QFile>

class Widget : public QOpenGLWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:

    void initializeGL(); //вызывается один раз, при создании приложения
    void resizeGL(int w, int h); //вызывается при масштабировании окна
    void paintGL(); //можно вызвать принудительно методом update()

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);

    void initShaders();
    void initBlock(float width, float height, float depth, QImage *diffuseMap = 0, QImage *normalMap = 0);

    void ChangeBlockTexture(QVector<qsizetype> blocks, QImage *texture = nullptr);
    int SelectObject(int x, int y, QVector<WorldEngineBase *> &objs);

private:

    QVector3D screenCoordsToWorldCoords(const QVector2D &mousePos);

    QMatrix4x4 ProjectionMatrix,
               ProjectionLightMatrix/*,
               LightMatrix,
               ShadowLightMatrix*/;

    QOpenGLShaderProgram CharacterShaderProgram,
                           SkyBoxShaderProgram,
                            SelectShaderProgram;

    QVector2D MousePosition;

    QVector<Character*> characters;
    QVector<Block*>  floor;

    QVector<WorldEngineBase *> WorldObjects, selectObjects, selectedBlocks;
    QVector<Group *> groups;

    Camera* camera;
    SkyBox* skybox;

    QMap<qsizetype, qsizetype> character_floor;
    /*

      map, потому что нужно понимать, под какой моделькой какой куб находится.
      так как шейдерная программа на первом этапе отрабатывает только для
      персонажей, мы не можем понять, на каком кубе он находится.
      поэтому, задаём соответствие между индексом полученной модельки и
      номером куба, на которой она была отрисована. скорее всего, тут
      есть и другое решение. можно, как вариант, добавить какой-нибудь метод
      в класс Character, куда можно передать индекс, и впоследствии его достать

    */

    qsizetype square;

    /*

      карта нарисована в виде матрицы, площадь которой = square * square

      на моем ноуте при square = 8 запускается секунд 30 (окошко приложения
      не откроется, пока все объекты не будут отрисованы)

    */


    Character *selectedCharacter = nullptr;
};
#endif // WIDGET_H
