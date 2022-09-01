#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

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

private:
    Ui::Menu *ui;

    Widget *w;


    void ChangeBackgroundColor(const int& r, const int& g, const int& b);

    void ChangeBackgroundPicture (const QString& path);


    void AddImage(QHBoxLayout* layout, const QString& ImagePath, const int& size = 30);

    void AddText(QHBoxLayout* layout, const QString& text, const QVector3D& textcolor, const int& size = 25);


    QVector<QWidget *> items; //чтобы всё созданное через new в функциях не терялось

};

#endif // MENU_H
