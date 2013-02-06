/*
 * math.h:
 *  Math utility functions for lcommon
 */

#ifndef LCOMMON_MATH_H_
#define LCOMMON_MATH_H_

#include <cmath>

#include "geometry.h"

int power_of_two_round(int input);
float compute_direction(const Posf& p1, const Posf& p2);

const float FLOAT_PI = 3.14159265358979323846f;

#ifdef __GNUC__
//inline long int round2int(float f) {
//	return lrintf(f);
//}
//for now:
inline long int round2int(float flt) {
	return (long int)round(flt);
}
#elif (defined (WIN32) || defined (_WIN32))
__inline long int round2int (float flt) {
	int intgr;
	_asm
	{	fld flt
		fistp intgr
	};
	return intgr;
}
#endif

int squish(int a, int b, int c);

Pos centered_multiple(const Pos& pos, int mult);
Pos round_to_multiple(const Pos& pos, int mult, bool centered = false);

int round_to_multiple(int num, int mult, bool centered = false);
int centered_multiple(int num, int mult);

void direction_towards(const Pos& a, const Pos& b, float& rx, float& ry,
		float speed = 1);
float distance_between(const Pos& a, const Pos& b);
float magnitude(float x, float y);
void normalize(float& x, float& y, float mag = 1);

#endif /* LCOMMON_MATH_H_ */
