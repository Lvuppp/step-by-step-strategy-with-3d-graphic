#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

#include <QPushButton>
#include <QPalette>
#include <QPixmap>
#include <QFont>

#include "widget.h"

namespace Ui {
class Menu;
}

class Menu : public QWidget
{
    Q_OBJECT

public:
    explicit Menu(QWidget *parent = nullptr);
    ~Menu();

private slots:
    void on_openGLWidget_aboutToResize();

    void on_pushButton_clicked();

signals:
    void TurnFinished();
    void KeyPress(QKeyEvent *);

private:
    Ui::Menu *ui;

    Widget *w;


    void ChangeBackgroundColor(const int& r, const int& g, const int& b); //передать компоненты цвета
    void ChangeBackgroundPicture (const QString& path); //передать путь к картинке, предварительно добавленную в файл ресурсов

    void AddImage(QHBoxLayout* layout, const QString& ImagePath, const int& size = 30);
    //передать указатель на слой (пример в cpp, путь к картинке и ее размер (квадрат)
    void AddText(QHBoxLayout* layout, const QString& text, const QVector3D& textcolor, const int& size = 25);
    //qvector3d потому что было немного лень писать инты через запятую(


    QVector<QWidget *> items; //чтобы всё созданное через new в функциях не терялось

protected:
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // MENU_H
