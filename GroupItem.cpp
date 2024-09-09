#include "GroupItem.h"

GroupItem::GroupItem(const QString &imgPath, const uint index, QWidget *parent):
    QWidget(parent), m_bMouseIn(false), m_bItemSelected(false)
{
    this->setFixedSize(ITEM_WIDTH, ITEM_HEIGHT);
    m_index = index;

    m_imgPath = imgPath;
    QImage* img = createImage();
    m_imgLabel = new QLabel;
    m_imgLabel->setStyleSheet(("border: 0px;"));
    m_imgLabel->setPixmap(QPixmap::fromImage(*img));

    m_mainLayout = new QVBoxLayout;
    m_mainLayout->setContentsMargins(0, 5, 0, 0);

    m_mainLayout->addWidget(m_imgLabel, 0, Qt::AlignHCenter | Qt::AlignTop);

    m_bottomWidgt = new QWidget;
    m_bottomWidgt->setFixedSize(140, 25);

    m_checkBoxLayout = new QHBoxLayout;
    m_checkBoxLayout->setContentsMargins(5, 0, 0, 0);
    m_checkBoxLayout->setSpacing(0);
    m_bottomWidgt->setLayout(m_checkBoxLayout);

    m_topChkBox = new QCheckBox("Top");
    m_topChkBox->setVisible(false);
    m_bottomChkBox = new QCheckBox("Bottom");
    m_bottomChkBox->setVisible(false);
    m_checkBoxLayout->addWidget(m_topChkBox, 0, Qt::AlignBottom);
    m_checkBoxLayout->addWidget(m_bottomChkBox, 0, Qt::AlignBottom);
    connect(m_topChkBox, SIGNAL(stateChanged(int)), this, SLOT(topChkBoxStateChanged(int)));
    connect(m_bottomChkBox, SIGNAL(stateChanged(int)), this, SLOT(bottomChkBoxStateChanged(int)));

    m_mainLayout->addWidget(m_bottomWidgt, 0, Qt::AlignBottom | Qt::AlignLeft);

    this->setLayout(m_mainLayout);

    this->update();

}

void GroupItem::topChkBoxStateChanged(int state)
{
    emit sigTopChkBoxClicked(m_index, state);
}

QString GroupItem::getImgPath()
{
    return m_imgPath;
}

void GroupItem::bottomChkBoxStateChanged(int state)
{
    emit sigBottomChkBoxClicked(m_index, state);
}

int GroupItem::getTopChkState()
{
    m_topChkBox->checkState();
}

int GroupItem::getBottomChkState()
{
    m_bottomChkBox->checkState();
}

QImage* GroupItem::createImage()
{
    QImage large_img = QImage(m_imgPath);
    QImage small_img = large_img.scaled(IMAGE_WIDTH, IMAGE_HEIGHT, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    return new QImage(small_img);
}

void GroupItem::setTopChkState(bool bState)
{
    m_topChkBox->setChecked(bState);
}

void GroupItem::setBottomChkState(bool bState)
{
    m_bottomChkBox->setChecked(bState);
}

uint GroupItem::getItemIndex()
{
    return m_index;
}

void GroupItem::paintEvent(QPaintEvent *event)
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

void GroupItem::enterEvent(QEnterEvent *event)
{
    Q_UNUSED(event);

    m_bMouseIn = true;
    this->update();
}

void GroupItem::setItemSelectedState(bool bSelected)
{
    m_bItemSelected = bSelected;
    if (m_bItemSelected)
    {
        m_topChkBox->setVisible(true);
        m_bottomChkBox->setVisible(true);
    }
    else
    {
        m_topChkBox->setChecked(0);
        m_bottomChkBox->setChecked(0);
        m_topChkBox->setVisible(false);
        m_bottomChkBox->setVisible(false);
    }
    this->update();
}

void GroupItem::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    m_bItemSelected = m_bItemSelected ? false: true;
    m_bMouseIn = false;
    if (m_bItemSelected)
    {
        m_topChkBox->setVisible(true);
        m_bottomChkBox->setVisible(true);
    }
    else
    {
        m_topChkBox->setChecked(0);
        m_bottomChkBox->setChecked(0);
        m_topChkBox->setVisible(false);
        m_bottomChkBox->setVisible(false);
    }

    emit sigItemClicked(m_index, m_bItemSelected);

    this->update();
}

void GroupItem::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);

    m_bMouseIn = false;
    this->update();
}

GroupItem::~GroupItem()
{

}
