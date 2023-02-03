#include "widget.h"

#include <QtMath>
#include <QOpenGLFunctions>

Widget::Widget( QVector<Player *> setPlayers, QWidget *parent)
    : QOpenGLWidget(parent), square{8}, currentPlayer{setPlayers[0]}
{
    camera = new Camera;
    camera->Translate(QVector3D(0.0f, 0.0f, -10.0f));

    players = std::move(setPlayers);

    foreach(auto line, mapMatrix){
        line.resize(square + 2);
    }

    setMouseTracking(true);

}

Widget::~Widget()
{
    delete camera;
    delete skybox;

    for (qsizetype i = 0; i < groups.size(); ++i)
        delete groups[i];
}

void Widget::FinishTurn()
{
    NextTurn();
}


void Widget::initializeGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE); //не рисовать задние грани

    initShaders();

    QVector<QImage*> FloorTexture;
    FloorTexture.append(new QImage(":/gameTextures/texture1.jpg"));
    FloorTexture.append(new QImage(":/gameTextures/texture2.jpg"));
    FloorTexture.append(new QImage(":/gameTextures/texture3.jpg"));
    FloorTexture.append(new QImage(":/gameTextures/texture4.jpg"));
    FloorTexture.append(new QImage(":/gameTextures/texture5.jpg"));

    QImage FloorNormalMap(":/gameTextures/normalTexture.jpg");

    groups.append(new Group);

    QRandomGenerator generator;

    float x = -0.5f, y = -3.5f, z = -8.0;
    for (int rowIndex = 0; rowIndex < square + 2; ++rowIndex) {
        mapMatrix.push_back(QVector<qsizetype>(square + 2));


        for(int lineIndex = 0; lineIndex < mapMatrix[0].size(); ++lineIndex){
            if(rowIndex == 0 || rowIndex == square + 1){
                 mapMatrix[rowIndex][lineIndex] = -1;
                 continue;
            }

            if(lineIndex != 0 && lineIndex != square + 1)
                mapMatrix[rowIndex][lineIndex] = (generator.generate() & std::numeric_limits<qint64>::max()) % 5;
            else
                mapMatrix[rowIndex][lineIndex] = -1;
        }
    }

    for (short X = 0; X < square * square; ++X) {

        if (X > 0 && !(X % square)) {
            z += 5.5f;
            x = -0.5;
        }

        if(X == 0 || X == 1){
            mapMatrix[X % square + 1][X / square + 1] = 1;
            floor.append(new Block(players[1]->GetPlayerColor(mapMatrix[X % square + 1][X / square + 1]),
                    mapMatrix[X % square + 1][X / square + 1] , players[1]));
            selectedBlocks.append(floor.last());
        }
        else if(X == square * square - 1 || X == square * square - 2){ //крайние квадратики
            //так можно (как вариант) пометить место старта игроков
            mapMatrix[X % square + 1][X / square + 1] = 1;
            floor.append(new Block(players[0]->GetPlayerColor(mapMatrix[X % square + 1][X / square + 1]),
                    Block::rock, players[0]));
            selectedBlocks.append(floor.last());
        }
        //        else if(){

        //        }
        //        else if(){

        //        }
        else{
            floor.append(new Block(FloorTexture[mapMatrix[X % square + 1][X / square + 1]], &FloorNormalMap,
                    mapMatrix[X % square + 1][X / square + 1]));
            selectedBlocks.append(floor.last());
        }
        floor.last()->Translate(QVector3D(x, y, z));

        groups.first()->addObject(floor.last());

        x += 5.5f;
    }


    worldObjects.append(groups.first());

    QVector3D floor_pos(-square, -3.5f, -square);
    groups.first()->Translate(QVector3D(floor_pos));

    groups.append(new Group);

    CreateUnit(square * square - 1);
    currentPlayer = players[1];
    CreateUnit(0);
    currentPlayer = players[0];

    emit ChangeGold(currentPlayer->GetMoney(), currentPlayer->GetMoneyIncome());
    skybox = new SkyBox(1000.0f, QImage(":/gameTextures/skyboxTexture.png"));

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

    for (qsizetype i = 0; i < worldObjects.size(); ++i)
        worldObjects[i]->draw(&UnitShaderProgram, context()->functions());

    UnitShaderProgram.release();
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        QVector<WorldEngineBase *> selectedUnitsOfPlayer(currentPlayer->GetUnits());

        int blockIndex = SelectObject(event->position().x(), event->position().y(), selectedBlocks) - 1;
        int unitIndex = SelectObject(event->position().x(), event->position().y(), selectedUnitsOfPlayer) - 1;

        qDebug()<< isUnitBoughtToCreate;

        if(isUnitBoughtToCreate == true
                && currentPlayer != nullptr
                && floor[blockIndex]->GetOwner() == currentPlayer
                && unitIndex == -1
                && currentPlayer->GetMoney() - 20 > 0)

        {
            CreateUnit(blockIndex);
            currentPlayer->DecreaseMoney(20);
        }
        else if(isBuildingBoughtToCreate == true
                && currentPlayer != nullptr
                && floor[blockIndex]->GetOwner() == currentPlayer
                && currentPlayer->GetMoney() - 20 > 0)
        {
            CreateBuilding(blockIndex);
            currentPlayer->DecreaseMoney(20);
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
    event.
    camera->Translate(QVector3D(event-event->pos().x(), 0.0f, 0.25f));
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

    case Qt::Key_Enter:
        NextTurn();
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

    if(indexOfUnit != -1 && currentPlayer->GetPlayerUnits()[indexOfUnit]->GetStamina() > 0){

        QVector<qsizetype> avalibaleBlocks  = currentPlayer->GetPlayerUnits()[indexOfUnit]->AvailableSteps(square, mapMatrix);

        qDebug() << avalibaleBlocks;

        if(selectedUnit == nullptr){
            selectedUnit = currentPlayer->GetPlayerUnits()[indexOfUnit];
            ChangeBlocksTexture(avalibaleBlocks, new QImage(":/gameTextures/selectionTexture.jpg"));

        }
        else if(selectedUnit == currentPlayer->GetPlayerUnits()[indexOfUnit]){
            ChangeBlocksTexture(avalibaleBlocks);
            selectedUnit = nullptr;
        }
        else{
            QVector<qsizetype> oldSelectedUnitAvalibaleBlocks =
                    selectedUnit->AvailableSteps(square,mapMatrix);

            ChangeBlocksTexture(oldSelectedUnitAvalibaleBlocks);

            selectedUnit = currentPlayer->GetPlayerUnits()[indexOfUnit];
            ChangeBlocksTexture(avalibaleBlocks, new QImage(":/gameTextures/selectionTexture.jpg"));

        }
    }
    else if(selectedUnit != nullptr
            && indexOfBlock != -1 && floor[indexOfBlock]->IsAvailableToStep(selectedUnit->GetLevelOfAttack())) {

        qDebug() << indexOfBlock;
        bool tmp = false;

        for (auto el : selectedUnit->AvailableSteps(square,mapMatrix)) {
            if(el == indexOfBlock)
                tmp = true;
        }

        if(!tmp)
            return;

        selectedUnit->Translate(floor[indexOfBlock]->GetLocation() -
                                floor[selectedUnit->GetBlockPosition()]->GetLocation());

        QVector<qsizetype> avalibaleBlocksForOldSelectedUnit =
                selectedUnit->AvailableSteps(square,mapMatrix);

        ChangeBlocksTexture(avalibaleBlocksForOldSelectedUnit);

        selectedUnit->SetBlockPosition(indexOfBlock);
        selectedUnit->DicreaseStamina();

        floor[indexOfBlock]->ChangeTexture(currentPlayer->
                GetPlayerColor(mapMatrix[indexOfBlock % square + 1][indexOfBlock / square + 1]));

        if(floor[indexOfBlock]->GetOwner() != nullptr)
            floor[indexOfBlock]->GetOwner()->DecreaseMoneyIncome(floor[indexOfBlock]->GetType());

        floor[indexOfBlock]->ChangeOwner(currentPlayer);

        floor[indexOfBlock]->ChangeLevelOfDefense(selectedUnit->GetLevelOfAttack());
        currentPlayer->IncreaseMoneyIncome(floor[indexOfBlock]->GetIncome());

        emit ChangeGold(currentPlayer->GetMoney(), currentPlayer->GetMoneyIncome());

        selectedUnit = nullptr;
    }

}

