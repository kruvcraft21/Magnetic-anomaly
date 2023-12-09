#ifndef MYMATH_H
#define MYMATH_H

#include <QSplineSeries>
#include <QLineSeries>
#include <math.h>
#include <string>

constexpr float MAX_RANGE = 500.f;
constexpr float STEP_RANGE = 50.f;

struct MathParametrs
{
    float b;
    float B;
    float h;
    float X;
};

struct Lines
{
    QtCharts::QSplineSeries* H;
    QtCharts::QSplineSeries* Z;
    QtCharts::QSplineSeries* T;
};

class Math
{
private:
    Math() = default;
    Math(const Math&&) = delete;
    Math(const Math&) = delete;
    Math operator=(const Math&) = delete;
    Math operator=(const Math&&) = delete;
public:
    bool getLinesSeries(Lines& lines, const MathParametrs& parametrs) const noexcept;
    bool getModelLine(QtCharts::QLineSeries* line, const MathParametrs& parametrs) const noexcept;
    static Math& getInstance();

public:
    mutable std::string lastError {""};
};

#endif // MYMATH_H
