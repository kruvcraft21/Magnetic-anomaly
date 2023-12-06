#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent, Qt::WindowCloseButtonHint)
    , mymath(Math::getInstance())
    , ui(new Ui::MainWindow())
{
    ui->setupUi(this);
    this->setupChart();
    this->setupModel();
    connect(this->ui->pushButton, &QPushButton::clicked, this, &MainWindow::click_result);
    connect(this->ui->line_X, &QLineEdit::editingFinished, this, &MainWindow::click_result);
    connect(this->ui->line_b, &QLineEdit::editingFinished, this, &MainWindow::click_result);
    connect(this->ui->line_B, &QLineEdit::editingFinished, this, &MainWindow::click_result);
    connect(this->ui->line_h, &QLineEdit::editingFinished, this, &MainWindow::click_result);
}

void MainWindow::setupChart()
{
    chartView = new QtCharts::QChartView;
    chartView->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    chartView->setMaximumSize(700, 400);
    this->ui->graphicsLayout->addWidget(chartView);
    chartView->chart()->setTitle("График математической модели");
    chartView->chart()->setToolTip("H - fgdfg\nZ - sdf\nT - sfdg");
}

void MainWindow::setupModel()
{
    modelView = new QtCharts::QChartView;
    modelView->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    modelView->setMaximumSize(700, 400);
    this->ui->graphicsLayout->addWidget(modelView);
    modelView->chart()->setTitle("Модель вертикального пласта");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupAxis(const QList<QtCharts::QAbstractAxis *> &chartAxis, const QList<QtCharts::QAbstractAxis *> modelAxis)
{
    modelAxis.at(0)->setRange(-MAX_RANGE, MAX_RANGE);
    auto* modelVerticalAxis = static_cast<QtCharts::QValueAxis*>(modelAxis.at(1));
    modelVerticalAxis->setReverse(true);
    modelVerticalAxis->setRange(0, MAX_RANGE);
    auto modelMaxY = QString::number(modelVerticalAxis->max());

    auto* chartVerticalAxis = static_cast<QtCharts::QValueAxis*>(chartAxis.at(1));
    auto chartMaxY = QString::number(chartVerticalAxis->max());
}

void MainWindow::click_result()
{
    MathParametrs param{0.f, 0.f, 0.f, 0.f};
    bool okh = true;
    bool okB = true;
    bool okb = true;
    bool okX = true;
    param.b = this->ui->line_b->text().replace(',', '.').toFloat(&okb);
    param.B = this->ui->line_B->text().replace(',', '.').toFloat(&okB);
    param.h = this->ui->line_h->text().replace(',', '.').toFloat(&okh);
    param.X = this->ui->line_X->text().replace(',', '.').toFloat(&okX);
    if (okh && okb && okB && okX)
    {
        Lines lines
        {
            new QtCharts::QSplineSeries,
            new QtCharts::QSplineSeries,
            new QtCharts::QSplineSeries
        };
        auto* modelLine = new QtCharts::QLineSeries;
        if (mymath.getLinesSeries(lines, param) && mymath.getModelLine(modelLine, param))
        {
            chartView->chart()->removeAllSeries();
            chartView->chart()->addSeries(lines.H);
            chartView->chart()->addSeries(lines.Z);
            chartView->chart()->addSeries(lines.T);
            chartView->chart()->createDefaultAxes();

            modelView->chart()->removeAllSeries();
            modelView->chart()->addSeries(modelLine);
            modelView->chart()->createDefaultAxes();
            this->setupAxis(chartView->chart()->axes(), modelView->chart()->axes());
        }
        else
        {
            delete modelLine;
            delete lines.H;
            delete lines.Z;
            delete lines.T;
        }
    }
}

