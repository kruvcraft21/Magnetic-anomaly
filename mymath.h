#ifndef MYMATH_H
#define MYMATH_H

#include <QSplineSeries>
#include <QLineSeries>
#include <math.h>
#include <string>

constexpr int COUNT_STEP = 20;
constexpr float tolerance = 0.001f;

struct MathParametrs
{
    const float b;
    const float B;
    const float h;
    const float X;
    float yMax;
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
    bool getLinesSeries(Lines& lines, MathParametrs& parametrs) const noexcept;
    bool getModelLine(QtCharts::QLineSeries* line, const MathParametrs& parametrs) const noexcept;
    static Math& getInstance();
    std::string& getLastError() const;

private:
    mutable std::string lastError {""};
};

#endif // MYMATH_H
