#include "menu.h"
#include "ui_menu.h"

Menu::Menu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Menu)
{
    ui->setupUi(this);

    ui->lobby->hide();
    ui->game->hide();

    qDebug() << QString::number(ui->openGLWidget->width()) +  " " + QString::number(ui->openGLWidget->height());

}

Menu::~Menu()
{
    delete ui;
}

void Menu::on_startGameButton_clicked()
{
    ui->lobby->hide();
    ui->game->show();

    qApp->installEventFilter(this);


    ChangeBackgroundColor(127, 145, 173);

    items.reserve(30);

    w = new Widget(players, ui->openGLWidget);

    w->resize(ui->openGLWidget->width(), ui->openGLWidget->height());

    AddText(ui->BuildingText, "f1 - create unit", QVector3D{255, 255, 255});
    AddImage(ui->CharacterImages,":/icons/Cards_Knight.png");
    AddImage(ui->CharacterImages,":/icons/Cards_Archers.png");
    AddImage(ui->CharacterImages,":/icons/Cards_Heal.png");
    AddImage(ui->CharacterImages,":/icons/Cards_Wizard.png");
    AddImage(ui->CharacterImages,":/icons/Cards_Golem.png");

    AddText(ui->BuildingText, "f2 - create building", QVector3D{255, 255, 255});

    connect(this, &Menu::TurnFinished, w, &Widget::FinishTurn);
    connect(this, &Menu::KeyPress, w, &Widget::keyPress);
    connect(w, &Widget::ChangeGold, this, &Menu::change_gold_label);

    w->show();

}


void Menu::on_addPlayersButton_clicked()
{
    if(players.size() == 4){
        ui->playerCount->setText("Total players amount: " + QString::number(players.size()) + "\n Max amount of player");
        return;
    }

    players.append(new Player(playersTextures[players.size()]));

    ui->playerCount->setText("Total players amount: " + QString::number(players.size()));
}


void Menu::on_openGLWidget_aboutToResize()
{
    //   w->resize(ui->openGLWidget->width(), ui->openGLWidget->height());
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


void Menu::on_createGameButton_clicked()
{
    ui->menu->hide();
    ui->lobby->show();

    QVector<QImage*> tmp;

    tmp.append(new QImage(":/playersColors/texture1.purple.jpg"));
    tmp.append(new QImage(":/playersColors/texture2.purple.jpg"));
    tmp.append(new QImage(":/playersColors/texture3.purple.jpg"));
    tmp.append(new QImage(":/playersColors/texture4.purple.jpg"));
    tmp.append(new QImage(":/playersColors/texture5.purple.jpg"));
    playersTextures.append(tmp);
    qDebug() << playersTextures[0];
    tmp.clear();

    tmp.append(new QImage(":/playersColors/texture1.red.jpg"));
    tmp.append(new QImage(":/playersColors/texture2.red.jpg"));
    tmp.append(new QImage(":/playersColors/texture3.red.jpg"));
    tmp.append(new QImage(":/playersColors/texture4.red.jpg"));
    tmp.append(new QImage(":/playersColors/texture5.red.jpg"));
    playersTextures.append(tmp);
    tmp.clear();

    tmp.append(new QImage(":/playersColors/texture1.turquoise.jpg"));
    tmp.append(new QImage(":/playersColors/texture2.turquoise.jpg"));
    tmp.append(new QImage(":/playersColors/texture3.turquoise.jpg"));
    tmp.append(new QImage(":/playersColors/texture4.turquoise.jpg"));
    tmp.append(new QImage(":/playersColors/texture5.turquoise.jpg"));
    playersTextures.append(tmp);
    tmp.clear();

    tmp.append(new QImage(":/playersColors/texture1.yellow.jpg"));
    tmp.append(new QImage(":/playersColors/texture2.yellow.jpg"));
    tmp.append(new QImage(":/playersColors/texture3.yellow.jpg"));
    tmp.append(new QImage(":/playersColors/texture4.yellow.jpg"));
    tmp.append(new QImage(":/playersColors/texture5.yellow.jpg"));
    playersTextures.append(tmp);
    tmp.clear();

    players.append(new Player(playersTextures[players.size()]));
    players.append(new Player(playersTextures[players.size()]));
    ui->playerCount->setText("Total players amount: " + QString::number(players.size()));

}

void Menu::change_gold_label(int money, int income)
{
    ui->gold->setText("Money: " + QString::number(money) + "(+" + QString::number(income) + ")");
}


void Menu::on_nextTurn_clicked()
{
    emit TurnFinished();
}


