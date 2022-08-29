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

    square = 8;
}

Widget::~Widget()
{
    delete camera;

    for (qsizetype i = 0; i < characters.size(); ++i)
        delete characters[i];

    for (qsizetype i = 0; i < groups.size(); ++i)
        delete groups[i];
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

    /*

        ниже сам цикл отрисовки карты

        строки

        initFloor...,

        floor.last()->...,

        groups.last()..

        НЕ ТРОГАТЬ,
        если (когда) будете изменять карту

    */

    for (short X = 0; X < square * square; ++X) {

        if (X > 0 && !(X % square)) {
            z += 5.5f;
            x = -0.5;
        }

        if (X == 0 || X == 1 || X == square * square - 1 || X == square * square - 2) //крайние квадратики
                                                                    //так можно (как вариант) пометить место старта игроков
            initFloor(3.0f, 3.0f, 3.0f, &PlTexture);
        else
            initFloor(3.0f, 3.0f, 3.0f, &FloorTexture, &FloorNormalMap);

        floor.last()->Translate(QVector3D(x, y, z));

        groups.first()->addObject(floor.last());

        x += 5.5f;
    }


    WorldObjects.append(groups.first());

    QVector3D floor_pos(-square, -3.5f, -square);
    groups.first()->Translate(QVector3D(floor_pos));


    groups.append(new Group);

    characters.append(new Character(Character::type0));
    characters.last()->loadObjectFromFile(characters.last()->GetObj());


    QVector3D pos = floor.last()->GetLocation() + floor_pos;
    characters.last()->Translate(QVector3D(pos.x(), -6.0f, pos.z()));


    groups.last()->addObject(characters.last());

    WorldObjects.append(groups.last());

    characters.last()->Scale(15.0f);

    selectObjects.append(groups.last());

    character_floor.insert(0, square * square - 1);
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

    CharacterShaderProgram.bind();

    CharacterShaderProgram.setUniformValue("u_projectionMatrix", ProjectionMatrix);
    CharacterShaderProgram.setUniformValue("u_lightPosition", QVector4D(0.0, 0.0, 0.0, 1.0));
    CharacterShaderProgram.setUniformValue("u_lightPower", 1.0f);

    camera->draw(&CharacterShaderProgram);

    for (qsizetype i = 0; i < WorldObjects.size(); ++i)
           WorldObjects[i]->draw(&CharacterShaderProgram, context()->functions());

    CharacterShaderProgram.release();
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        MousePosition = QVector2D(event->position()); //координаты указателя относительно левого верхнего угла данного окна

    else if (event->button() == Qt::RightButton) {


        int indexOfSelectedObject = SelectObject(event->position().x(), event->position().y(), selectObjects);

        if (!indexOfSelectedObject--){

            int indexOfSelectedBlock = SelectObject(event->position().x(), event->position().y(), selectedBlocks);

            if(!iondex){

            }

            return;
        }

        qsizetype charactersBlock = character_floor[indexOfSelectedObject];
        QVector<qsizetype> avalibaleBlocks  = characters[indexOfSelectedObject]->AvailableSteps(charactersBlock, square);

        if(characters[indexOfSelectedObject]->IsSelect()){

            ChangeBlockTexture(avalibaleBlocks);
            characters[indexOfSelectedObject]->ChangeSelectionStatus();

            return ;
        }

        characters[indexOfSelectedObject]->ChangeSelectionStatus();
        ChangeBlockTexture(avalibaleBlocks, new QImage(":/_floor.jpg"));

    }

    event->accept();
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{

    QVector2D diff = QVector2D(event->position()) - MousePosition;

    MousePosition = QVector2D(event->position()); //localPos()

    float angX = diff.y() / 2.0f,
          angY = diff.x() / 2.0f;

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

void Widget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()) {

    case Qt::Key_Left:
        camera->Translate(QVector3D(0.5f, 0.0f, 0.0f));
        break;

    case Qt::Key_Right:
        camera->Translate(QVector3D(-0.5f, 0.0f, 0.0f));
        break;

    case Qt::Key_Up:
        camera->Translate(QVector3D(0.0f, 0.0f, 0.5f));
        break;

    case Qt::Key_Down:
        camera->Translate(QVector3D(0.0f, 0.0f, -0.5f));
    }


    update();
}

