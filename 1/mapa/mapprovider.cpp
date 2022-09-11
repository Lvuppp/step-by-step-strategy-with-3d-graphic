#include "mapprovider.h"

MapProvider::MapProvider(QQuickItem *parent) :
    QQuickPaintedItem(parent)
{
    connect(this, SIGNAL(xChanged()), this, SLOT(update()));
    connect(this, SIGNAL(yChanged()), this, SLOT(update()));
}

void MapProvider::paint(QPainter *painter)
{
    // получаем номера колонки и строки, с которых начинается отрисовка
    int startcol = qFloor(m_cx / m_tilesizew);
    int startrow = qFloor(m_cy / m_tilesizeh);

    // рассчитываем количество видимых тайлов
    int tilecountw = qFloor(width() / m_tilesize);
    int tilecounth = qFloor(height() / m_tilesize) * 4;
    int tilecount = tilecountw * tilecounth;

    int col, row, globcol, globrow, globid = 0;
    double tx, ty = 0.0f;
    bool iseven;
    QPixmap tile;
    QString tileSourceID;

    for(int tileid = 0; tileid < tilecount; tileid++) {
        // узнаем к какой колонке и строке относится тайл
        col = tileid % tilecountw;
        row = qFloor(tileid / tilecountw) ;

        // узнаем реальные колонку, строку и индекс тайла
        globcol = col + startcol;
        globrow = row + startrow * 2;
        globid = m_mapcols * globrow + globcol;

        // если вдруг описание карты было заполнено неправильно
        // то на карте появится белая дыра
        if(globid >= m_mapdata.size()) {
            return;
        }
        // не рисуем то, что осталось за пределами видимости
        else if(globcol >= m_mapcols || globrow >= m_maprows) {
            continue;
        }

        // определяем чётность строки
        iseven = !(row&1);

        // вычисляем позицию тайла
        tx = iseven ? col * m_tilesizew : col * m_tilesizew + m_tilesizew/2;
        ty = iseven ? row * m_tilesizeh : row * m_tilesizeh - m_tilesizeh/2;

        ty -= qFloor(row/2) * m_tilesizeh;

        // вытягиваем название ресурса по его индексу
        tileSourceID = m_mapdata.at(globid).toString();

        // достаем картинку из кеша, если она там есть
        if(tileCache.contains(tileSourceID)) {
            tile = tileCache.value(tileSourceID);
        }
        // либо создаем картинку и скидываем в массив
        else {
            tile = QPixmap(QString(":/assets/texture/%1.png").arg(tileSourceID))
                    .scaled(QSize(m_tilesize, m_tilesize),
                            Qt::IgnoreAspectRatio,
                            Qt::SmoothTransformation);

            tileCache.insert(tileSourceID, tile);
        }

        // рисуем тайл
        painter->drawPixmap(tx, ty, tile);

        // подписываем информацию о тайле
        painter->setFont(QFont("Helvetica", 8));
        painter->setPen(QColor(255, 255, 255, 100));

        painter->drawText(QRectF(tx, ty, m_tilesizew, m_tilesizeh),
                          Qt::AlignCenter,
                          QString("%1\n%2:%3").arg(globid).arg(globcol).arg(globrow));
    }
}

double MapProvider::tilesize() const
{
    return m_tilesize;
}

double MapProvider::mapcols() const
{
    return m_mapcols;
}

double MapProvider::maprows() const
{
    return m_maprows;
}

double MapProvider::cx() const
{
    return m_cx;
}

double MapProvider::cy() const
{
    return m_cy;
}

QVariantList MapProvider::mapdata() const
{
    return m_mapdata;
}

double MapProvider::tilesizew() const
{
    return m_tilesizew;
}

double MapProvider::tilesizeh() const
{
    return m_tilesizeh;
}

void MapProvider::setTilesize(double tilesize)
{
    if (m_tilesize == tilesize)
        return;

    m_tilesize = tilesize;
    emit tilesizeChanged(tilesize);
}

void MapProvider::setMapcols(double mapcols)
{
    if (m_mapcols == mapcols)
        return;

    m_mapcols = mapcols;
    emit mapcolsChanged(mapcols);
}

void MapProvider::setMaprows(double maprows)
{
    if (m_maprows == maprows)
        return;

    m_maprows = maprows;
    emit maprowsChanged(maprows);
}

void MapProvider::setCx(double cx)
{
    if (m_cx == cx)
        return;

    m_cx = cx;
    emit cxChanged(cx);
}

void MapProvider::setCy(double cy)
{
    if (m_cy == cy)
        return;

    m_cy = cy;
    emit cyChanged(cy);
}

void MapProvider::setMapdata(QVariantList mapData)
{
    if (m_mapdata == mapData)
        return;

    m_mapdata = mapData;
    emit mapDatachanged(mapData);
}

void MapProvider::setTilesizew(double tilesizew)
{
    if (m_tilesizew == tilesizew)
        return;

    m_tilesizew = tilesizew;
    emit tilesizewChanged(tilesizew);
}

void MapProvider::setTilesizeh(double tilesizeh)
{
    if (m_tilesizeh == tilesizeh)
        return;

    m_tilesizeh = tilesizeh;
    emit tilesizehChanged(tilesizeh);
}

