#ifndef SINGLEIMAGEITEM_H
#define SINGLEIMAGEITEM_H

#include <QGroupBox>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>

#define ITEM_WIDTH 140
#define ITEM_HEIGHT 160
#define IMAGE_WIDTH 88
#define IMAGE_HEIGHT 128

class SingleImageItem: public QWidget
{
    Q_OBJECT

public:
    SingleImageItem(const QString& imgPath, const uint index, QWidget* parent = nullptr);
    ~SingleImageItem();
    void setItemSelected(bool bSelected);

protected:
    QImage* createImage();
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void enterEvent(QEnterEvent *event);
    void leaveEvent(QEvent *event);

signals:
    void sigItemClicked(uint idx);

private:

    QVBoxLayout* m_mainLayout;
    bool m_bMouseIn;
    bool m_bItemSelected;

    QString m_imgPath;
    QLabel* m_imgLabel;

    uint m_index;

};

#endif // SINGLEIMAGEITEM_H
