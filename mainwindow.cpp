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
        if (mymath.getLinesSeries(lines, param))
        {
            chartView->chart()->removeAllSeries();
            chartView->chart()->addSeries(lines.H);
            chartView->chart()->addSeries(lines.Z);
            chartView->chart()->addSeries(lines.T);
            chartView->chart()->createDefaultAxes();

            const float maxY = param.h * 3;
            modelView->chart()->removeAllSeries();
            auto* line = new QtCharts::QLineSeries;
            line->setName("Конут пласта");
            line->append(-param.b, maxY);
            line->append(-param.b, maxY - param.h);
            line->append(param.b, maxY - param.h);
            line->append(param.b, maxY);
            modelView->chart()->addSeries(line);
            modelView->chart()->createDefaultAxes();
            modelView->chart()->axes().at(0)->setRange(-MAX_RANGE, MAX_RANGE);
            modelView->chart()->axes().at(1)->setReverse(true);
            modelView->chart()->axes().at(1)->setRange(0, maxY);
        }
        else
        {
            delete lines.H;
            delete lines.Z;
            delete lines.T;
        }
    }
}

