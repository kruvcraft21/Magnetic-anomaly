#include "mymath.h"

bool Math::getLinesSeries(Lines& lines, MathParametrs& parametrs) const noexcept
{
    try
    {
        const float helfRange = parametrs.b * 5.f;
        const float stepRange = (helfRange * 2.f) / COUNT_STEP;
        const float J = parametrs.B * parametrs.X;
        for (int i = 0; i <= COUNT_STEP; i++)
        {
            float x = (stepRange * i) + -helfRange;
            auto tmp_H = J * logf((parametrs.h * parametrs.h + powf(x - parametrs.b, 2.f)) / (parametrs.h * parametrs.h + powf(x + parametrs.b, 2.f)));
            auto tmp_Z = 2 * J * atanf((2 * parametrs.b * parametrs.h)/(powf(parametrs.h, 2.f) + powf(x, 2.f) - powf(parametrs.b, 2.f)));
            auto tmp_T = sqrtf(powf(tmp_Z, 2.f) + powf(tmp_H, 2.f));
            if (parametrs.yMax < tmp_T)
            {
                parametrs.yMax = tmp_T;
            }
            lines.H->append(x, tmp_H);
            lines.Z->append(x, tmp_Z);
            lines.T->append(x, tmp_T);
        }
        lines.H->setName("H - горизонтальная составляющая");
        lines.Z->setName("Z - вертикальная составляющая");
        lines.T->setName("T - полный вектор напряжонности");
        return true;
    }
    catch (const std::exception& e)
    {
        lastError = e.what();
        return false;
    }
}

bool Math::getModelLine(QtCharts::QLineSeries* line, const MathParametrs &parametrs) const noexcept
{
    try
    {
        line->setName("Контур пласта");
        line->append(-parametrs.b, parametrs.h * 2);
        line->append(-parametrs.b, parametrs.h);
        line->append(parametrs.b, parametrs.h);
        line->append(parametrs.b, parametrs.h * 2);
        return true;
    }
    catch (const std::exception& e)
    {
        return false;
    }
}

std::string& Math::getLastError() const
{
    return lastError;
}

Math& Math::getInstance()
{
    static Math math;
    return math;
}
