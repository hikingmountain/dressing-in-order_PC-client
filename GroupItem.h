#ifndef GROUPITEM_H
#define GROUPITEM_H

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

class GroupItem: public QWidget
{
    Q_OBJECT

public:
    GroupItem(const QString& imgPath, const uint index, QWidget* parent = nullptr);
    ~GroupItem();
    void setTopChkState(bool bState);
    void setBottomChkState(bool bState);
    void setItemSelectedState(bool bSelected);
    int getTopChkState();
    int getBottomChkState();
    uint getItemIndex(void);
    QString getImgPath();

protected:
    QImage* createImage();
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void enterEvent(QEnterEvent *event);
    void leaveEvent(QEvent *event);

private slots:
    void topChkBoxStateChanged(int state);
    void bottomChkBoxStateChanged(int state);

signals:
    void sigItemClicked(uint idx, bool bSelected);
    void sigTopChkBoxClicked(uint idx, int state);
    void sigBottomChkBoxClicked(uint idx, int state);

private:
    QCheckBox* m_topChkBox;
    QCheckBox* m_bottomChkBox;
    QVBoxLayout* m_mainLayout;
    QWidget* m_bottomWidgt;
    QHBoxLayout* m_checkBoxLayout;
    bool m_bMouseIn;
    bool m_bItemSelected;

    QString m_imgPath;
    QLabel* m_imgLabel;
    uint m_index;
};

#endif // GROUPITEM_H
