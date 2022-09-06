#include "widget.h"

#include <QtMath>
#include <QOpenGLFunctions>

Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    camera = new Camera;

    camera->Translate(QVector3D(0.0f, 0.0f, -5.0f));


    /*

      Можете повернуть камеру методом
      Rotate(QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, angle))
      где Axis - ось, вокруг которой осуществляется поворот

    */

    foreach(auto line, mapMatrix){
        line.resize(square + 2);
    }

    players.append(new Player());
    currentPlayer = players[0];


    square = 8;
}

Widget::~Widget()
{
    delete camera;
    delete skybox;

    delete selectedUnit;
    delete currentPlayer;

    for (qsizetype i = 0; i < groups.size(); ++i)
        delete groups[i];
}

void Widget::FinishTurn()
{
    if(currentPlayer != nullptr)
        currentPlayer->RestoreUnitsStamina();
}


void Widget::initializeGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE); //не рисовать задние грани

    initShaders();

    QImage FloorTexture(":/pantone-very-peri-2022.jpg"),
            FloorNormalMap(":/ice_texture.jpg"),

            PlTexture(":/cl_ppl.jpg");

    groups.append(new Group);

    float x = -0.5f, y = -3.5f, z = -8.0;

    for (int rowIndex = 0; rowIndex < square + 2; ++rowIndex) {
        mapMatrix.push_back(QVector<qsizetype>(square + 2));

        if(rowIndex == 0 || rowIndex == square + 1)
            continue;

        for(int lineIndex = 0; lineIndex < mapMatrix[0].size(); ++lineIndex){
            if(lineIndex != 0 && lineIndex != square + 1)
                mapMatrix[rowIndex][lineIndex] = 1;
        }
    }

    for (int i = 0; i < mapMatrix.size(); ++i) {
        qDebug() << mapMatrix[i];
    }


    for (short X = 0; X < square * square; ++X) {

        if (X > 0 && !(X % square)) {
            z += 5.5f;
            x = -0.5;
        }

        if (/*X == 0 || X == 1 ||*/ X == square * square - 1 || X == square * square - 2){ //крайние квадратики
            //так можно (как вариант) пометить место старта игроков
            floor.append(new Block(&PlTexture, Block::rock, players[0]));
            selectedBlocks.append(floor.last());

        }
        else{
            floor.append(new Block(&FloorTexture, &FloorNormalMap,Block::rock));
            selectedBlocks.append(floor.last());
        }
        floor.last()->Translate(QVector3D(x, y, z));

        groups.first()->addObject(floor.last());

        x += 5.5f;
    }


    WorldObjects.append(groups.first());

    QVector3D floor_pos(-square, -3.5f, -square);
    groups.first()->Translate(QVector3D(floor_pos));

    groups.append(new Group);

    CreateUnit(square * square - 1);

    skybox = new SkyBox(100.0f, QImage(":/skybox0.png"));

}

void Widget::resizeGL(int w, int h)
{
    float aspect = w / (float)h;

    ProjectionMatrix.setToIdentity();
    ProjectionMatrix.perspective(45, aspect, /*0.1f*/ 0.01f, /*10.0f*/ 1000.0f); //3 параметр: передняя плоскость отсечения,
    //4 - задняя
}

