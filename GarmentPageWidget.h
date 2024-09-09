#ifndef GARMENTPAGEWIDGET_H
#define GARMENTPAGEWIDGET_H

#include <QWidget>
#include <QScrollArea>
#include <QGridLayout>
#include <QVector>
#include <QMap>
#include "GroupItem.h"

class GarmentPageWidget : public QScrollArea
{
    Q_OBJECT
public:
    explicit GarmentPageWidget(const QString& imgPath, QWidget *parent = nullptr);
    void createItems();
    void countTopBottomSelected(QMap<QString, int>& mapCount, QMap<QString, QString>& mapImgPath);

private slots:
    void itemSelectedChanged(uint idx, bool bSelected);
    void topChkBoxClicked(uint idx, int state);
    void bottomChkBoxClicked(uint idx, int state);

private:
    QWidget* m_widget;
    QScrollArea* m_scrollArea;
    QGridLayout* m_layout;
    const QString& m_imgPath;
    QVector<uint> m_itemsSelectedVec;
    QVector<GroupItem*> m_pItemVec;

signals:

};

#endif // GARMENTPAGEWIDGET_H
