#include <QFileDialog>
#include <QDebug>
#include "GarmentPageWidget.h"
#include "SingleImageItem.h"

GarmentPageWidget::GarmentPageWidget(const QString& imgPath, QWidget *parent)
    : QScrollArea{parent}, m_imgPath(imgPath)
{
    m_widget = new QWidget;
    m_widget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    m_layout = new QGridLayout;
    m_layout->setSpacing(10);

    m_widget->setLayout(m_layout);

    this->setFixedSize(490, 520);

    this->setWidgetResizable(true);
    this->setWidget(m_widget);

    createItems();

}

void GarmentPageWidget::itemSelectedChanged(uint idx, bool bSelected)
{
    if (!bSelected)
    {
        m_pItemVec[idx]->setTopChkState(false);
        m_pItemVec[idx]->setBottomChkState(false);

        for (auto it=m_itemsSelectedVec.begin(); it != m_itemsSelectedVec.end(); )
        {
            if (*it == idx)
            {
                it = m_itemsSelectedVec.erase(it);
                break;
            }
            else
            {
                ++it;
            }
        }
    }
    else
    {
        if (m_itemsSelectedVec.count() < 2)
        {
            m_itemsSelectedVec.push_back(idx);
        }
        else
        {
            uint k = m_itemsSelectedVec[0];
            m_pItemVec[k]->setItemSelectedState(false);
            m_itemsSelectedVec.pop_front();
            m_itemsSelectedVec.push_back(idx);
        }
    }
}

void GarmentPageWidget::countTopBottomSelected(QMap<QString, int>& mapCount, QMap<QString,QString>& mapImgPath)
{
    int top = 0, bottom = 0;
    for (auto it=m_itemsSelectedVec.begin(); it != m_itemsSelectedVec.end(); ++it)
    {
        if (m_pItemVec[*it]->getTopChkState() == Qt::Checked)
        {
            mapImgPath["top"] = m_pItemVec[*it]->getImgPath();
            top++;
        }
        if (m_pItemVec[*it]->getBottomChkState() == Qt::Checked)
        {
            mapImgPath["bottom"] = m_pItemVec[*it]->getImgPath();
            bottom++;
        }
    }
    mapCount["top"] = top;
    mapCount["bottom"] = bottom;
}

void GarmentPageWidget::topChkBoxClicked(uint idx, int state)
{
    if (state == 2 && m_itemsSelectedVec.count() == 2)
    {
        for (auto it = m_itemsSelectedVec.begin(); it != m_itemsSelectedVec.end(); ++it)
        {
            if (*it != idx && m_pItemVec[*it]->getTopChkState() == 2)
            {
                m_pItemVec[*it]->setTopChkState(false);
                break;
            }
        }
    }
}

void GarmentPageWidget::bottomChkBoxClicked(uint idx, int state)
{
    if (state == 2 && m_itemsSelectedVec.count() == 2)
    {
        for (auto it = m_itemsSelectedVec.begin(); it != m_itemsSelectedVec.end(); ++it)
        {
            if (*it != idx && m_pItemVec[*it]->getBottomChkState() == 2)
            {
                m_pItemVec[*it]->setBottomChkState(false);
                break;
            }
        }
    }
}

void GarmentPageWidget::createItems()
{
    QDir dir(m_imgPath);
    QFileInfoList fileInfoList = dir.entryInfoList(QDir::Files);
    QString filePath;

    GroupItem* pGrpItem = nullptr;
    for (int i=0; i<fileInfoList.count(); i++)
    {
        if (fileInfoList.at(i).fileName() == "." || fileInfoList.at(i).fileName() == "..")
            continue;
        filePath.clear();
        filePath.append(m_imgPath + "/" + fileInfoList.at(i).fileName());
        pGrpItem = new GroupItem(filePath, i);
        m_layout->addWidget(pGrpItem, m_layout->count() / 3, m_layout->count() % 3);
        m_pItemVec.push_back(pGrpItem);
        connect(pGrpItem, SIGNAL(sigItemClicked(uint,bool)), this, SLOT(itemSelectedChanged(uint,bool)));
        connect(pGrpItem, SIGNAL(sigTopChkBoxClicked(uint,int)), this, SLOT(topChkBoxClicked(uint,int)));
        connect(pGrpItem, SIGNAL(sigBottomChkBoxClicked(uint,int)), this, SLOT(bottomChkBoxClicked(uint,int)));
    }
}