void Widget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    SkyBoxShaderProgram.bind();
    SkyBoxShaderProgram.setUniformValue("u_projectionMatrix", ProjectionMatrix);

    camera->draw(&SkyBoxShaderProgram);

    skybox->draw(&SkyBoxShaderProgram, context()->functions());

    SkyBoxShaderProgram.release();

    UnitShaderProgram.bind();

    UnitShaderProgram.setUniformValue("u_projectionMatrix", ProjectionMatrix);
    UnitShaderProgram.setUniformValue("u_lightPosition", QVector4D(0.0, 0.0, 0.0, 1.0));
    UnitShaderProgram.setUniformValue("u_lightPower", 1.0f);

    camera->draw(&UnitShaderProgram);

    for (qsizetype i = 0; i < WorldObjects.size(); ++i)
        WorldObjects[i]->draw(&UnitShaderProgram, context()->functions());

    UnitShaderProgram.release();
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton){
        MousePosition = QVector2D(event->position()); //координаты указателя относительно левого верхнего угла данного окна
    }
    else if (event->button() == Qt::RightButton) {
        QVector<WorldEngineBase *> selectedUnitsOfPlayer(currentPlayer->GetUnits());
        int blockIndex = SelectObject(event->position().x(), event->position().y(), selectedBlocks) - 1;
        int unitIndex = SelectObject(event->position().x(), event->position().y(), selectedUnitsOfPlayer) - 1;

        if(isUnitBoughtToCreate == true
                && currentPlayer != nullptr
                && floor[blockIndex]->GetOwner() == currentPlayer
                && unitIndex == -1
                && floor[blockIndex]->IsOccupied() == false)
        {
            CreateUnit(blockIndex);
        }
        else if(isBuildingBoughtToCreate == true
                && currentPlayer != nullptr
                && floor[blockIndex]->GetOwner() == currentPlayer
                && floor[blockIndex]->IsOccupied() == false)
        {
            CreateBuilding(blockIndex);
        }
        else{
            TakeStep(unitIndex, blockIndex);
        }
    }

    update();

    event->accept();
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{

    QVector2D diff = QVector2D(event->position()) - MousePosition;

    MousePosition = QVector2D(event->position()); //localPos()

    float angX = diff.y() / 2.0f, angY = diff.x() / 2.0f;

    camera->RotateX(QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, angX));
    camera->RotateY(QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, angY));

    update();
}

void Widget::wheelEvent(QWheelEvent *event)
{
    if (event->angleDelta().y() > 0)
        camera->Translate(QVector3D(0.0f, 0.0f, 0.25f));

    else if (event->angleDelta().y() < 0)
        camera->Translate(QVector3D(0.0f, 0.0f, -0.25f));

    update();
}

void Widget::keyPress(QKeyEvent *event)
{
    switch(event->key()) {

    case Qt::Key_A:
        camera->Translate(QVector3D(0.5f, 0.0f, 0.0f));
        break;

    case Qt::Key_D:
        camera->Translate(QVector3D(-0.5f, 0.0f, 0.0f));
        break;

    case Qt::Key_W:
        camera->Translate(QVector3D(0.0f, 0.0f, 0.5f));
        break;

    case Qt::Key_S:
        camera->Translate(QVector3D(0.0f, 0.0f, -0.5f));
        break;

    case Qt::Key_F1:
        isUnitBoughtToCreate = !isUnitBoughtToCreate;
        break;

    case Qt::Key_F2:
        isBuildingBoughtToCreate = !isBuildingBoughtToCreate;
        break;
    }

    update();
}

void Widget::initShaders()
{
    if (!UnitShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vcharacter.vsh"))
        exit(-230);

    if (!UnitShaderProgram.addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, ":/fcharacter.fsh"))
        exit(-229);

    if (!UnitShaderProgram.link())
        exit(-228);



    if (!SkyBoxShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vskybox.vsh"))
        exit(-2330);

    if (!SkyBoxShaderProgram.addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, ":/fskybox.fsh"))
        exit(-2229);

    if (!SkyBoxShaderProgram.link())
        exit(-2228);



    if (!SelectShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vselect.vsh"))
        exit(-2013);

    if (!SelectShaderProgram.addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, ":/fselect.fsh"))
        exit(-2019);

    if (!SelectShaderProgram.link())
        exit(-2018);
}