void Widget::initShaders()
{
   if (!CharacterShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vcharacter.vsh"))
       exit(-230);

   if (!CharacterShaderProgram.addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, ":/fcharacter.fsh"))
       exit(-229);

   if (!CharacterShaderProgram.link())
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

void Widget::initFloor(float width, float height, float depth, QImage *diffuseMap, QImage *normalMap)
{
    float w_by2 = width / 2.0f;

    float h_by2 = height / 2.0f;

    float d_by2 = depth / 2.0f;

    QVector<VertexData> vertexes;

    vertexes.append(VertexData(QVector3D(-w_by2, h_by2, d_by2), QVector2D(0.0, 1.0), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(-w_by2, -h_by2, d_by2), QVector2D(0.0, 0.0), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(w_by2, h_by2, d_by2), QVector2D(1.0, 1.0), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(w_by2, -h_by2, d_by2), QVector2D(1.0, 0.0), QVector3D(0.0, 0.0, 1.0)));

    vertexes.append(VertexData(QVector3D(w_by2, h_by2, d_by2), QVector2D(0.0, 1.0), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(w_by2, -h_by2, d_by2), QVector2D(0.0, 0.0), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(w_by2, h_by2, -d_by2), QVector2D(1.0, 1.0), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(w_by2, -h_by2, -d_by2), QVector2D(1.0, 0.0), QVector3D(1.0, 0.0, 0.0)));

    vertexes.append(VertexData(QVector3D(w_by2, h_by2, d_by2), QVector2D(0.0, 1.0), QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(w_by2, h_by2, -d_by2), QVector2D(0.0, 0.0), QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-w_by2, h_by2, d_by2), QVector2D(1.0, 1.0), QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-w_by2, h_by2, -d_by2), QVector2D(1.0, 0.0), QVector3D(0.0, 1.0, 0.0)));

    vertexes.append(VertexData(QVector3D(w_by2, h_by2, -d_by2), QVector2D(0.0, 1.0), QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(w_by2, -h_by2, -d_by2), QVector2D(0.0, 0.0), QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(-w_by2, h_by2, -d_by2), QVector2D(1.0, 1.0), QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(-w_by2, -h_by2, -d_by2), QVector2D(1.0, 0.0), QVector3D(0.0, 0.0, -1.0)));

    vertexes.append(VertexData(QVector3D(-w_by2, h_by2, d_by2), QVector2D(0.0, 1.0), QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-w_by2, h_by2, -d_by2), QVector2D(0.0, 0.0), QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-w_by2, -h_by2, d_by2), QVector2D(1.0, 1.0), QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-w_by2, -h_by2, -d_by2), QVector2D(1.0, 0.0), QVector3D(-1.0, 0.0, 0.0)));

    vertexes.append(VertexData(QVector3D(-w_by2, -h_by2, d_by2), QVector2D(0.0, 1.0), QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-w_by2, -h_by2, -d_by2), QVector2D(0.0, 0.0), QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(w_by2, -h_by2, d_by2), QVector2D(1.0, 1.0), QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(w_by2, -h_by2, -d_by2), QVector2D(1.0, 0.0), QVector3D(0.0, -1.0, 0.0)));

    QVector<GLuint> indexes;

    for (short i = 0; i < 24; i += 4) {

        indexes.append(i + 0);
        indexes.append(i + 1);
        indexes.append(i + 2);
        indexes.append(i + 2);
        indexes.append(i + 1);
        indexes.append(i + 3);
    }

    Material* mtl = new Material;


    if (diffuseMap)
        mtl->setDiffuseMap(*diffuseMap);
    else
        mtl->setDiffuseMap(":/pantone-very-peri-2022.jpg");


    if (normalMap)
        mtl->setNormalMap(*normalMap);
    else
        mtl->setNormalMap(":/ice_texture.jpg");

    mtl->setShinnes(96.0);
    mtl->setDiffuseColor(QVector3D(1.0, 1.0, 1.0));
    mtl->setSpecularColor(QVector3D(1.0, 1.0, 1.0));
    mtl->setAmbienceColor(QVector3D(1.0, 1.0, 1.0));

    Block *newObj = new Block(mtl->getDiffuseMap());

    newObj->calculateTBN(vertexes);
    newObj->addObject(new Object3D(vertexes, indexes, mtl));

    floor.append(newObj);
    selectedBlocks.append(floor.last());
}

void Widget::ChangeBlockTexture(QVector<qsizetype> blocks, QImage *texture)
{
    for (qsizetype j = 0; j < blocks.size(); ++j) {

        qsizetype blockIndex = blocks[j];

        if(blockIndex >= square * square || blockIndex <= 0)
            continue;

        QVector3D blockPosition = floor[blockIndex]->GetLocation();
        qDebug() << blockPosition.x();
        groups.first()->delObject(blockIndex);

        if(texture == nullptr)
            initFloor(3.0f, 3.0f, 3.0f, floor.at(blockIndex)->getMainTexture());
        else
            initFloor(3.0f, 3.0f, 3.0f, texture);

        floor.last()->Translate(blockPosition);
        groups.first()->insertObject(floor.last(),blockIndex);
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