void Widget::CreateUnit(const int &blockPosition)
{

    QVector3D position = floor[blockPosition]->GetLocation() + QVector3D(-square, -3.5f, -square);

    units.append(new Unit(Unit::knight, position, blockPosition));

    currentPlayer->AddUnit(units.last());

    groups.at(1)->addObject(units.last());
    worldObjects.append(groups.at(1));

    isUnitBoughtToCreate = false;

}

void Widget::NextTurn()
{
    if(currentPlayer != nullptr){
        currentPlayer->RestoreUnitsStamina();
        currentPlayer->IncreaseMoney();


        if(currentPlayerIndex == 1){
            currentPlayerIndex = 0;
        }
        else{
            currentPlayerIndex++;
        }
        currentPlayer = players[currentPlayerIndex];

        emit ChangeGold(currentPlayer->GetMoney(), currentPlayer->GetMoneyIncome());
    }
}

void Widget::CreateBuilding(const int &blockPosition)
{
    QVector3D position = floor[blockPosition]->GetLocation() + QVector3D(-square, -3.5f, -square);

    buildings.append(new DefenseBuilding(Building::common, position, blockPosition));

    groups.last()->addObject(buildings.last());
    worldObjects.append(groups.last());

    //currentPlayer->AddBuilding(buildings.last());

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
