#include "Math.h"

float MATH::Math::Clamp(float a, float b, float value)
{
	float tempA = a - value, tempB = b - value;

	if (a < 0) { tempA *= -1; }
	if (b < 0) { tempB *= -1; }

	if (tempA < tempB) { return a; }
	else { return b; }
}
