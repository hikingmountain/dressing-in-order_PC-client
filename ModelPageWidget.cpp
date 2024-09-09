#include <QFileDialog>
#include <QDebug>
#include "GroupItem.h"
#include "ModelPageWidget.h"


ModelPageWidget::ModelPageWidget(const QString& imgDir, QWidget *parent)
    : QScrollArea{parent}, m_imgDir(imgDir)
{
    m_currentItemIdx = 0;
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

void ModelPageWidget::createItems()
{
    QDir dir(m_imgDir);
    QFileInfoList fileInfoList = dir.entryInfoList(QDir::Files);
    QString filePath;

    SingleImageItem* pImgItem = nullptr;
    for (int i=0; i<fileInfoList.count(); i++)
    {
        if (fileInfoList.at(i).fileName() == "." || fileInfoList.at(i).fileName() == "..")
            continue;
        filePath.clear();
        filePath.append(m_imgDir + "/" + fileInfoList.at(i).fileName());
        pImgItem = new SingleImageItem(filePath, i);
        m_layout->addWidget(pImgItem, m_layout->count() / 3, m_layout->count() % 3);

        m_pItemVec.push_back(pImgItem);
        m_imgPathVec.push_back(QString(filePath));
        connect(pImgItem, SIGNAL(sigItemClicked(uint)), this, SLOT(itemClicked(uint)));
    }

    m_pItemVec[0]->setItemSelected(true);
}

const QString& ModelPageWidget::getCurrentImgPath()
{
    return m_imgPathVec[m_currentItemIdx];
}

void ModelPageWidget::itemClicked(uint idx)
{
    if (m_currentItemIdx != idx)
    {
        m_pItemVec[m_currentItemIdx]->setItemSelected(false);
        m_currentItemIdx = idx;
        emit sigItemChanged();
    }
}
