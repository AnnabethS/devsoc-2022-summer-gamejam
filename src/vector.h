#ifndef ANNA_VECTORS
#define ANNA_VECTORS

#define PI 3.14152
#define RAD2DEG 180 / PI
#define DEG2RAD PI / 180

#include <math.h>
// unforunately this library requires the code to be linked
// against the math library, due to needing `sqrt`

typedef struct{
	int x;
	int y;
}vec2;

typedef struct{
	float x;
	float y;
}vec2f;


/* returns the magnitude of a vec2f `v` as a scalar */
static float vec2fMagnitude(vec2f* v);

/* returns the distance from `v1` to `v2` as a scalar */
static float vec2fDist(vec2f* v1, vec2f* v2);

/* gets the vector from `fromVector` to `toVector` and stores it in `dest` as a vec2f */
static void vec2fDiff(vec2f* dest, vec2f* fromVector, vec2f* toVector);

/* adds `src1` and `src2`, stores the result in dest. */
static void vec2fAdd(vec2f* dest, vec2f* src1, vec2f* src2);

/* scales `src` by `scalar` and puts the result in `dest` */
static void vec2fScalarProduct(vec2f* dest, vec2f* src, float scalar);

/* get the normalised (unit vector) version of `src` and put the result in `dest` */
static void vec2fNormalise(vec2f* dest, vec2f* src);

/* clamp the magnitude of `src` to `max_magnitude` and store in `dest` */
static void vec2fClampMagnitude(vec2f* dest, vec2f* src, float max_magnitude);

/* return the angle from `fromVector` to `toVector` in radians*/
static float vec2fAngleRadians(vec2f* fromVector, vec2f* toVector);

/* return the angle from `fromVector` to `toVector` in degrees*/
static float vec2fAngleDegrees(vec2f* fromVector, vec2f* toVector);

/* get a normalised vector from an `angle` in degrees, store in `dest`*/
static void vec2fNormalisedVectorFromAngleDegrees(vec2f* dest, float angle);

/* get a normalised vector from an `angle` in radians, store in `dest`*/
static void vec2fNormalisedVectorFromAngleRadians(vec2f* dest, float angle);



static float vec2fMagnitude(vec2f* v)
{
	return sqrt(pow(v->x, 2) + pow(v->y, 2));
}

static float vec2fDist(vec2f* v1, vec2f* v2)
{
	vec2f diffVec;
	vec2fDiff(&diffVec, v1, v2);
	return vec2fMagnitude(&diffVec);
}

static void vec2fDiff(vec2f* dest, vec2f* fromVector, vec2f* toVector)
{
	dest->x = toVector->x - fromVector->x;
	dest->y = toVector->y - fromVector->y;
}

static void vec2fAdd(vec2f* dest, vec2f* src1, vec2f* src2)
{
	dest->x = src1->x + src2->x;
	dest->y = src1->y + src2->y;
}

static void vec2fScalarProduct(vec2f* dest, vec2f* src, float scalar)
{
	dest->x = src->x * scalar;
	dest->y = src->y * scalar;
}

static void vec2fNormalise(vec2f* dest, vec2f* src)
{
	float length = vec2fMagnitude(src);
	dest->x = src->x / length;
	dest->y = src->y / length;
}

static void vec2fClampMagnitude(vec2f* dest, vec2f* src, float max_magnitude)
{
    if(vec2fMagnitude(src) > max_magnitude)
    {
        vec2fNormalise(dest, src);
        vec2fScalarProduct(dest, dest, max_magnitude);
    }
}

static float vec2fAngleRadians(vec2f* fromVector, vec2f* toVector)
{
	vec2f diff;
	vec2fDiff(&diff, fromVector, toVector);
	return atan2(diff.y, diff.x);
}

static float vec2fAngleDegrees(vec2f* fromVector, vec2f* toVector)
{
	float rads = vec2fAngleRadians(fromVector, toVector);
	return rads * RAD2DEG;
}

static void vec2fNormalisedVectorFromAngleDegrees(vec2f* dest, float angle)
{
	dest->x = cos(angle * DEG2RAD);
	dest->y = sin(angle * DEG2RAD);
}

static void vec2fNormalisedVectorFromAngleRadians(vec2f* dest, float angle)
{
	dest->x = cos(angle);
	dest->y = sin(angle);
}

#endif
