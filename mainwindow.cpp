#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent, Qt::WindowCloseButtonHint)
    , mymath(Math::getInstance())
    , chartView(new QtCharts::QChartView(this))
    , modelView(new QtCharts::QChartView(this))
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
    chartView->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    chartView->setMaximumSize(700, 400);
    this->ui->graphicsLayout->addWidget(chartView);
    chartView->chart()->setTitle("График математической модели");
    chartView->chart()->setToolTip("H - горизонтальная составляющая\nZ - вертикальная составляющая\nT - полный вектор напряжонности");
}

void MainWindow::setupModel()
{
    modelView->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    modelView->setMaximumSize(700, 400);
    this->ui->graphicsLayout->addWidget(modelView);
    modelView->chart()->setTitle("Модель вертикального пласта");
}

void removeAllAxes(QtCharts::QChart* chart)
{
    const auto& axes = chart->axes();
    for (auto* axis : axes)
    {
        chart->removeAxis(axis);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::addAxes(const Axes& axes, QtCharts::QChart* chart)
{
    chart->addAxis(axes.x, Qt::AlignBottom);
    chart->addAxis(axes.y, Qt::AlignLeft);
    const auto& series = chart->series();
    for (auto* s : series)
    {
        s->attachAxis(axes.x);
        s->attachAxis(axes.y);
    }
}

void MainWindow::createAxes(const MathParametrs &parametrs)
{
    removeAllAxes(chartView->chart());
    removeAllAxes(modelView->chart());

    Axes chartAxes
    {
        new QtCharts::QValueAxis(this),
        new QtCharts::QValueAxis(this)
    };
    chartAxes.x->setRange(-parametrs.b * 5.f, parametrs.b * 5.f);
    chartAxes.y->setRange(-parametrs.yMax, parametrs.yMax);
    this->addAxes(chartAxes, chartView->chart());

    Axes modelAxes
    {
        new QtCharts::QValueAxis(this),
        new QtCharts::QValueAxis(this)
    };
    modelAxes.x->setRange(-parametrs.b * 5.f, parametrs.b * 5.f);
    modelAxes.y->setRange(0, parametrs.h * 2);
    modelAxes.y->setReverse(true);
    this->addAxes(modelAxes, modelView->chart());
}

void MainWindow::click_result()
{
    bool okh = true;
    bool okB = true;
    bool okb = true;
    bool okX = true;
    MathParametrs param
    {
        this->ui->line_b->text().replace(',', '.').toFloat(&okb), // b
        this->ui->line_B->text().replace(',', '.').toFloat(&okB), // B
        this->ui->line_h->text().replace(',', '.').toFloat(&okh), // h
        this->ui->line_X->text().replace(',', '.').toFloat(&okX), // X
        0.f, // yMax
    };
    if (okh && okb && okB && okX)
    {
        Lines lines
        {
            new QtCharts::QSplineSeries(this),
            new QtCharts::QSplineSeries(this),
            new QtCharts::QSplineSeries(this)
        };
        auto* modelLine = new QtCharts::QLineSeries;
        if (mymath.getLinesSeries(lines, param) && mymath.getModelLine(modelLine, param))
        {
            chartView->chart()->removeAllSeries();
            chartView->chart()->addSeries(lines.H);
            chartView->chart()->addSeries(lines.Z);
            chartView->chart()->addSeries(lines.T);

            modelView->chart()->removeAllSeries();
            modelView->chart()->addSeries(modelLine);

            this->createAxes(param);
        }
        else
        {
            qDebug() << mymath.getLastError().c_str();
            delete modelLine;
            delete lines.H;
            delete lines.Z;
            delete lines.T;
        }
    }
}

