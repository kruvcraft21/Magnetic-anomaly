#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QChartView>
#include <QValueAxis>
#include "mymath.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void setupChart();
    void setupModel();
    void setupAxis(const QList<QtCharts::QAbstractAxis*>& chartAxis, const QList<QtCharts::QAbstractAxis*> modelAxis);

public slots:
    void click_result();

private:
    Math& mymath;
    QtCharts::QChartView* chartView;
    QtCharts::QChartView* modelView;
    Ui::MainWindow* ui;
};
#endif // MAINWINDOW_H
