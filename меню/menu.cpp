#include "menu.h"
#include "ui_menu.h"

//#define N 4

Menu::Menu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Menu)
{
    //resize(width(), height());

    ui->setupUi(this);

    ChangeBackgroundColor(0, 0, 0);

    items.reserve(30);

    w = new Widget(ui->openGLWidget);

    w->resize(ui->openGLWidget->width(), ui->openGLWidget->height());

    AddImage(ui->CharacterImages, ":/pantone-very-peri-2022.jpg", 30);
    AddImage(ui->CharacterImages, ":/_floor.jpg");

    AddImage(ui->BuildingImages, ":/cl_ppl.jpg");

    AddText(ui->BuildingText, "1", QVector3D{255, 255, 255});

    w->show();
}

Menu::~Menu()
{
    delete ui;

    delete w;

    for (auto i : items)
        delete i;
}

void Menu::on_openGLWidget_aboutToResize()
{
    w->resize(ui->openGLWidget->width(), ui->openGLWidget->height());
}

void Menu::ChangeBackgroundColor(const int &r, const int &g, const int &b)
{
    setAutoFillBackground(true);

    QPalette pal;

    pal.setColor(QPalette::Window, QColor(r, g, b));

    setPalette(pal);
}

void Menu::ChangeBackgroundPicture(const QString &path)
{
    setAutoFillBackground(true);

    QPalette pal;

    QPixmap pic(path);

    QBrush br;
    br.setTexture(pic);

    pal.setBrush(QPalette::Window, br);

    setPalette(pal);
}

void Menu::AddImage(QHBoxLayout *layout, const QString &ImagePath, const int& size)
{
    QPixmap picture(ImagePath);

    QLabel* itm = new QLabel(this);

    itm->setPixmap(picture.scaled(size, size));

    layout->addWidget(itm);

    items.push_back(std::move(itm));
}

void Menu::AddText(QHBoxLayout *layout, const QString &text, const QVector3D& textcolor, const int &size)
{
    QLabel* itm = new QLabel(this);

    QFont font;
    font.setPointSize(size);

    QPalette fontColor;
    fontColor.setColor(itm->foregroundRole(), QColor(textcolor.x(), textcolor.y(), textcolor.z()));

    itm->setPalette(fontColor);
    itm->setFont(font);
    itm->setText(text);

    layout->addWidget(itm);

    items.push_back(std::move(itm));
}
