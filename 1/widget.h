#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QOpenGLContext>
#include <QOpenGLFramebufferObject>
#include <QRandomGenerator>
#include <QTimer>

#include "group.h"
#include "camera.h"
#include "skybox.h"
#include "unit.h"
#include "block.h"
#include "player.h"
#include "building.h"
#include "extractionbuilding.h"
#include "defensebuilding.h"

#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>

#include <QMap>

#include <QFile>

class Widget : public QOpenGLWidget
{
    Q_OBJECT

public:
    Widget(QVector<Player *> players, QWidget *parent = nullptr);
    ~Widget();
signals:
    void ChangeGold(int, int);

public slots:
    void FinishTurn(); // окончание хода
    void keyPress(QKeyEvent *event);
protected:

    void initializeGL(); //вызывается один раз, при создании приложения
    void resizeGL(int w, int h); //вызывается при масштабировании окна
    void paintGL(); //можно вызвать принудительно методом update()

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);


    void initShaders();
    void TakeStep(const int& indexOfBlock, const int& indexOfUnit);
    void CreateBuilding(const int &blockPosition);
    void CreateUnit(const int &blockPosition);
    void NextTurn();

    void ChangeBlocksTexture(QVector<qsizetype> blocks, QImage *texture = nullptr);
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

    QVector<Unit *> units;
    QVector<Block *>  floor;
    QVector<Building *> buildings;

    QVector<WorldEngineBase *> worldObjects, selectObjects, selectedBlocks;
    QVector<Group *> groups;

    Camera* camera;
    SkyBox* skybox;
    QTimer* gameTimer;

    QVector<QVector<qsizetype>> mapMatrix; // МОЖНО СОВМЕСТИТЬ С БЛОКОМ, А ТАКЖЕ ИЗМЕНИТЬ СПАВН КАРТЫ,
    // СДЕЛАТЬ СВЯЗНЫЙ ГРАФ КОТОРЫЙ БУДЕМ СОЗДАВАТЬ
    qsizetype square;

    /*

      карта нарисована в виде матрицы, площадь которой = square * square

      на моем ноуте при square = 8 запускается секунд 30 (окошко приложения
      не откроется, пока все объекты не будут отрисованы)

    */
    bool isUnitBoughtToCreate = false; // нажата ли кнопка для покупки юнита
    bool isBuildingBoughtToCreate = false; // нажата ли кнопка для покупки юнита


    QVector<Player *> players;
    qsizetype currentPlayerIndex = 0;

    Player *currentPlayer = nullptr;  // текущий игрок
    Unit *selectedUnit = nullptr; // выбранный юнит, требуется для запоминания текущего выбранного юнита
};
#endif // WIDGET_H
