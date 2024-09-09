#include <QPixmap>
#include <QMap>
#include <QMessageBox>
#include <QCoreApplication>
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_pyCaller = new PyCaller;
    this->setFixedSize(800, 600);
    m_mainWidgt = new QWidget(this);
    m_mainWidgt->setGeometry(0, 0, 800, 600);

    m_mainImgLabel = new QLabel;
    m_mainImgLabel->setGeometry(0, 0, 176, 256);

    m_configReader = JsonReader();
    const QString model_img_path = m_configReader.getValue("model_img_path");
    m_modelPage = new ModelPageWidget(model_img_path);
    modelItemChanged();
    connect(m_modelPage, SIGNAL(sigItemChanged()), this, SLOT(modelItemChanged()));

    const QString garment_img_path = m_configReader.getValue("garment_img_path");
    m_garmentPage = new GarmentPageWidget(garment_img_path);

    m_tabWidget = new QTabWidget;
    m_tabWidget->addTab(m_modelPage, "Model");
    m_tabWidget->addTab(m_garmentPage, "Garment");

    m_mainLayout = new QHBoxLayout;
    m_mainLayout->setSpacing(10);
    m_mainWidgt->setLayout(m_mainLayout);

    m_leftAreaLayout = new QVBoxLayout;
    m_leftAreaLayout->setSpacing(10);
    m_mainLayout->addLayout(m_leftAreaLayout);

    m_leftAreaLayout->addWidget(m_mainImgLabel, 0, Qt::AlignHCenter);

    m_garmentImgLabel = new QLabel;
    m_garmentImgLabel->setFixedSize(280, 205);
    m_garmentImgLabel->setGeometry(0, 0, 280, 205);
    m_leftAreaLayout->addWidget(m_garmentImgLabel);

    m_rightAreaLayout = new QVBoxLayout;
    m_mainLayout->addLayout(m_rightAreaLayout);


    m_rightAreaLayout->addWidget(m_tabWidget, 0, Qt::AlignTop | Qt::AlignLeft);

    m_rightBottomLayout = new QHBoxLayout;
    m_tuckCheckBox = new QCheckBox("Tuck");
    m_rightBottomLayout->addWidget(m_tuckCheckBox, 0, Qt::AlignLeft | Qt::AlignVCenter);

    m_tryonBtn = new QPushButton("Try on");
    m_tryonBtn->setFixedSize(80, 30);

    m_tryonBtn->setStyleSheet("QPushButton{border-image: url(:/button_img/b1.png);}"
                               "QPushButton:hover{border-image: url(:/button_img/b2.png); color: blue;}"
                               "QPushButton:pressed{border-image: url(:/button_img/b3.png); color: black;}"
                               );

    m_rightBottomLayout->addWidget(m_tryonBtn, 0, Qt::AlignLeft | Qt::AlignBottom);

    m_rightBottomLayout->addStretch(1);
    m_rightAreaLayout->addLayout(m_rightBottomLayout);

    connect(m_tryonBtn, SIGNAL(clicked()), this, SLOT(tryonButtonClicked()));

}

MainWindow::~MainWindow() {}

void MainWindow::tryonButtonClicked()
{
    QMap<QString, int> mapCount;
    QMap<QString, QString> mapImgPath;
    QMessageBox msgBox;
    m_garmentPage->countTopBottomSelected(mapCount, mapImgPath);
    if (mapCount.value("top") == 0 && mapCount.value("bottom") == 0)
    {
        msgBox.setText("Please select one top and one bottom from garment images!");
        msgBox.exec();
        return;
    }
    else if (mapCount.value("top") == 0 && mapCount.value("bottom") == 1)
    {
        msgBox.setText("Please select a top from garment images!");
        msgBox.exec();
        return;
    }
    else if (mapCount.value("top") == 1 && mapCount.value("bottom") == 0)
    {
        msgBox.setText("Please select a bottom from garment images!");
        msgBox.exec();
        return;
    }

    int tuckState = m_tuckCheckBox->checkState();

    m_pyCaller->try_on(m_modelPage->getCurrentImgPath(), mapImgPath, tuckState==Qt::Checked ? true:false );

    QString currentDirPath = QCoreApplication::applicationDirPath();
    QPixmap genImg(currentDirPath + "/" + "gen_img.jpg");
    m_mainImgLabel->setPixmap(genImg);

    QPixmap garmentImg(currentDirPath + "/" + "gimg.jpg");
    // scaled 0.8
    QPixmap smallGarmentImg = garmentImg.scaled(280, 205, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    m_garmentImgLabel->setPixmap(smallGarmentImg);

}

void MainWindow::modelItemChanged()
{
    const QString modelImgPath = m_modelPage->getCurrentImgPath();
    QPixmap modelImg(modelImgPath);
    m_mainImgLabel->setPixmap(modelImg.scaled(176, 256, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}


