#pragma once

namespace MATH {
	class Math
	{
	public:
		inline static float Clamp(float a, float b, bool max) {
			if (max) {
				if (a < b) { return b; }
				else { return a; }
			}
			else {
				if (a < b) { return a; }
				else { return b; }
			}
		}
	};
}