void Widget::TakeStep(const int &indexOfUnit,const int &indexOfBlock) ///надо перенести в логику персонажа
{

    if (indexOfUnit == -1 && selectedUnit == nullptr){
        return;
    }

    if(indexOfUnit != -1 && units[indexOfUnit]->GetStamina() > 0){

        QVector<qsizetype> avalibaleBlocks  = units[indexOfUnit]->AvailableSteps(square, mapMatrix);
        qDebug() << avalibaleBlocks;

        if(selectedUnit == nullptr){
            selectedUnit = units[indexOfUnit];
            ChangeBlocksTexture(avalibaleBlocks, new QImage(":/_floor.jpg"));

        }
        else if(selectedUnit == units[indexOfUnit]){
            ChangeBlocksTexture(avalibaleBlocks);
            selectedUnit = nullptr;
        }
        else{
            QVector<qsizetype> oldSelectedUnitAvalibaleBlocks =
                    selectedUnit->AvailableSteps(square,mapMatrix);

            ChangeBlocksTexture(oldSelectedUnitAvalibaleBlocks);

            selectedUnit = units[indexOfUnit];
            ChangeBlocksTexture(avalibaleBlocks, new QImage(":/_floor.jpg"));

        }
    }
    else if(selectedUnit != nullptr
            && indexOfBlock != -1 && floor[indexOfBlock]->IsAvailableToStep(selectedUnit->GetLevelOfAttack())) {

        selectedUnit->Translate(floor[indexOfBlock]->GetLocation() -
                                floor[selectedUnit->GetBlockPosition()]->GetLocation());

        QVector<qsizetype> avalibaleBlocksForOldSelectedUnit =
                selectedUnit->AvailableSteps(square,mapMatrix);

        ChangeBlocksTexture(avalibaleBlocksForOldSelectedUnit);

        selectedUnit->SetBlockPosition(indexOfBlock);
        selectedUnit->DicreaseStamina();

        floor[indexOfBlock]->ChangeTexture(&players[0]->GetPlayerTexture());
        floor[indexOfBlock]->ChangeOwner(players[0]);
        floor[indexOfBlock]->ChangeLevelOfDefense(selectedUnit->GetLevelOfAttack());

        selectedUnit = nullptr;

    }

}

void Widget::CreateUnit(const int &blockPosition)
{

    QVector3D floor_pos(-square, -3.5f, -square);
    units.append(new Unit(Unit::knight, blockPosition));
    units.last()->loadObjectFromFile(units.last()->GetObj());

    QVector3D pos = floor[blockPosition]->GetLocation() + floor_pos;
    units.last()->Translate(QVector3D(pos.x(), -6.0f, pos.z()));
    units.last()->Scale(15.0f);

    groups.at(1)->addObject(units.last());
    WorldObjects.append(groups.at(1));

    currentPlayer->AddUnit(units.last());

    isUnitBoughtToCreate = false;

}

void Widget::CreateBuilding(const int &blockPosition)
{

    QVector3D floor_pos(-square, -3.5f, -square);
    buildings.append(new Building(Building::type0, blockPosition));
    buildings.last()->loadObjectFromFile(buildings.last()->GetObj());

    QVector3D pos = floor[blockPosition]->GetLocation() + floor_pos;
    buildings.last()->Translate(QVector3D(pos.x(), -6.0f, pos.z()));
    buildings.last()->Scale(2.5f);

    groups.last()->addObject(buildings.last());
    WorldObjects.append(groups.last());

    currentPlayer->AddBuilding(buildings.last());
    isBuildingBoughtToCreate = false;

}
void Widget::ChangeBlocksTexture(QVector<qsizetype> blocks, QImage *texture)
{
    for (qsizetype j = 0; j < blocks.size(); ++j) {

        qsizetype blockIndex = blocks[j];

        if(blockIndex >= square * square || blockIndex <= 0)
            continue;

        if(texture == nullptr){
            floor[blockIndex]->ChangeTexture(floor[blockIndex]->getMainTexture());
        }
        else{
            floor[blockIndex]->ChangeTexture(texture);
        }

        floor[blockIndex]->ChangeAvailableToStepStatus();
    }

    update();
}

int Widget::SelectObject(int x, int y, QVector<WorldEngineBase *> &objs)
{

    glViewport(0, 0, width(), height());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST); //для корректной работы оси z: дальние объекты не должны перекрывать ближние

    SelectShaderProgram.bind();
    SelectShaderProgram.setUniformValue("u_projectionMatrix", ProjectionMatrix);;
    camera->draw(&SelectShaderProgram);

    for (qsizetype i = 0; i < objs.size(); ++i) {

        SelectShaderProgram.setUniformValue("u_code", float(i + 1)); //i + 1 чтоб не совпадал с цветом фона (чёрный)
        objs[i]->draw(&SelectShaderProgram, context()->functions(), false);
    }

    SelectShaderProgram.release();

    GLint viewport[4]; //x, y, w, h
    glGetIntegerv(GL_VIEWPORT, viewport);

    unsigned char res[4]; //4 компоненты RGBA, каждый по байту, [0-255], поэтому такой тип данных

    //1, 1 - ширина и высота пикселя который нужно считать под указателем мыши
    glReadPixels(x, viewport[3] - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &res);

    glDisable(GL_DEPTH_TEST);

    return res[0]; //красная компонента
}
