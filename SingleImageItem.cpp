#include "SingleImageItem.h"

SingleImageItem::SingleImageItem(const QString &imgPath, const uint index, QWidget *parent):
    QWidget(parent), m_bMouseIn(false), m_bItemSelected(false)
{
    this->setFixedSize(ITEM_WIDTH, ITEM_HEIGHT);
    m_index = index;

    m_imgPath = imgPath;
    QImage* img = createImage();
    m_imgLabel = new QLabel(this);
    m_imgLabel->setStyleSheet(("border: 0px;"));
    m_imgLabel->setPixmap(QPixmap::fromImage(*img));
    m_imgLabel->setGeometry((ITEM_WIDTH-IMAGE_WIDTH)/2, (ITEM_HEIGHT-IMAGE_HEIGHT)/2, IMAGE_WIDTH, IMAGE_HEIGHT);

    this->update();

}

QImage* SingleImageItem::createImage()
{
    QImage large_img = QImage(m_imgPath);
    QImage small_img = large_img.scaled(IMAGE_WIDTH, IMAGE_HEIGHT, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    return new QImage(small_img);
}

void SingleImageItem::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    if (m_bMouseIn)
        painter.setPen(QPen(Qt::red, 2));
    else if (m_bItemSelected)
        painter.setPen(QPen(Qt::blue, 2));
    else
    {
        painter.setPen(QPen(Qt::black, 1));
    }

    painter.drawRect(2, 2, ITEM_WIDTH-4, ITEM_HEIGHT-4);

}

void SingleImageItem::enterEvent(QEnterEvent *event)
{
    Q_UNUSED(event);

    m_bMouseIn = true;
    this->update();
}

void SingleImageItem::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    m_bItemSelected = true;
    m_bMouseIn = false;
    if (m_bItemSelected)
    {
        emit sigItemClicked(m_index);
    }

    this->update();
}

void SingleImageItem::setItemSelected(bool bSelected)
{
    m_bItemSelected = bSelected;
    this->update();
}

void SingleImageItem::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);

    m_bMouseIn = false;
    this->update();
}

SingleImageItem::~SingleImageItem()
{

}
