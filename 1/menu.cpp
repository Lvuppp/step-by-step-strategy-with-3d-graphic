#include "menu.h"
#include "ui_menu.h"

//#define N 4

Menu::Menu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Menu)
{

    ui->setupUi(this);
    qApp->installEventFilter(this);

    ChangeBackgroundColor(127, 145, 173);

    items.reserve(30);

    w = new Widget(ui->openGLWidget);

    w->resize(ui->openGLWidget->width(), ui->openGLWidget->height());

    AddImage(ui->CharacterImages, ":/icons/Cards_Knight.png", 80);
    AddImage(ui->CharacterImages, ":/icons/Cards_Archers.png", 80);
    AddImage(ui->CharacterImages, ":/icons/Cards_Heal.png", 80);
    AddImage(ui->CharacterImages, ":/icons/Cards_Wizard.png", 80);
    AddImage(ui->CharacterImages, ":/icons/Cards_Golem.png", 80);

    AddImage(ui->BuildingImages, ":/cl_ppl.jpg");

    AddText(ui->BuildingText, "1", QVector3D{255, 255, 255});

    connect(this, &Menu::TurnFinished, w, &Widget::FinishTurn);
    connect(this, &Menu::KeyPress, w, &Widget::keyPress);

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

    itm->setPixmap(picture.scaled(size * 0.83, size));

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

void Menu::keyPressEvent(QKeyEvent *event)
{
    emit KeyPress(event);
}

void Menu::on_pushButton_clicked()
{
    emit TurnFinished();
}

