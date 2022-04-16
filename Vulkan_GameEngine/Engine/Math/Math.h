#pragma once

namespace MATH {
	class Math
	{
	public:


		//TODO: Could change this function to clamp to a givin number, not just 0.
		//Return the value closest to the givin value; 
		static float Clamp(float a, float b, float value);

		inline static float Max(float a, float b) {
			if (a > b) { return a; }
			else { return b; }
		}

		inline static float Min(float a, float b) {
			if (a < b) { return a; }
			else { return b; }
		}
		
	};
}

