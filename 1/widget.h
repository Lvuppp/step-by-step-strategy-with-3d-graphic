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
#include "unit.h"
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
    void TakeStep(const int& indexOfBlock, const int& indexOfUnit);

    void ChangeBlockTexture(QVector<qsizetype> blocks, QImage *texture = nullptr);
    int SelectObject(int x, int y, QVector<WorldEngineBase *> &objs);

private:

    QVector3D screenCoordsToWorldCoords(const QVector2D &mousePos);

    QMatrix4x4 ProjectionMatrix,
               ProjectionLightMatrix/*,
               LightMatrix,
               ShadowLightMatrix*/;

    QOpenGLShaderProgram UnitShaderProgram,
                           SkyBoxShaderProgram,
                            SelectShaderProgram;

    QVector2D MousePosition;

    QVector<Unit*> Units;
    QVector<Block*>  floor;

    QVector<WorldEngineBase *> WorldObjects, selectObjects, selectedBlocks;
    QVector<Group *> groups;

    Camera* camera;
    SkyBox* skybox;

    // мап убрал т.к перенёс эту логику в блоки

    qsizetype square;

    /*

      карта нарисована в виде матрицы, площадь которой = square * square

      на моем ноуте при square = 8 запускается секунд 30 (окошко приложения
      не откроется, пока все объекты не будут отрисованы)

    */


    QVector<Player *> players;

    Unit *selectedUnit = nullptr; // выбранный персонаж, требуется для запоминания текущего выбранного пперсонажа
};
#endif // WIDGET_H
