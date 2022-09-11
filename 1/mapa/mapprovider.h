#ifndef MAPPROVIDER_H
#define MAPPROVIDER_H

#include <QQuickItem>
#include <QQuickPaintedItem>
#include <QPainter>
#include <QtMath>
#include <QVariantList>

// дебаг
#include <QDebug>
#include <QDateTime>

class MapProvider : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(double tilesize READ tilesize WRITE setTilesize NOTIFY tilesizeChanged)
    Q_PROPERTY(double tilesizew READ tilesizew WRITE setTilesizew NOTIFY tilesizewChanged)
    Q_PROPERTY(double tilesizeh READ tilesizeh WRITE setTilesizeh NOTIFY tilesizehChanged)
    Q_PROPERTY(double mapcols READ mapcols WRITE setMapcols NOTIFY mapcolsChanged)
    Q_PROPERTY(double maprows READ maprows WRITE setMaprows NOTIFY maprowsChanged)
    Q_PROPERTY(double cx READ cx WRITE setCx NOTIFY cxChanged)
    Q_PROPERTY(double cy READ cy WRITE setCy NOTIFY cyChanged)
    Q_PROPERTY(QVariantList mapdata READ mapdata WRITE setMapdata NOTIFY mapDatachanged)

public:
    MapProvider(QQuickItem *parent = 0);

    void paint(QPainter *painter);

    double cx() const;
    double cy() const;
    double tilesize() const;
    double tilesizew() const;
    double tilesizeh() const;
    double mapcols() const;
    double maprows() const;
    QVariantList mapdata() const;


signals:
    void cxChanged(double cx);
    void cyChanged(double cy);
    void tilesizeChanged(double tilesize);
    void tilesizewChanged(double tilesizew);
    void tilesizehChanged(double tilesizeh);
    void mapcolsChanged(double mapcols);
    void maprowsChanged(double maprows);
    void mapDatachanged(QVariantList mapdata);

public slots:
    void setCx(double cx);
    void setCy(double cy);
    void setTilesize(double tilesize);
    void setTilesizew(double tilesizew);
    void setTilesizeh(double tilesizeh);
    void setMapcols(double mapcols);
    void setMaprows(double maprows);
    void setMapdata(QVariantList mapdata);


private:
    QMap<QString, QPixmap> tileCache;
    QVariantList m_mapdata;

    double m_cx;
    double m_cy;
    double m_tilesize;
    double m_tilesizew;
    double m_tilesizeh;
    double m_mapcols;
    double m_maprows;
};

#endif // MAPPROVIDER_H
