#ifndef MODELPAGEWIDGET_H
#define MODELPAGEWIDGET_H

#include <QWidget>
#include <QScrollArea>
#include <QGridLayout>
#include <QVector>

#include "SingleImageItem.h"

class ModelPageWidget : public QScrollArea
{
    Q_OBJECT

public:
    explicit ModelPageWidget(const QString& imgDir, QWidget *parent = nullptr);
    void createItems();
    const QString& getCurrentImgPath(void);

private slots:
    void itemClicked(uint idx);

private:
    QWidget* m_widget;
    QScrollArea* m_scrollArea;
    QGridLayout* m_layout;
    const QString& m_imgDir;

    QVector<SingleImageItem*> m_pItemVec;
    QVector<QString> m_imgPathVec;
    uint m_currentItemIdx;

signals:
    void sigItemChanged();

};

#endif // MODELPAGEWIDGET_H
