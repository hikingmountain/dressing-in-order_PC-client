#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QTabWidget>
#include <QCheckBox>

#include "ModelPageWidget.h"
#include "GarmentPageWidget.h"
#include "util.h"
#include "pycaller.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void modelItemChanged();
    void tryonButtonClicked();

private:
    QWidget* m_mainWidgt;
    QTabWidget* m_tabWidget;
    GarmentPageWidget* m_garmentPage;
    ModelPageWidget* m_modelPage;

    QHBoxLayout* m_mainLayout;
    QVBoxLayout* m_leftAreaLayout;
    QVBoxLayout* m_rightAreaLayout;
    QHBoxLayout* m_rightBottomLayout;

    QLabel* m_mainImgLabel;
    QLabel* m_garmentImgLabel;

    QCheckBox* m_tuckCheckBox;
    QPushButton* m_tryonBtn;

    JsonReader m_configReader;
    PyCaller* m_pyCaller;

};
#endif // MAINWINDOW_H
