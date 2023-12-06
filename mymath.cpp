#include "mymath.h"

bool Math::getLinesSeries(Lines& lines, const MathParametrs& parametrs) const noexcept
{
    try
    {
        const float J = parametrs.B * parametrs.X;
        for (int i = 0; i <= (2 * MAX_RANGE) / STEP_RANGE; i++)
        {
            float x = (STEP_RANGE * i) + -MAX_RANGE;
            auto tmp_H = J * logf((parametrs.h * parametrs.h + powf(x - parametrs.b, 2.f)) / (parametrs.h * parametrs.h + powf(x + parametrs.b, 2.f)));
            auto tmp_Z = 2 * J * atanf((2 * parametrs.b * parametrs.h)/(powf(parametrs.h, 2.f) + powf(x, 2.f) - powf(parametrs.b, 2.f)));
            lines.H->append(x, tmp_H);
            lines.Z->append(x, tmp_Z);
            lines.T->append(x, sqrtf(powf(tmp_Z, 2.f) + powf(tmp_H, 2.f)));
        }
        lines.H->setName("H");
        lines.Z->setName("Z");
        lines.T->setName("T");
        return true;
    }
    catch (const std::exception& e)
    {
        lastError = e.what();
        return false;
    }
}

Math& Math::getInstance()
{
    static Math math;
    return math;
}

Math::Math()
{

}
