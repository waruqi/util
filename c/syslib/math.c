//
// Created by hujianzhe
//

#include "math.h"
#include <stddef.h>

#ifdef	__cplusplus
extern "C" {
#endif

/*
a == b return =0, a > b return >0, a < b return <0;
*/
int fcmpf(float a, float b, float epsilon) {
	float v = a - b;
	/* a == b */
	if (v > -epsilon && v < epsilon)
		return 0;
	return v >= epsilon ? 1 : -1;
}
/*
a == b return 0, a > b return >0, a < b return <0;
*/
int fcmp(double a, double b, double epsilon) {
	double v = a - b;
	/* a == b */
	if (v > -epsilon && v < epsilon)
		return 0;
	return v >= epsilon ? 1 : -1;
}

float finvsqrtf(float x) {
	float xhalf = 0.5f * x;
	int i = *(int*)&x;
	i = 0x5f3759df - (i >> 1);
	x = *(float*)&i;
	x = x * (1.5f - xhalf * x * x);
	x = x * (1.5f - xhalf * x * x);
	x = x * (1.5f - xhalf * x * x);
	return x;
}
float fsqrtf(float x) { return 1.0f / finvsqrtf(x); }

double finvsqrt(double x) {
	double xhalf = 0.5 * x;
	long long i = *(long long*)&x;
	i = 0x5fe6ec85e7de30da - (i >> 1);
	x = *(double*)&i;
	x = x * (1.5 - xhalf * x * x);
	x = x * (1.5 - xhalf * x * x);
	x = x * (1.5 - xhalf * x * x);
	return x;
}
double fsqrt(double x) { return 1.0 / finvsqrt(x); }

int mathQuadraticEquation(float a, float b, float c, float r[2]) {
	int cmp;
	float delta;
	if (fcmpf(a, 0.0f, CCT_EPSILON) == 0)
		return 0;
	delta = b * b - 4.0f * a * c;
	cmp = fcmpf(delta, 0.0f, CCT_EPSILON);
	if (cmp < 0)
		return 0;
	else if (0 == cmp) {
		r[0] = r[1] = -b / a * 0.5f;
		return 1;
	}
	else {
		float sqrt_delta = sqrtf(delta);
		r[0] = (-b + sqrt_delta) / a * 0.5f;
		r[1] = (-b - sqrt_delta) / a * 0.5f;
		return 2;
	}
}

/*
	vec3 and quat
*/

int mathVec3IsZero(const float v[3]) {
	return	fcmpf(v[0], 0.0f, CCT_EPSILON) == 0 &&
			fcmpf(v[1], 0.0f, CCT_EPSILON) == 0 &&
			fcmpf(v[2], 0.0f, CCT_EPSILON) == 0;
}

int mathVec3Equal(const float v1[3], const float v2[3]) {
	return	fcmpf(v1[0], v2[0], CCT_EPSILON) == 0 &&
			fcmpf(v1[1], v2[1], CCT_EPSILON) == 0 &&
			fcmpf(v1[2], v2[2], CCT_EPSILON) == 0;
}

/* r = v */
float* mathVec3Copy(float r[3], const float v[3]) {
	r[0] = v[0];
	r[1] = v[1];
	r[2] = v[2];
	return r;
}

float mathVec3LenSq(const float v[3]) {
	return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}

float mathVec3Len(const float v[3]) {
	return sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

float* mathVec3Normalized(float r[3], const float v[3]) {
	float len = mathVec3Len(v);
	if (fcmpf(len, 0.0f, CCT_EPSILON) > 0) {
		float inv_len = 1.0f / len;
		r[0] = v[0] * inv_len;
		r[1] = v[1] * inv_len;
		r[2] = v[2] * inv_len;
	}
	return r;
}

/* r = -v */
float* mathVec3Negate(float r[3], const float v[3]) {
	r[0] = -v[0];
	r[1] = -v[1];
	r[2] = -v[2];
	return r;
}

/* r = v1 + v2 */
float* mathVec3Add(float r[3], const float v1[3], const float v2[3]) {
	r[0] = v1[0] + v2[0];
	r[1] = v1[1] + v2[1];
	r[2] = v1[2] + v2[2];
	return r;
}

/* r += v * n */
float* mathVec3AddScalar(float r[3], const float v[3], float n) {
	r[0] += v[0] * n;
	r[1] += v[1] * n;
	r[2] += v[2] * n;
	return r;
}

/* r = v1 - v2 */
float* mathVec3Sub(float r[3], const float v1[3], const float v2[3]) {
	r[0] = v1[0] - v2[0];
	r[1] = v1[1] - v2[1];
	r[2] = v1[2] - v2[2];
	return r;
}

/* r = v * n */
float* mathVec3MultiplyScalar(float r[3], const float v[3], float n) {
	r[0] = v[0] * n;
	r[1] = v[1] * n;
	r[2] = v[2] * n;
	return r;
}

float mathVec3Dot(const float v1[3], const float v2[3]) {
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

float mathVec3Radian(const float v1[3], const float v2[3]) {
	return acosf(mathVec3Dot(v1, v2) / mathVec3Len(v1) * mathVec3Len(v2));
}

/* r = v1 X v2 */
float* mathVec3Cross(float r[3], const float v1[3], const float v2[3]) {
	float x = v1[1] * v2[2] - v1[2] * v2[1];
	float y = v1[2] * v2[0] - v1[0] * v2[2];
	float z = v1[0] * v2[1] - v1[1] * v2[0];
	r[0] = x;
	r[1] = y;
	r[2] = z;
	return r;
}

float* mathCoordinateSystemTransform(const float v[3], const float new_origin[3], float new_axies[3][3], float new_v[3]) {
	float t[3];
	if (new_origin)/* if v is normal vector, this field must be NULL */
		mathVec3Sub(t, v, new_origin);
	else
		mathVec3Copy(t, v);
	new_v[0] = mathVec3Dot(t, new_axies[0]);
	new_v[1] = mathVec3Dot(t, new_axies[1]);
	new_v[2] = mathVec3Dot(t, new_axies[2]);
	return new_v;
}

float* mathQuatNormalized(float r[4], const float q[4]) {
	float m = q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3];
	if (fcmpf(m, 0.0f, CCT_EPSILON) > 0) {
		m = 1.0f / sqrtf(m);
		r[0] = q[0] * m;
		r[1] = q[1] * m;
		r[2] = q[2] * m;
		r[3] = q[3] * m;
	}
	return r;
}

float* mathQuatFromEuler(float q[4], const float e[3], const char order[3]) {
	float pitch_x = e[0];
	float yaw_y = e[1];
	float roll_z = e[2];

	float c1 = cosf(pitch_x * 0.5f);
	float c2 = cosf(yaw_y * 0.5f);
	float c3 = cosf(roll_z * 0.5f);
	float s1 = sinf(pitch_x * 0.5f);
	float s2 = sinf(yaw_y * 0.5f);
	float s3 = sinf(roll_z * 0.5f);

	if (order[0] == 'X' && order[1] == 'Y' && order[2] == 'Z') {
		q[0] = s1 * c2 * c3 + c1 * s2 * s3;
		q[1] = c1 * s2 * c3 - s1 * c2 * s3;
		q[2] = c1 * c2 * s3 + s1 * s2 * c3;
		q[3] = c1 * c2 * c3 - s1 * s2 * s3;
	}
	else if (order[0] == 'Y' && order[1] == 'X' && order[2] == 'Z') {
		q[0] = s1 * c2 * c3 + c1 * s2 * s3;
		q[1] = c1 * s2 * c3 - s1 * c2 * s3;
		q[2] = c1 * c2 * s3 - s1 * s2 * c3;
		q[3] = c1 * c2 * c3 + s1 * s2 * s3;
	}
	else if (order[0] == 'Z' && order[1] == 'X' && order[2] == 'Y') {
		q[0] = s1 * c2 * c3 - c1 * s2 * s3;
		q[1] = c1 * s2 * c3 + s1 * c2 * s3;
		q[2] = c1 * c2 * s3 + s1 * s2 * c3;
		q[3] = c1 * c2 * c3 - s1 * s2 * s3;
	}
	else if (order[0] == 'Z' && order[1] == 'Y' && order[2] == 'X') {
		q[0] = s1 * c2 * c3 - c1 * s2 * s3;
		q[1] = c1 * s2 * c3 + s1 * c2 * s3;
		q[2] = c1 * c2 * s3 - s1 * s2 * c3;
		q[3] = c1 * c2 * c3 + s1 * s2 * s3;
	}
	else if (order[0] == 'Y' && order[1] == 'Z' && order[2] == 'X') {
		q[0] = s1 * c2 * c3 + c1 * s2 * s3;
		q[1] = c1 * s2 * c3 + s1 * c2 * s3;
		q[2] = c1 * c2 * s3 - s1 * s2 * c3;
		q[3] = c1 * c2 * c3 - s1 * s2 * s3;
	}
	else if (order[0] == 'X' && order[1] == 'Z' && order[2] == 'Y') {
		q[0] = s1 * c2 * c3 - c1 * s2 * s3;
		q[1] = c1 * s2 * c3 - s1 * c2 * s3;
		q[2] = c1 * c2 * s3 + s1 * s2 * c3;
		q[3] = c1 * c2 * c3 + s1 * s2 * s3;
	}
	else {
		q[0] = q[1] = q[2] = 0.0f;
		q[3] = 1.0f;
	}
	return q;
}

float* mathQuatFromUnitVec3(float q[4], const float from[3], const float to[3]) {
	float v[3];
	float w = mathVec3Dot(from, to) + 1.0f;
	if (w < 1E-7f) {
		float from_abs_x = from[0] > 0.0f ? from[0] : -from[0];
		float from_abs_z = from[2] > 0.0f ? from[2] : -from[2];
		if (from_abs_x > from_abs_z) {
			v[0] = -from[1];
			v[1] = from[0];
			v[2] = 0.0f;
		}
		else {
			v[0] = 0.0f;
			v[1] = -from[2];
			v[2] = from[1];
		}
		w = 0.0f;
	}
	else {
		mathVec3Cross(v, from, to);
	}

	q[0] = v[0];
	q[1] = v[1];
	q[2] = v[2];
	q[3] = w;
	return mathQuatNormalized(q, q);
}

float* mathQuatFromAxisRadian(float q[4], const float axis[3], float radian) {
	const float half_rad = radian * 0.5f;
	const float s = sinf(half_rad);
	q[0] = axis[0] * s;
	q[1] = axis[1] * s;
	q[2] = axis[2] * s;
	q[3] = cosf(half_rad);
	return q;
}

void mathQuatToAxisRadian(float q[4], float axis[3], float* radian) {
	const float qx = q[0], qy = q[1], qz = q[2], qw = q[3];
	const float s2 = qx*qx + qy*qy + qz*qz;
	const float s = 1.0f / sqrtf(s2);
	axis[0] = qx * s;
	axis[1] = qy * s;
	axis[2] = qz * s;
	*radian = atan2f(s2*s, qw) * 2.0f;
}

float* mathQuatIdentity(float q[4]) {
	q[0] = q[1] = q[2] = 0.0f;
	q[3] = 1.0f;
	return q;
}

/* r = -q */
float* mathQuatConjugate(float r[4], const float q[4]) {
	r[0] = -q[0];
	r[1] = -q[1];
	r[2] = -q[2];
	r[3] = q[3];
	return r;
}

/* r = q1 * q2 */
float* mathQuatMulQuat(float r[4], const float q1[4], const float q2[4]) {
	const float q1x = q1[0], q1y = q1[1], q1z = q1[2], q1w = q1[3];
	const float q2x = q2[0], q2y = q2[1], q2z = q2[2], q2w = q2[3];
	r[0] = q1w*q2x + q2w*q1x + q1y*q2z - q2y*q1z;
	r[1] = q1w*q2y + q2w*q1y + q1z*q2x - q2z*q1x;
	r[2] = q1w*q2z + q2w*q1z + q1x*q2y - q2x*q1y;
	r[3] = q1w*q2w - q2x*q1x - q1y*q2y - q2z*q1z;
	return r;
}

/* r = q * v */
float* mathQuatMulVec3(float r[3], const float q[4], const float v[3]) {
	const float vx = 2.0f * v[0];
	const float vy = 2.0f * v[1];
	const float vz = 2.0f * v[2];
	const float qx = q[0], qy = q[1], qz = q[2], qw = q[3];
	const float qw2 = qw*qw - 0.5f;
	const float dot2 = qx*vx + qy*vy + qz*vz;
	r[0] = vx*qw2 + (qy * vz - qz * vy)*qw + qx*dot2;
	r[1] = vy*qw2 + (qz * vx - qx * vz)*qw + qy*dot2;
	r[2] = vz*qw2 + (qx * vy - qy * vx)*qw + qz*dot2;
	return r;
}

/*
	continue collision detection
*/

static void copy_result(CCTResult_t* dst, CCTResult_t* src) {
	if (dst == src)
		return;
	dst->distance = src->distance;
	dst->hit_point_cnt = src->hit_point_cnt;
	if (1 == src->hit_point_cnt) {
		mathVec3Copy(dst->hit_point, src->hit_point);
	}
}

void mathPointProjectionLine(const float p[3], float ls[2][3], float np[3], float* distance) {
	float v0v1[3], v0p[3], pp[3];
	mathVec3Sub(v0v1, ls[1], ls[0]);
	mathVec3Sub(v0p, p, ls[0]);
	mathVec3Normalized(v0v1, v0v1);
	mathVec3Copy(pp, ls[0]);
	mathVec3AddScalar(pp, v0v1, mathVec3Dot(v0v1, v0p));
	if (np)
		mathVec3Copy(np, pp);
	if (distance) {
		float v[3];
		*distance = mathVec3Len(mathVec3Sub(v, pp, p));
	}
}

void mathPointProjectionPlane(const float p[3], const float plane_v[3], const float plane_normal[3], float np[3], float* distance) {
	float pv[3], d;
	mathVec3Sub(pv, plane_v, p);
	d = mathVec3Dot(pv, plane_normal);
	if (distance)
		*distance = d;
	if (np) {
		mathVec3Copy(np, p);
		mathVec3AddScalar(np, plane_normal, d);
	}
}

float* mathPlaneNormalByVertices3(float vertices[3][3], float normal[3]) {
	float v0v1[3], v0v2[3];
	mathVec3Sub(v0v1, vertices[1], vertices[0]);
	mathVec3Sub(v0v2, vertices[2], vertices[0]);
	mathVec3Cross(normal, v0v1, v0v2);
	return mathVec3Normalized(normal, normal);
}

int mathLineSegmentHasPoint(float ls[2][3], const float p[3]) {
	const float *v1 = ls[0], *v2 = ls[1];
	float pv1[3], pv2[3], N[3];
	mathVec3Sub(pv1, v1, p);
	mathVec3Sub(pv2, v2, p);
	if (!mathVec3IsZero(mathVec3Cross(N, pv1, pv2)))
		return 0;
	else {
		int i;
		if (mathVec3Equal(ls[0], p))
			return 1;
		if (mathVec3Equal(ls[1], p))
			return 2;
		for (i = 0; i < 3; ++i) {
			float min_v, max_v;
			v1[i] < v2[i] ? (min_v = v1[i], max_v = v2[i]) : (min_v = v2[i], max_v = v1[i]);
			if (fcmpf(p[i], min_v, CCT_EPSILON) < 0 || fcmpf(p[i], max_v, CCT_EPSILON) > 0)
				return 0;
		}
		return 3;
	}
}

int mathTriangleHasPoint(float tri[3][3], const float p[3], float* p_u, float* p_v) {
	float ap[3], ab[3], ac[3], N[3];
	mathVec3Sub(ap, p, tri[0]);
	mathVec3Sub(ab, tri[1], tri[0]);
	mathVec3Sub(ac, tri[2], tri[0]);
	mathVec3Cross(N, ab, ac);
	if (fcmpf(mathVec3Dot(N, ap), 0.0f, CCT_EPSILON))
		return 0;
	else {
		float u, v;
		float dot_ac_ac = mathVec3Dot(ac, ac);
		float dot_ac_ab = mathVec3Dot(ac, ab);
		float dot_ac_ap = mathVec3Dot(ac, ap);
		float dot_ab_ab = mathVec3Dot(ab, ab);
		float dot_ab_ap = mathVec3Dot(ab, ap);
		float tmp = 1.0f / (dot_ac_ac * dot_ab_ab - dot_ac_ab * dot_ac_ab);
		u = (dot_ab_ab * dot_ac_ap - dot_ac_ab * dot_ab_ap) * tmp;
		if (fcmpf(u, 0.0f, CCT_EPSILON) < 0 || fcmpf(u, 1.0f, CCT_EPSILON) > 0)
			return 0;
		v = (dot_ac_ac * dot_ab_ap - dot_ac_ab * dot_ac_ap) * tmp;
		if (fcmpf(v, 0.0f, CCT_EPSILON) < 0 || fcmpf(v + u, 1.0f, CCT_EPSILON) > 0)
			return 0;
		if (p_u)
			*p_u = u;
		if (p_v)
			*p_v = v;
		return 1;
	}
}

int mathCapsuleHasPoint(float cp[2][3], float radius, const float p[3]) {
	float cpdir[3], cplensq, v[3], dot;
	mathVec3Sub(cpdir, cp[1], cp[0]);
	cplensq = mathVec3LenSq(cpdir);
	mathVec3Sub(v, p, cp[0]);
	dot = mathVec3Dot(cpdir, v);
	if (fcmpf(dot, 0.0f, CCT_EPSILON) < 0) {
		return mathSphereHasPoint(cp[0], radius, p);
	}
	mathVec3Normalized(cpdir, cpdir);
	dot = mathVec3Dot(cpdir, v);
	if (fcmpf(dot * dot, cplensq, CCT_EPSILON) > 0) {
		return mathSphereHasPoint(cp[1], radius, p);
	}
	else {
		int cmp = fcmpf(mathVec3LenSq(v) - dot * dot, radius * radius, CCT_EPSILON);
		if (cmp > 0)
			return 0;
		return cmp < 0 ? 2 : 1;
	}
}

int mathSphereHasPoint(const float o[3], float radius, const float p[3]) {
	float op[3];
	int cmp = fcmpf(mathVec3LenSq(mathVec3Sub(op, p, o)), radius * radius, CCT_EPSILON);
	if (cmp > 0)
		return 0;
	if (0 == cmp)
		return 1;
	else
		return 2;
}

float* mathTriangleGetPoint(float tri[3][3], float u, float v, float p[3]) {
	float v0[3], v1[3], v2[3];
	mathVec3MultiplyScalar(v0, tri[0], 1.0f - u - v);
	mathVec3MultiplyScalar(v1, tri[1], u);
	mathVec3MultiplyScalar(v2, tri[2], v);
	return mathVec3Add(p, mathVec3Add(p, v0, v1), v2);
}

int mathLineIntersectLine(const float ls1v[3], const float ls1dir[3], const float ls2v[3], const float ls2dir[3], float distance[2]) {
	float N[3], v[3], dot;
	mathVec3Cross(N, ls1dir, ls2dir);
	if (mathVec3IsZero(N)) {
		mathVec3Sub(v, ls1v, ls2v);
		dot = mathVec3Dot(v, ls2dir);
		return fcmpf(dot * dot, mathVec3LenSq(v), CCT_EPSILON) ? 0 : -1;
	}
	else {
		mathVec3Sub(v, ls1v, ls2v);
		if (mathVec3IsZero(v)) {
			distance[0] = distance[1] = 0.0f;
			return 1;
		}
		dot = mathVec3Dot(v, N);
		if (fcmpf(dot, 0.0f, CCT_EPSILON))
			return 0;
		dot = mathVec3Dot(v, ls2dir);
		mathVec3AddScalar(mathVec3Copy(v, ls2v), ls2dir, dot);
		mathVec3Sub(v, v, ls1v);
		if (mathVec3IsZero(v)) {
			distance[0] = 0.0f;
		}
		else {
			distance[0] = mathVec3Len(v);
			distance[0] /= mathVec3Dot(mathVec3Normalized(v, v), ls1dir);
		}
		mathVec3Sub(v, ls2v, ls1v);
		dot = mathVec3Dot(v, ls1dir);
		mathVec3AddScalar(mathVec3Copy(v, ls1v), ls1dir, dot);
		mathVec3Sub(v, v, ls2v);
		if (mathVec3IsZero(v)) {
			distance[1] = 0.0f;
		}
		else {
			distance[1] = mathVec3Len(v);
			distance[1] /= mathVec3Dot(mathVec3Normalized(v, v), ls2dir);
		}
		return 1;
	}
}

static int overlapSegmentIntersectSegment(float ls1[2][3], float ls2[2][3], float p[3]) {
	int res = mathLineSegmentHasPoint(ls1, ls2[0]);
	if (3 == res)
		return 2;
	else if (res) {
		if (mathLineSegmentHasPoint(ls1, ls2[1]))
			return 2;
		if (mathLineSegmentHasPoint(ls2, ls1[res == 1 ? 1 : 0]))
			return 2;
		mathVec3Copy(p, ls1[res - 1]);
		return 1;
	}
	res = mathLineSegmentHasPoint(ls1, ls2[1]);
	if (3 == res)
		return 2;
	else if (res) {
		if (mathLineSegmentHasPoint(ls2, ls1[res == 1 ? 1 : 0]))
			return 2;
		mathVec3Copy(p, ls1[res - 1]);
		return 1;
	}

	if (mathLineSegmentHasPoint(ls2, ls1[0]) == 3 ||
		mathLineSegmentHasPoint(ls2, ls1[1]) == 3)
	{
		return 2;
	}
	return 0;
}

int mathSegmentIntersectSegment(float ls1[2][3], float ls2[2][3], float p[3]) {
	int res;
	float lsdir1[3], lsdir2[3], d[2], lslensq1, lslensq2;
	mathVec3Sub(lsdir1, ls1[1], ls1[0]);
	mathVec3Sub(lsdir2, ls2[1], ls2[0]);
	lslensq1 = mathVec3LenSq(lsdir1);
	lslensq2 = mathVec3LenSq(lsdir2);
	mathVec3Normalized(lsdir1, lsdir1);
	mathVec3Normalized(lsdir2, lsdir2);
	res = mathLineIntersectLine(ls1[0], lsdir1, ls2[0], lsdir2, d);
	if (0 == res)
		return 0;
	if (res > 0) {
		if (fcmpf(d[0], 0.0f, CCT_EPSILON) < 0 || fcmpf(d[1], 0.0f, CCT_EPSILON) < 0)
			return 0;
		if (fcmpf(d[0] * d[0], lslensq1, CCT_EPSILON) > 0 || fcmpf(d[1] * d[1], lslensq2, CCT_EPSILON) > 0)
			return 0;
		mathVec3AddScalar(mathVec3Copy(p, ls1[0]), lsdir1, d[0]);
		return 1;
	}
	else {
		return overlapSegmentIntersectSegment(ls1, ls2, p);
	}
}

int mathLineIntersectPlane(const float ls_v[3], const float lsdir[3], const float plane_v[3], const float plane_normal[3], float* distance) {
	float cos_theta = mathVec3Dot(lsdir, plane_normal);
	mathPointProjectionPlane(ls_v, plane_v, plane_normal, NULL, distance);
	if (fcmpf(cos_theta, 0.0f, CCT_EPSILON)) {
		*distance /= cos_theta;
		return 1;
	}
	else {
		return fcmpf(*distance, 0.0f, CCT_EPSILON) ? 0 : -1;
	}
}

int mathSphereIntersectLine(const float o[3], float radius, const float ls_vertice[3], const float lsdir[3], float distance[2]) {
	int cmp;
	float vo[3], lp[3], lpo[3], lpolensq, radiussq, dot;
	mathVec3Sub(vo, o, ls_vertice);
	dot = mathVec3Dot(vo, lsdir);
	mathVec3AddScalar(mathVec3Copy(lp, ls_vertice), lsdir, dot);
	mathVec3Sub(lpo, o, lp);
	lpolensq = mathVec3LenSq(lpo);
	radiussq = radius * radius;
	cmp = fcmpf(lpolensq, radiussq, CCT_EPSILON);
	if (cmp > 0)
		return 0;
	else if (0 == cmp) {
		distance[0] = distance[1] = dot;
		return 1;
	}
	else {
		float d = sqrtf(radiussq - lpolensq);
		distance[0] = dot + d;
		distance[1] = dot - d;
		return 2;
	}
}

int mathSphereIntersectSegment(const float o[3], float radius, float ls[2][3], float p[3]) {
	int c[2];
	c[0] = mathSphereHasPoint(o, radius, ls[0]);
	c[1] = mathSphereHasPoint(o, radius, ls[1]);
	if (c[0] + c[1] >= 2)
		return 2;
	else {
		float pl[3], plo[3];
		mathPointProjectionLine(o, ls, pl, NULL);
		if (!mathLineSegmentHasPoint(ls, pl)) {
			if (c[0] && p)
				mathVec3Copy(p, ls[0]);
			else if (c[1] && p)
				mathVec3Copy(p, ls[1]);
			return c[0] + c[1];
		}
		mathVec3Sub(plo, o, pl);
		c[0] = fcmpf(mathVec3LenSq(plo), radius * radius, CCT_EPSILON);
		if (c[0] < 0)
			return 2;
		if (0 == c[0]) {
			if (p)
				mathVec3Copy(p, pl);
			return 1;
		}
		return 0;
	}
}

int mathSphereIntersectPlane(const float o[3], float radius, const float plane_v[3], const float plane_normal[3], float new_o[3], float* new_r) {
	int cmp;
	float pp[3], ppd, ppo[3];
	mathPointProjectionPlane(o, plane_v, plane_normal, pp, &ppd);
	mathVec3Sub(ppo, o, pp);
	cmp = fcmpf(mathVec3LenSq(ppo), radius * radius, CCT_EPSILON);
	if (cmp > 0)
		return 0;
	if (0 == cmp) {
		mathVec3Copy(new_o, pp);
		*new_r = 0.0f;
		return 1;
	}
	else {
		mathVec3Copy(new_o, pp);
		*new_r = sqrtf(radius * radius - mathVec3LenSq(ppo));
		return 2;
	}
}

int mathSphereIntersectSphere(const float o1[3], float r1, const float o2[3], float r2, float p[3]) {
	int cmp;
	float o1o2[3], radius_sum = r1 + r2;
	mathVec3Sub(o1o2, o2, o1);
	cmp = fcmpf(mathVec3LenSq(o1o2), radius_sum * radius_sum, CCT_EPSILON);
	if (cmp > 0)
		return 0;
	if (cmp < 0)
		return 2;
	else {
		mathVec3Normalized(o1o2, o1o2);
		mathVec3AddScalar(mathVec3Copy(p, o1), o1o2, r1);
		return 1;
	}
}

int mathSphereIntersectCapsule(const float sphere_o[3], float sphere_radius, float cp[2][3], float cp_radius, float p[3]) {
	float cpdir[3], cplensq, v[3], dot;
	mathVec3Sub(cpdir, cp[1], cp[0]);
	cplensq = mathVec3LenSq(cpdir);
	mathVec3Sub(v, sphere_o, cp[0]);
	dot = mathVec3Dot(cpdir, v);
	if (fcmpf(dot, 0.0f, CCT_EPSILON) < 0)
		return mathSphereIntersectSphere(sphere_o, sphere_radius, cp[0], cp_radius, p);
	mathVec3Normalized(cpdir, cpdir);
	dot = mathVec3Dot(cpdir, v);
	if (fcmpf(dot * dot, cplensq, CCT_EPSILON) > 0)
		return mathSphereIntersectSphere(sphere_o, sphere_radius, cp[1], cp_radius, p);
	else {
		float radius_sum = sphere_radius + cp_radius;
		int cmp = fcmpf(mathVec3LenSq(v) - dot * dot, radius_sum * radius_sum, CCT_EPSILON);
		if (cmp > 0)
			return 0;
		if (cmp < 0)
			return 2;
		else {
			float pl[3];
			mathVec3AddScalar(mathVec3Copy(pl, cp[0]), cpdir, dot);
			mathVec3Sub(v, pl, sphere_o);
			mathVec3Normalized(v, v);
			mathVec3AddScalar(mathVec3Copy(p, sphere_o), v, sphere_radius);
			return 1;
		}
	}
}

int mathAABBIntersectAABB(const float o1[3], const float half1[3], const float o2[3], const float half2[3]) {
	/*
	!(o2[0] - half2[0] > o1[0] + half1[0] || o1[0] - half1[0] > o2[0] + half2[0] ||
	o2[1] - half2[1] > o1[1] + half1[1] || o1[1] - half1[1] > o2[1] + half2[1] ||
	o2[2] - half2[2] > o1[2] + half1[2] || o1[2] - half1[2] > o2[2] + half2[2]);
	*/

	return !(o2[0] - o1[0] > half1[0] + half2[0] || o1[0] - o2[0] > half1[0] + half2[0] ||
		o2[1] - o1[1] > half1[1] + half2[1] || o1[1] - o2[1] > half1[1] + half2[1] ||
		o2[2] - o1[2] > half1[2] + half2[2] || o1[2] - o2[2] > half1[2] + half2[2]);
}

int mathCylinderInfiniteIntersectLine(float cp[2][3], float radius, const float ls_v[3], const float lsdir[3], float distance[2]) {
	const float *p0 = cp[0], *p1 = cp[1];
	float new_o[3], new_dir[3], p0p1len;
	float new_axies[3][3], z_axies_normal[3] = { 0.0f, 0.0f, 1.0f };
	float A, B, C;
	int rcnt;
	mathVec3Sub(new_axies[2], p1, p0);
	p0p1len = mathVec3Len(new_axies[2]);
	mathVec3Normalized(new_axies[2], new_axies[2]);
	new_axies[1][0] = 0.0f;
	new_axies[1][1] = new_axies[2][2];
	new_axies[1][2] = new_axies[2][1];
	mathVec3Cross(new_axies[0], new_axies[1], new_axies[2]);
	mathVec3Normalized(new_axies[0], new_axies[0]);
	mathCoordinateSystemTransform(ls_v, p0, new_axies, new_o);
	mathCoordinateSystemTransform(lsdir, NULL, new_axies, new_dir);
	A = new_dir[0] * new_dir[0] + new_dir[1] * new_dir[1];
	B = 2.0f * (new_o[0] * new_dir[0] + new_o[1] * new_dir[1]);
	C = new_o[0] * new_o[0] + new_o[1] * new_o[1] - radius * radius;
	rcnt = mathQuadraticEquation(A, B, C, distance);
	if (0 == rcnt) {
		float plp[3], plpp[3];
		mathPointProjectionLine(ls_v, cp, plp, NULL);
		mathVec3Sub(plpp, ls_v, plp);
		return fcmpf(mathVec3LenSq(plpp), radius * radius, CCT_EPSILON) > 0 ? 0 : -1;
	}
	return rcnt;
}

int mathCylinderInfiniteIntersectPlane(float cp[2][3], float radius, const float plane_vertice[3], const float plane_normal[3], float res_data[4][3]) {
	float axis[3], cos_theta;
	mathVec3Sub(axis, cp[1], cp[0]);
	cos_theta = mathVec3Dot(axis, plane_normal);
	if (fcmpf(cos_theta, 0.0f, CCT_EPSILON) == 0) {
		float d;
		mathPointProjectionPlane(cp[0], plane_vertice, plane_normal, NULL, &d);
		return fcmpf(d < 0.0f ? -d : d, radius, CCT_EPSILON) > 0 ? 0 : -1;
	}
	else {
		float center[3], v1[3], d;
		mathVec3Normalized(axis, axis);
		cos_theta = mathVec3Dot(axis, plane_normal);
		mathPointProjectionPlane(cp[0], plane_vertice, plane_normal, NULL, &d);
		d /= cos_theta;
		mathVec3AddScalar(mathVec3Copy(center, cp[0]), axis, d);
		mathVec3Cross(v1, plane_normal, axis);
		if (mathVec3IsZero(v1)) {
			mathVec3Copy(res_data[0], center);
			mathVec3Copy(res_data[1], axis);
			return 1;
		}
		else {
			float v2[3];
			mathVec3Cross(v2, v1, plane_normal);
			mathVec3Normalized(v1, v1);
			mathVec3AddScalar(mathVec3Copy(res_data[0], center), v1, radius);
			mathVec3AddScalar(mathVec3Copy(res_data[1], center), v1, -radius);
			mathVec3Normalized(v2, v2);
			d = radius / cos_theta;
			mathVec3AddScalar(mathVec3Copy(res_data[2], center), v2, d);
			mathVec3AddScalar(mathVec3Copy(res_data[3], center), v2, -d);
			return 2;
		}
	}
}

/*
int mathCylinderIntersectLine(float cp[2][3], float radius, float ls_vertice[3], float lsdir[3], float p[2][3]) {
	int res = mathCylinderInfiniteIntersectLine(cp, radius, ls_vertice, lsdir, p);
	if (0 == res)
		return 0;
	else if (1 == res)
		return 1;
	else if (2 == res) {
		int c[2];
		c[0] = mathCylinderHasPoint(cp, radius, p[0]);
		c[1] = mathCylinderHasPoint(cp, radius, p[1]);
		if (c[0] && c[1])
			return 2;
		else if (c[0] || c[1]) {
			float axis[3], d[2], *other_p, min_d, cos_theta;
			mathVec3Sub(axis, cp[1], cp[0]);
			mathVec3Normalized(axis, axis);
			other_p = c[0] ? p[1] : p[0];
			mathPointProjectionPlane(other_p, cp[0], axis, NULL, &d[0]);
			mathPointProjectionPlane(other_p, cp[1], axis, NULL, &d[1]);
			if (d[0] <= -CCT_EPSILON)
				min_d = d[0] < d[1] ? d[1] : d[0];
			else
				min_d = d[0] < d[1] ? d[0] : d[1];
			cos_theta = mathVec3Dot(axis, lsdir);
			min_d /= cos_theta;
			mathVec3AddScalar(other_p, lsdir, min_d);
			return mathVec3Equal(p[0], p[1]) ? 1 : 2;
		}
		else
			return 0;
	}
	else {
		float axis[3];
		mathVec3Sub(axis, cp[1], cp[0]);
		mathVec3Normalized(axis, axis);
		mathPointProjectionPlane(ls_vertice, cp[0], axis, p[0], NULL);
		mathPointProjectionPlane(ls_vertice, cp[1], axis, p[1], NULL);
		return 2;
	}
}
*/

CCTResult_t* mathRaycastLineSegment(float o[3], float dir[3], float ls[2][3], CCTResult_t* result) {
	if (mathLineSegmentHasPoint(ls, o)) {
		result->distance = 0.0f;
		result->hit_point_cnt = 1;
		mathVec3Copy(result->hit_point, o);
		return result;
	}
	else {
		float pl[3], pld;
		mathPointProjectionLine(o, ls, pl, &pld);
		if (fcmpf(pld, 0.0f, CCT_EPSILON)) {
			float opl[3], cos_theta, d;
			mathVec3Sub(opl, pl, o);
			d = mathVec3Len(opl);
			mathVec3Normalized(opl, opl);
			cos_theta = mathVec3Dot(opl, dir);
			if (fcmpf(cos_theta, 0.0f, CCT_EPSILON) <= 0)
				return NULL;
			d /= cos_theta;
			result->distance = d;
			result->hit_point_cnt = 1;
			mathVec3AddScalar(mathVec3Copy(result->hit_point, o), dir, d);
			return mathLineSegmentHasPoint(ls, result->hit_point) ? result : NULL;
		}
		else {
			float ols[3], d[2];
			mathVec3Sub(ols, ls[0], o);
			d[0] = mathVec3Dot(ols, dir);
			if (fcmpf(d[0], 0.0f, CCT_EPSILON) < 0 || fcmpf(d[0] * d[0], mathVec3LenSq(ols), CCT_EPSILON))
				return NULL;
			mathVec3Sub(ols, ls[1], o);
			d[1] = mathVec3Dot(ols, dir);
			if (d[0] < d[1]) {
				result->distance = d[0];
				mathVec3Copy(result->hit_point, ls[0]);
			}
			else {
				result->distance = d[1];
				mathVec3Copy(result->hit_point, ls[1]);
			}
			result->hit_point_cnt = 1;
			return result;
		}
	}
}

CCTResult_t* mathRaycastPlane(float o[3], float dir[3], float vertice[3], float normal[3], CCTResult_t* result) {
	float d, cos_theta;
	mathPointProjectionPlane(o, vertice, normal, NULL, &d);
	if (fcmpf(d, 0.0f, CCT_EPSILON) == 0) {
		result->distance = 0.0f;
		result->hit_point_cnt = 1;
		mathVec3Copy(result->hit_point, o);
		return result;
	}
	cos_theta = mathVec3Dot(dir, normal);
	if (fcmpf(cos_theta, 0.0f, CCT_EPSILON) == 0)
		return NULL;
	d /= cos_theta;
	if (fcmpf(d, 0.0f, CCT_EPSILON) < 0)
		return NULL;
	result->distance = d;
	result->hit_point_cnt = 1;
	mathVec3Copy(result->hit_point, o);
	mathVec3AddScalar(result->hit_point, dir, d);
	return result;
}

CCTResult_t* mathRaycastTriangle(float o[3], float dir[3], float tri[3][3], CCTResult_t* result) {
	float N[3];
	if (mathRaycastPlane(o, dir, tri[0], mathPlaneNormalByVertices3(tri, N), result)) {
		if (mathTriangleHasPoint(tri, result->hit_point, NULL, NULL))
			return result;
		else if (fcmpf(result->distance, 0.0f, CCT_EPSILON) == 0) {
			CCTResult_t results[3], *p_results = NULL;
			int i;
			for (i = 0; i < 3; ++i) {
				float edge[2][3];
				mathVec3Copy(edge[0], tri[i % 3]);
				mathVec3Copy(edge[1], tri[(i + 1) % 3]);
				if (!mathRaycastLineSegment(o, dir, edge, results + i))
					continue;
				if (!p_results || p_results->distance > results[i].distance) {
					p_results = results + i;
				}
			}
			if (p_results) {
				copy_result(result, p_results);
				return result;
			}
		}
	}
	return NULL;
}

CCTResult_t* mathRaycastSphere(float o[3], float dir[3], float center[3], float radius, CCTResult_t* result) {
	float radius2 = radius * radius;
	float d, dr2, oc2, dir_d;
	float oc[3];
	mathVec3Sub(oc, center, o);
	oc2 = mathVec3LenSq(oc);
	dir_d = mathVec3Dot(dir, oc);
	if (fcmpf(oc2, radius2, CCT_EPSILON) <= 0) {
		result->distance = 0.0f;
		result->hit_point_cnt = 1;
		mathVec3Copy(result->hit_point, o);
		return result;
	}
	else if (fcmpf(dir_d, 0.0f, CCT_EPSILON) <= 0)
		return NULL;

	dr2 = oc2 - dir_d * dir_d;
	if (fcmpf(dr2, radius2, CCT_EPSILON) > 0)
		return NULL;

	d = sqrtf(radius2 - dr2);
	result->distance = dir_d - d;
	result->hit_point_cnt = 1;
	mathVec3Copy(result->hit_point, o);
	mathVec3AddScalar(result->hit_point, dir, result->distance);
	return result;
}

/*
CCTResult_t* mathRaycastCylinder(float o[3], float dir[3], float cp[2][3], float radius, CCTResult_t* result) {
	float *p0 = cp[0], *p1 = cp[1];
	float new_axies[3][3], p0p1len, dot;
	float new_o[3], z_axies_normal[3] = { 0.0f, 0.0f, 1.0f };
	mathVec3Sub(new_axies[2], p1, p0);
	p0p1len = mathVec3Len(new_axies[2]);
	mathVec3Normalized(new_axies[2], new_axies[2]);
	new_axies[1][0] = 0.0f;
	new_axies[1][1] = new_axies[2][2];
	new_axies[1][2] = new_axies[2][1];
	mathVec3Cross(new_axies[0], new_axies[1], new_axies[2]);
	mathVec3Normalized(new_axies[0], new_axies[0]);
	mathCoordinateSystemTransform(o, p0, new_axies, new_o);
	dot = mathVec3Dot(new_o, z_axies_normal);
	if (new_o[2] > -CCT_EPSILON && new_o[2] < p0p1len + CCT_EPSILON &&
		fcmpf(mathVec3LenSq(new_o) - dot * dot, radius * radius, CCT_EPSILON) <= 0)
	{
		result->distance = 0.0f;
		result->hit_point_cnt = 1;
		mathVec3Copy(result->hit_point, o);
		return result;
	}
	else {
		int rcnt;
		CCTResult_t results[2], *p_result;
		float new_dir[3], new_p[3];
		float A, B, C, r[2];
		mathCoordinateSystemTransform(dir, NULL, new_axies, new_dir);
		A = new_dir[0] * new_dir[0] + new_dir[1] * new_dir[1];
		B = 2.0f * (new_o[0] * new_dir[0] + new_o[1] * new_dir[1]);
		C = new_o[0] * new_o[0] + new_o[1] * new_o[1] - radius * radius;
		rcnt = mathQuadraticEquation(A, B, C, r);
		if (2 == rcnt) {
			int has_zero = 0;
			float z;
			if (fcmpf(r[0], 0.0f, CCT_EPSILON) < 0)
				--rcnt;
			else {
				z = new_o[2] + new_dir[2] * r[0];
				if (z <= -CCT_EPSILON || z >= p0p1len + CCT_EPSILON)
					--rcnt;
			}

			if (fcmpf(r[1], 0.0f, CCT_EPSILON) < 0)
				--rcnt;
			else {
				z = new_o[2] + new_dir[2] * r[1];
				if (z <= -CCT_EPSILON || z >= p0p1len + CCT_EPSILON)
					--rcnt;
			}

			if (0 == rcnt)
				return NULL;
			if (2 == rcnt) {
				float t = r[0] < r[1] ? r[0] : r[1];
				result->distance = t;
				result->hit_point_cnt = 1;
				mathVec3Copy(result->hit_point, o);
				mathVec3AddScalar(result->hit_point, dir, t);
				return result;
			}
		}
		else if (1 == rcnt) {
			float z = new_o[2] + new_dir[2] * r[0];
			if (z > -CCT_EPSILON && z < p0p1len + CCT_EPSILON) {
				result->distance = r[0];
				result->hit_point_cnt = 1;
				mathVec3Copy(result->hit_point, o);
				mathVec3AddScalar(result->hit_point, dir, r[0]);
				return result;
			}
		}
		new_p[0] = 0.0f;
		new_p[1] = 0.0f;
		new_p[2] = p0p1len;
		p_result = NULL;
		if (mathRaycastCircle(new_o, new_dir, new_p, radius, z_axies_normal, &results[0]) &&
			(!p_result || p_result->distance > results[0].distance))
		{
			p_result = &results[0];
		}
		new_p[2] = 0.0f;
		if (mathRaycastCircle(new_o, new_dir, new_p, radius, z_axies_normal, &results[1]) &&
			(!p_result || p_result->distance > results[1].distance))
		{
			p_result = &results[1];
		}
		if (p_result) {
			copy_result(result, p_result);
			mathVec3Copy(result->hit_point, o);
			mathVec3AddScalar(result->hit_point, dir, result->distance);
			return result;
		}
		return NULL;
	}
}
*/

CCTResult_t* mathLineSegmentcastPlane(float ls[2][3], float dir[3], float vertice[3], float normal[3], CCTResult_t* result) {
	int cmp[2];
	float d[2];
	mathPointProjectionPlane(ls[0], vertice, normal, NULL, &d[0]);
	mathPointProjectionPlane(ls[1], vertice, normal, NULL, &d[1]);
	cmp[0] = fcmpf(d[0], 0.0f, CCT_EPSILON);
	cmp[1] = fcmpf(d[1], 0.0f, CCT_EPSILON);
	if (0 == cmp[0] || 0 == cmp[1]) {
		result->distance = 0.0f;
		if (cmp[0] + cmp[1]) {
			result->hit_point_cnt = 1;
			mathVec3Copy(result->hit_point, cmp[0] ? ls[1] : ls[0]);
		}
		else {
			result->hit_point_cnt = -1;
		}
		return result;
	}
	else if (cmp[0] * cmp[1] < 0) {
		float ldir[3], cos_theta, ddir;
		mathVec3Sub(ldir, ls[1], ls[0]);
		mathVec3Normalized(ldir, ldir);
		cos_theta = mathVec3Dot(normal, ldir);
		ddir = d[0] / cos_theta;
		result->distance = 0.0f;
		result->hit_point_cnt = 1;
		mathVec3Copy(result->hit_point, ls[0]);
		mathVec3AddScalar(result->hit_point, ldir, ddir);
		return result;
	}
	else {
		float min_d, *p;
		float cos_theta = mathVec3Dot(normal, dir);
		if (fcmpf(cos_theta, 0.0f, CCT_EPSILON) == 0)
			return NULL;
		else if (fcmpf(d[0], d[1], CCT_EPSILON) == 0) {
			min_d = d[0];
			result->hit_point_cnt = -1;
		}
		else {
			result->hit_point_cnt = 1;
			if (cmp[0] > 0) {
				if (d[0] < d[1]) {
					min_d = d[0];
					p = ls[0];
				}
				else {
					min_d = d[1];
					p = ls[1];
				}
			}
			else {
				if (d[0] < d[1]) {
					min_d = d[1];
					p = ls[1];
				}
				else {
					min_d = d[0];
					p = ls[0];
				}
			}
		}
		min_d /= cos_theta;
		if (fcmpf(min_d, 0.0f, CCT_EPSILON) < 0)
			return NULL;
		result->distance = min_d;
		if (1 == result->hit_point_cnt) {
			mathVec3Copy(result->hit_point, p);
			mathVec3AddScalar(result->hit_point, dir, result->distance);
		}
		return result;
	}
}

CCTResult_t* mathLineSegmentcastLineSegment(float ls1[2][3], float dir[3], float ls2[2][3], CCTResult_t* result) {
	float N[3], lsdir1[3];
	mathVec3Sub(lsdir1, ls1[1], ls1[0]);
	mathVec3Cross(N, lsdir1, dir);
	if (mathVec3IsZero(N)) {
		int c0, c1;
		CCTResult_t results[2], *p_result;
		c0 = (mathRaycastLineSegment(ls1[0], dir, ls2, &results[0]) != NULL);
		c1 = (mathRaycastLineSegment(ls1[1], dir, ls2, &results[1]) != NULL);
		if (!c0 && !c1)
			return NULL;
		else if (c0 && c1) {
			p_result = results[0].distance < results[1].distance ? &results[0] : &results[1];
			copy_result(result, p_result);
		}
		else {
			result->distance = 0.0f;
			result->hit_point_cnt = -1;
		}
		return result;
	}
	else {
		float neg_dir[3];
		if (!mathLineSegmentcastPlane(ls2, dir, ls1[0], N, result))
			return NULL;
		mathVec3Negate(neg_dir, dir);
		if (result->hit_point_cnt < 0) {
			int is_parallel, dot;
			float lsdir2[3], test_n[3];
			CCTResult_t results[4], *p_result = NULL;

			mathVec3Sub(lsdir2, ls2[1], ls2[0]);
			mathVec3Cross(test_n, lsdir1, lsdir2);
			is_parallel = mathVec3IsZero(test_n);

			if (is_parallel) {
				dot = mathVec3Dot(lsdir1, lsdir2);
			}
			else {
				float p[3], op[3], d, cos_theta;
				mathPointProjectionLine(ls1[0], ls2, p, &d);
				if (fcmpf(d, 0.0f, CCT_EPSILON) == 0) {
					result->distance = 0.0f;
					result->hit_point_cnt = 1;
					mathVec3Copy(result->hit_point, ls1[0]);
					return result;
				}
				mathVec3Normalized(op, mathVec3Sub(op, p, ls1[0]));
				mathVec3Normalized(lsdir1, lsdir1);
				cos_theta = mathVec3Dot(lsdir1, op);
				if (fcmpf(cos_theta, 0.0f, CCT_EPSILON) < 0) {
					cos_theta = mathVec3Dot(mathVec3Negate(lsdir1, lsdir1), op);
				}
				d /= cos_theta;
				mathVec3Copy(p, ls1[0]);
				mathVec3AddScalar(p, lsdir1, d);
				if (mathLineSegmentHasPoint(ls1, p) && mathLineSegmentHasPoint(ls2, p)) {
					result->distance = 0.0f;
					result->hit_point_cnt = 1;
					mathVec3Copy(result->hit_point, p);
					return result;
				}
			}

			do {
				int c0 = 0, c1 = 0;
				if (mathRaycastLineSegment(ls1[0], dir, ls2, &results[0])) {
					c0 = 1;
					if (!p_result)
						p_result = &results[0];
					if (is_parallel) {
						if (!mathVec3Equal(results[0].hit_point, ls2[0]) && !mathVec3Equal(results[0].hit_point, ls2[1]))
							p_result->hit_point_cnt = -1;
						else if (mathVec3Equal(results[0].hit_point, ls2[0]) && dot > 0)
							p_result->hit_point_cnt = -1;
						else if (mathVec3Equal(results[0].hit_point, ls2[1]) && dot < 0)
							p_result->hit_point_cnt = -1;
						break;
					}
				}
				if (mathRaycastLineSegment(ls1[1], dir, ls2, &results[1])) {
					c1 = 1;
					if (!p_result || p_result->distance > results[1].distance)
						p_result = &results[1];
					if (is_parallel) {
						if (!(mathVec3Equal(results[1].hit_point, ls2[0]) && dot > 0) &&
							!(mathVec3Equal(results[1].hit_point, ls2[1]) && dot < 0))
						{
							p_result->hit_point_cnt = -1;
						}
						break;
					}
				}
				if (c0 && c1)
					break;
				if (mathRaycastLineSegment(ls2[0], neg_dir, ls1, &results[2])) {
					if (!p_result || p_result->distance > results[2].distance) {
						p_result = &results[2];
						mathVec3Copy(p_result->hit_point, ls2[0]);
					}
					if (is_parallel) {
						p_result->hit_point_cnt = -1;
						break;
					}
				}
				if (mathRaycastLineSegment(ls2[1], neg_dir, ls1, &results[3])) {
					if (!p_result || p_result->distance > results[3].distance) {
						p_result = &results[3];
						mathVec3Copy(p_result->hit_point, ls2[1]);
					}
					if (is_parallel) {
						p_result->hit_point_cnt = -1;
						break;
					}
				}
			} while (0);
			if (p_result) {
				copy_result(result, p_result);
				return result;
			}
			return NULL;
		}
		else {
			float hit_point[3];
			mathVec3Copy(hit_point, result->hit_point);
			if (!mathRaycastLineSegment(hit_point, neg_dir, ls1, result))
				return NULL;
			mathVec3Copy(result->hit_point, hit_point);
			return result;
		}
	}
}

CCTResult_t* mathLineSegmentcastTriangle(float ls[2][3], float dir[3], float tri[3][3], CCTResult_t* result) {
	int i;
	CCTResult_t results[3], *p_result = NULL;
	float N[3];
	mathPlaneNormalByVertices3(tri, N);
	if (!mathLineSegmentcastPlane(ls, dir, tri[0], N, result))
		return NULL;
	else if (result->hit_point_cnt < 0) {
		int c[2];
		for (i = 0; i < 2; ++i) {
			float test_p[3];
			mathVec3Copy(test_p, ls[i]);
			mathVec3AddScalar(test_p, dir, result->distance);
			c[i] = mathTriangleHasPoint(tri, test_p, NULL, NULL);
		}
		if (c[0] && c[1])
			return result;
	}
	else if (mathTriangleHasPoint(tri, result->hit_point, NULL, NULL))
		return result;

	for (i = 0; i < 3; ++i) {
		float edge[2][3];
		mathVec3Copy(edge[0], tri[i % 3]);
		mathVec3Copy(edge[1], tri[(i + 1) % 3]);
		if (!mathLineSegmentcastLineSegment(ls, dir, edge, &results[i]))
			continue;
		if (!p_result)
			p_result = &results[i];
		else {
			int cmp = fcmpf(p_result->distance, results[i].distance, CCT_EPSILON);
			if (0 == cmp) {
				if (results[i].hit_point_cnt < 0 ||
					p_result->hit_point_cnt < 0 ||
					!mathVec3Equal(p_result->hit_point, results[i].hit_point))
				{
					p_result->hit_point_cnt = -1;
				}
				break;
			}
			else if (cmp > 0)
				p_result = &results[i];
		}
	}
	if (p_result) {
		copy_result(result, p_result);
		return result;
	}
	return NULL;
}

CCTResult_t* mathLineSegmentcastSphere(float ls[2][3], float dir[3], float center[3], float radius, CCTResult_t* result) {
	int c = mathSphereIntersectSegment(center, radius, ls, result->hit_point);
	if (1 == c) {
		result->distance = 0.0f;
		result->hit_point_cnt = 1;
		return result;
	}
	else if (2 == c) {
		result->distance = 0.0f;
		result->hit_point_cnt = -1;
		return result;
	}
	else {
		float lsdir[3], N[3];
		mathVec3Sub(lsdir, ls[1], ls[0]);
		mathVec3Cross(N, lsdir, dir);
		if (mathVec3IsZero(N)) {
			CCTResult_t results[2], *p_result;
			if (!mathRaycastSphere(ls[0], dir, center, radius, &results[0]))
				return NULL;
			if (!mathRaycastSphere(ls[1], dir, center, radius, &results[1]))
				return NULL;
			p_result = results[0].distance < results[1].distance ? &results[0] : &results[1];
			copy_result(result, p_result);
			return result;
		}
		else {
			CCTResult_t results[2], *p_result;
			int i;
			float np[3], lp[3], lpnp[3], lpnplensq, npd, delta_lensq;
			mathVec3Normalized(N, N);
			mathPointProjectionPlane(center, ls[0], N, np, &npd);
			if (fcmpf(npd * npd, radius * radius, CCT_EPSILON) > 0)
				return NULL;
			mathPointProjectionLine(np, ls, lp, NULL);
			mathVec3Sub(lpnp, np, lp);
			lpnplensq = mathVec3LenSq(lpnp);
			delta_lensq = radius * radius - npd * npd;
			i = fcmpf(lpnplensq, delta_lensq, CCT_EPSILON);
			if (i >= 0) {
				float p[3], new_ls[2][3], d;
				if (i > 0) {
					float cos_theta = mathVec3Dot(lpnp, dir);
					if (fcmpf(cos_theta, 0.0f, CCT_EPSILON) <= 0)
						return NULL;
					cos_theta = mathVec3Dot(mathVec3Normalized(lpnp, lpnp), dir);
					d = sqrtf(lpnplensq) - sqrtf(delta_lensq);
					mathVec3AddScalar(mathVec3Copy(p, lp), lpnp, d);
					d /= cos_theta;
					mathVec3AddScalar(mathVec3Copy(new_ls[0], ls[0]), dir, d);
					mathVec3AddScalar(mathVec3Copy(new_ls[1], ls[1]), dir, d);
				}
				else {
					d = 0.0f;
					mathVec3Copy(new_ls[0], ls[0]);
					mathVec3Copy(new_ls[1], ls[1]);
					mathVec3Copy(p, lp);
				}
				if (mathLineSegmentHasPoint(new_ls, p)) {
					result->distance = d;
					result->hit_point_cnt = 1;
					mathVec3Copy(result->hit_point, p);
					return result;
				}
			}
			p_result = NULL;
			for (i = 0; i < 2; ++i) {
				if (mathRaycastSphere(ls[i], dir, center, radius, &results[i]) &&
					(!p_result || p_result->distance > results[i].distance))
				{
					p_result = &results[i];
				}
			}
			if (p_result) {
				copy_result(result, p_result);
				return result;
			}
			return NULL;
		}
	}
}

/*
CCTResult_t* mathLineSegmentcastCylinder(float ls[2][3], float dir[3], float cp[2][3], float radius, CCTResult_t* result) {
	int i, j, res;
	CCTResult_t results[8], *p_result;
	float lsdir[3], N[3], intersect_point[2][3];
	mathVec3Sub(lsdir, ls[1], ls[0]);
	mathVec3Normalized(lsdir, lsdir);
	res = mathCylinderIntersectLine(cp, radius, ls[0], lsdir, intersect_point);
	if (1 == res) {
		if (mathLineSegmentHasPoint(ls, intersect_point[0])) {
			result->distance = 0.0f;
			result->hit_point_cnt = 1;
			mathVec3Copy(result->hit_point, intersect_point[0]);
			return result;
		}
	}
	else if (2 == res) {
		int c[2];
		for (i = 0; i < 2; ++i) {
			c[i] = mathLineSegmentHasPoint(intersect_point, ls[i]);
			if (c[i] && !mathVec3Equal(intersect_point[0], ls[i]) && !mathVec3Equal(intersect_point[1], ls[i])) {
				result->distance = 0.0f;
				result->hit_point_cnt = -1;
				return result;
			}
		}
		for (i = 0; i < 2; ++i) {
			c[i] = mathLineSegmentHasPoint(ls, intersect_point[i]);
			if (c[i] && !mathVec3Equal(ls[0], intersect_point[i]) && !mathVec3Equal(ls[1], intersect_point[i])) {
				result->distance = 0.0f;
				result->hit_point_cnt = -1;
				return result;
			}
		}
		if (c[0]) {
			result->distance = 0.0f;
			result->hit_point_cnt = 1;
			mathVec3Copy(result->hit_point, intersect_point[0]);
			return result;
		}
		if (c[1]) {
			result->distance = 0.0f;
			result->hit_point_cnt = 1;
			mathVec3Copy(result->hit_point, intersect_point[1]);
		}
	}
	mathVec3Cross(N, lsdir, dir);
	p_result = NULL;
	i = j = 0;
	if (!mathVec3IsZero(N)) {
		float intersect_res[4][3];
		res = mathCylinderInfiniteIntersectPlane(cp, radius, ls[0], mathVec3Normalized(N, N), intersect_res);
		if (0 == res)
			return NULL;
		else {
			float axis[3];
			if (1 == res) {
				if (!mathLineSegmentHasPoint(cp, intersect_res[0]))
					return NULL;
				return mathLineSegmentcastCircle(ls, dir, intersect_res[0], radius, intersect_res[1], result);
			}
			else if (2 == res) {
				int has_point_cnt = 0;
				float neg_dir[3];
				mathVec3Negate(neg_dir, dir);
				for (; i < 4; ++i) {
					if (!mathCylinderHasPoint(cp, radius, intersect_res[i]))
						continue;
					has_point_cnt++;
					if (!mathRaycastLineSegment(intersect_res[i], neg_dir, ls, &results[i]))
						continue;
					if (!p_result || p_result->distance > results[i].distance) {
						p_result = &results[i];
						mathVec3Copy(p_result->hit_point, intersect_res[i]);
					}
				}
				if (0 == has_point_cnt)
					return NULL;
				j = i;
			}
			mathVec3Sub(axis, cp[1], cp[0]);
			mathVec3Normalized(axis, axis);
			for (; i < j + 2; ++i) {
				if (!mathLineSegmentcastCircle(ls, dir, cp[i - j], radius, axis, &results[i]))
					continue;
				if (!p_result) {
					p_result = &results[i];
				}
				else {
					int cmp = fcmpf(p_result->distance, results[i].distance, CCT_EPSILON);
					if (0 == cmp)
						p_result->hit_point_cnt = -1;
					else if (cmp > 0)
						p_result = &results[i];
				}
			}
			j = i;
		}
	}
	for (; i < j + 2; ++i) {
		if (!mathRaycastCylinder(ls[i - j], dir, cp, radius, &results[i]))
			continue;
		if (!p_result)
			p_result = &results[i];
		else {
			int cmp = fcmpf(p_result->distance, results[i].distance, CCT_EPSILON);
			if (0 == cmp)
				p_result->hit_point_cnt = -1;
			else if (cmp > 0)
				p_result = &results[i];
		}
	}
	if (p_result) {
		copy_result(result, p_result);
		return result;
	}
	return NULL;
}
*/

CCTResult_t* mathCirclecastCircle(float o1[3], float r1, float n1[3], float dir[3], float o2[3], float r2, float n2[3], CCTResult_t* result) {
	float cos_theta = mathVec3Dot(n1, n2);
	if (fcmpf(cos_theta, 1.0f, CCT_EPSILON) && fcmpf(cos_theta, -1.0f, CCT_EPSILON)) {
		// TODO
		return NULL;
	}
	else {
		int cmp;
		float d;
		mathPointProjectionPlane(o1, o2, n2, NULL, &d);
		cos_theta = mathVec3Dot(n2, dir);
		if (fcmpf(d, 0.0f, CCT_EPSILON)) {
			float new_o1[3], new_o1o2[3];
			if (fcmpf(cos_theta, 0.0f, CCT_EPSILON) == 0)
				return NULL;
			d /= cos_theta;
			if (fcmpf(d, 0.0f, CCT_EPSILON) < 0)
				return NULL;
			mathVec3AddScalar(mathVec3Copy(new_o1, o1), dir, d);
			mathVec3Sub(new_o1o2, o2, new_o1);
			cmp = fcmpf(mathVec3LenSq(new_o1o2), (r1 + r2) * (r1 + r2), CCT_EPSILON);
			if (cmp < 0) {
				result->distance = 0.0f;
				result->hit_point_cnt = -1;
				return result;
			}
			else if (0 == cmp) {
				mathVec3Normalized(new_o1o2, new_o1o2);
				result->distance = 0.0f;
				result->hit_point_cnt = 1;
				mathVec3AddScalar(mathVec3Copy(result->hit_point, new_o1), new_o1o2, r1);
				return result;
			}
			return NULL;
		}
		else {
			float o1o2[3];
			mathVec3Sub(o1o2, o2, o1);
			cmp = fcmpf(mathVec3LenSq(o1o2), (r1 + r2) * (r1 + r2), CCT_EPSILON);
			if (cmp < 0) {
				result->distance = 0.0f;
				result->hit_point_cnt = -1;
				return result;
			}
			else if (0 == cmp) {
				mathVec3Normalized(o1o2, o1o2);
				result->distance = 0.0f;
				result->hit_point_cnt = 1;
				mathVec3AddScalar(mathVec3Copy(result->hit_point, o1), o1o2, r1);
				return result;
			}
			else if (fcmpf(cos_theta, 0.0f, CCT_EPSILON))
				return NULL;
			else {
				float distance, dn, delta_d, r1r2sum = r1 + r2;
				float dot = mathVec3Dot(o1o2, dir);
				if (fcmpf(dot, 0.0f, CCT_EPSILON) <= 0)
					return NULL;
				dn = mathVec3LenSq(o1o2) - dot * dot;
				delta_d = r1r2sum * r1r2sum - dn;
				if (fcmpf(delta_d, 0.0f, CCT_EPSILON) < 0)
					return NULL;
				distance = dot - sqrtf(delta_d);
				result->distance = distance;
				result->hit_point_cnt = 1;
				mathVec3AddScalar(mathVec3Copy(result->hit_point, o1), dir, distance);
				mathVec3Sub(o1o2, o2, result->hit_point);
				mathVec3Normalized(o1o2, o1o2);
				mathVec3AddScalar(result->hit_point, o1o2, r1);
				return result;
			}
		}
	}
}

CCTResult_t* mathTrianglecastPlane(float tri[3][3], float dir[3], float vertice[3], float normal[3], CCTResult_t* result) {
	CCTResult_t results[3], *p_result = NULL;
	int i;
	for (i = 0; i < 3; ++i) {
		float ls[2][3];
		mathVec3Copy(ls[0], tri[i % 3]);
		mathVec3Copy(ls[1], tri[(i + 1) % 3]);
		if (mathLineSegmentcastPlane(ls, dir, vertice, normal, &results[i])) {
			if (!p_result)
				p_result = &results[i];
			else {
				int cmp = fcmpf(p_result->distance, results[i].distance, CCT_EPSILON);
				if (0 == cmp) {
					if (results[i].hit_point_cnt < 0 ||
						p_result->hit_point_cnt < 0 ||
						!mathVec3Equal(p_result->hit_point, results[i].hit_point))
					{
						p_result->hit_point_cnt = -1;
					}
				}
				else if (cmp > 0)
					p_result = &results[i];
			}
		}
	}
	if (p_result) {
		copy_result(result, p_result);
		return result;
	}
	return NULL;
}

CCTResult_t* mathTrianglecastTriangle(float tri1[3][3], float dir[3], float tri2[3][3], CCTResult_t* result) {
	float neg_dir[3];
	CCTResult_t results[6], *p_result = NULL;
	int i, cmp;
	for (i = 0; i < 3; ++i) {
		float ls[2][3];
		mathVec3Copy(ls[0], tri1[i % 3]);
		mathVec3Copy(ls[1], tri1[(i + 1) % 3]);
		if (!mathLineSegmentcastTriangle(ls, dir, tri2, &results[i]))
			continue;
		if (!p_result) {
			p_result = &results[i];
			continue;
		}
		cmp = fcmpf(p_result->distance, results[i].distance, CCT_EPSILON);
		if (cmp > 0)
			p_result = &results[i];
		else if (0 == cmp && results[i].hit_point_cnt < 0)
			p_result = &results[i];
	}
	mathVec3Negate(neg_dir, dir);
	for (; i < 6; ++i) {
		float ls[2][3];
		mathVec3Copy(ls[0], tri2[i % 3]);
		mathVec3Copy(ls[1], tri2[(i + 1) % 3]);
		if (!mathLineSegmentcastTriangle(ls, neg_dir, tri1, &results[i]))
			continue;
		if (!p_result) {
			p_result = &results[i];
			continue;
		}
		cmp = fcmpf(p_result->distance, results[i].distance, CCT_EPSILON);
		if (cmp > 0)
			p_result = &results[i];
		else if (0 == cmp && results[i].hit_point_cnt < 0)
			p_result = &results[i];
	}
	if (p_result) {
		copy_result(result, p_result);
		return result;
	}
	return NULL;
}

/*
	     7+------+6			0 = ---
	     /|     /|			1 = +--
	    / |    / |			2 = ++-
	   / 4+---/--+5			3 = -+-
	 3+------+2 /    y   z	4 = --+
	  | /    | /     |  /	5 = +-+
	  |/     |/      |/		6 = +++
	 0+------+1      *---x	7 = -++
*/
static int Box_Edge_Indices[] = {
	0, 1,	1, 2,	2, 3,	3, 0,
	7, 6,	6, 5,	5, 4,	4, 7,
	1, 5,	6, 2,
	3, 7,	4, 0
};
static int Box_Triangle_Vertices_Indices[] = {
	0, 1, 2,	2, 3, 0,
	7, 6, 5,	5, 4, 7,
	1, 5, 6,	6, 2, 1,
	3, 7, 4,	4, 0, 3,
	3, 7, 6,	6, 2, 3,
	0, 4, 5,	5, 1, 0
};
static void AABBVertices(float o[3], float half[3], float v[8][3]) {
	v[0][0] = o[0] - half[0], v[0][1] = o[1] - half[1], v[0][2] = o[2] - half[2];
	v[1][0] = o[0] + half[0], v[1][1] = o[1] - half[1], v[1][2] = o[2] - half[2];
	v[2][0] = o[0] + half[0], v[2][1] = o[1] + half[1], v[2][2] = o[2] - half[2];
	v[3][0] = o[0] - half[0], v[3][1] = o[1] + half[1], v[3][2] = o[2] - half[2];
	v[4][0] = o[0] - half[0], v[4][1] = o[1] - half[1], v[4][2] = o[2] + half[2];
	v[5][0] = o[0] + half[0], v[5][1] = o[1] - half[1], v[5][2] = o[2] + half[2];
	v[6][0] = o[0] + half[0], v[6][1] = o[1] + half[1], v[6][2] = o[2] + half[2];
	v[7][0] = o[0] - half[0], v[7][1] = o[1] + half[1], v[7][2] = o[2] + half[2];
}

CCTResult_t* mathAABBcastPlane(float o[3], float half[3], float dir[3], float vertice[3], float normal[3], CCTResult_t* result) {
	CCTResult_t* p_result = NULL;
	float v[8][3];
	int i;
	AABBVertices(o, half, v);
	for (i = 0; i < sizeof(Box_Triangle_Vertices_Indices) / sizeof(Box_Triangle_Vertices_Indices[0]); i += 3) {
		CCTResult_t result_temp;
		float tri[3][3];
		mathVec3Copy(tri[0], v[Box_Triangle_Vertices_Indices[i]]);
		mathVec3Copy(tri[1], v[Box_Triangle_Vertices_Indices[i + 1]]);
		mathVec3Copy(tri[2], v[Box_Triangle_Vertices_Indices[i + 2]]);
		if (mathTrianglecastPlane(tri, dir, vertice, normal, &result_temp) &&
			(!p_result || p_result->distance > result_temp.distance))
		{
			copy_result(result, &result_temp);
			p_result = result;
		}
	}
	return p_result;
}

CCTResult_t* mathAABBcastAABB(float o1[3], float half1[3], float dir[3], float o2[3], float half2[3], CCTResult_t* result) {
	if (mathAABBIntersectAABB(o1, half1, o2, half2)) {
		result->distance = 0.0f;
		result->hit_point_cnt = -1;
		return result;
	}
	else {
		CCTResult_t *p_result = NULL;
		int i;
		float v1[8][3], v2[8][3];
		AABBVertices(o1, half1, v1);
		AABBVertices(o2, half2, v2);
		for (i = 0; i < sizeof(Box_Triangle_Vertices_Indices) / sizeof(Box_Triangle_Vertices_Indices[0]); i += 3) {
			int j;
			float tri1[3][3];
			mathVec3Copy(tri1[0], v1[Box_Triangle_Vertices_Indices[i]]);
			mathVec3Copy(tri1[1], v1[Box_Triangle_Vertices_Indices[i + 1]]);
			mathVec3Copy(tri1[2], v1[Box_Triangle_Vertices_Indices[i + 2]]);
			for (j = 0; j < sizeof(Box_Triangle_Vertices_Indices) / sizeof(Box_Triangle_Vertices_Indices[0]); j += 3) {
				CCTResult_t result_temp;
				float tri2[3][3];
				mathVec3Copy(tri2[0], v2[Box_Triangle_Vertices_Indices[j]]);
				mathVec3Copy(tri2[1], v2[Box_Triangle_Vertices_Indices[j + 1]]);
				mathVec3Copy(tri2[2], v2[Box_Triangle_Vertices_Indices[j + 2]]);

				if (!mathTrianglecastTriangle(tri1, dir, tri2, &result_temp))
					continue;

				if (!p_result || p_result->distance > result_temp.distance) {
					p_result = result;
					copy_result(p_result, &result_temp);
				}
			}
		}
		return p_result;
	}
}

CCTResult_t* mathSpherecastPlane(float o[3], float radius, float dir[3], float vertices[3][3], CCTResult_t* result) {
	float N[3], dn, dn_abs, d, cos_theta;
	mathPlaneNormalByVertices3(vertices, N);
	mathPointProjectionPlane(o, vertices[0], N, NULL, &dn);
	if (fcmpf(dn * dn, radius * radius, CCT_EPSILON) < 0) {
		result->distance = 0.0f;
		result->hit_point_cnt = -1;
		return result;
	}
	cos_theta = mathVec3Dot(N, dir);
	if (fcmpf(cos_theta, 0.0f, CCT_EPSILON) == 0)
		return NULL;
	d = dn / cos_theta;
	if (fcmpf(d, 0.0f, CCT_EPSILON) < 0)
		return NULL;
	dn_abs = fcmpf(dn, 0.0f, CCT_EPSILON) > 0 ? dn : -dn;
	d -= radius / dn_abs * d;
	result->distance = d;
	result->hit_point_cnt = 1;
	mathVec3Copy(result->hit_point, o);
	mathVec3AddScalar(result->hit_point, dir, d);
	if (fcmpf(dn, 0.0f, CCT_EPSILON) < 0)
		mathVec3AddScalar(result->hit_point, N, -radius);
	else
		mathVec3AddScalar(result->hit_point, N, radius);
	return result;
}

CCTResult_t* mathSpherecastSphere(float o1[3], float r1, float dir[3], float o2[3], float r2, CCTResult_t* result) {
	float r12 = r1 + r2;
	float o1o2[3];
	mathVec3Sub(o1o2, o2, o1);
	if (fcmpf(mathVec3LenSq(o1o2), r12 * r12, CCT_EPSILON) < 0) {
		result->distance = 0.0f;
		result->hit_point_cnt = -1;
		return result;
	}
	else {
		float dot = mathVec3Dot(o1o2, dir);
		if (fcmpf(dot, 0.0f, CCT_EPSILON) <= 0)
			return NULL;
		else {
			float distance, new_o1o2[3];
			float dn = mathVec3LenSq(o1o2) - dot * dot;
			float delta_len = r12 * r12 - dn;
			if (fcmpf(delta_len, 0.0f, CCT_EPSILON) < 0)
				return NULL;
			distance = dot - sqrtf(delta_len);
			result->distance = distance;
			result->hit_point_cnt = 1;
			mathVec3AddScalar(mathVec3Copy(result->hit_point, o1), dir, distance);
			mathVec3Sub(new_o1o2, o2, result->hit_point);
			mathVec3Normalized(new_o1o2, new_o1o2);
			mathVec3AddScalar(result->hit_point, new_o1o2, r1);
			return result;
		}
	}
}

CCTResult_t* mathSpherecastTriangle(float o[3], float radius, float dir[3], float tri[3][3], CCTResult_t* result) {
	if (!mathSpherecastPlane(o, radius, dir, tri, result))
		return NULL;
	if (result->hit_point_cnt > 0 && mathTriangleHasPoint(tri, result->hit_point, NULL, NULL))
		return result;
	else {
		CCTResult_t* p_result = NULL;
		int i;
		float neg_dir[3];
		mathVec3Negate(neg_dir, dir);
		for (i = 0; i < 3; ++i) {
			CCTResult_t result_temp;
			float ls[2][3];
			mathVec3Copy(ls[0], tri[i % 3]);
			mathVec3Copy(ls[1], tri[(i + 1) % 3]);
			if (mathLineSegmentcastSphere(ls, neg_dir, o, radius, &result_temp) &&
				(!p_result || p_result->distance > result_temp.distance))
			{
				copy_result(result, &result_temp);
				p_result = result;
			}
		}
		if (p_result)
			mathVec3AddScalar(p_result->hit_point, dir, p_result->distance);
		return p_result;
	}
}

CCTResult_t* mathSpherecastTrianglesPlane(float o[3], float radius, float dir[3], float vertices[][3], int indices[], int indicescnt, CCTResult_t* result) {
	float tri[3][3];
	mathVec3Copy(tri[0], vertices[indices[0]]);
	mathVec3Copy(tri[1], vertices[indices[1]]);
	mathVec3Copy(tri[2], vertices[indices[2]]);
	if (!mathSpherecastPlane(o, radius, dir, tri, result))
		return NULL;
	else if (result->hit_point_cnt > 0 && mathTriangleHasPoint(tri, result->hit_point, NULL, NULL))
		return result;
	else {
		float neg_dir[3];
		CCTResult_t *p_result;
		int i = 3;
		while (i < indicescnt) {
			mathVec3Copy(tri[0], vertices[indices[i++]]);
			mathVec3Copy(tri[1], vertices[indices[i++]]);
			mathVec3Copy(tri[2], vertices[indices[i++]]);
			if (result->hit_point_cnt > 0 && mathTriangleHasPoint(tri, result->hit_point, NULL, NULL))
				return result;
		}
		mathVec3Negate(neg_dir, dir);
		p_result = NULL;
		for (i = 0; i < indicescnt; i += 3) {
			int j;
			for (j = 0; j < 3; ++j) {
				CCTResult_t result_temp;
				float ls[2][3];
				mathVec3Copy(ls[0], vertices[indices[j % 3 + i]]);
				mathVec3Copy(ls[1], vertices[indices[(j + 1) % 3 + i]]);
				if (mathLineSegmentcastSphere(ls, neg_dir, o, radius, &result_temp) &&
					(!p_result || p_result->distance > result_temp.distance))
				{
					copy_result(result, &result_temp);
					p_result = result;
				}
			}
		}
		if (p_result)
			mathVec3AddScalar(p_result->hit_point, dir, p_result->distance);
		return p_result;
	}
}

CCTResult_t* mathSpherecastAABB(float o[3], float radius, float dir[3], float center[3], float half[3], CCTResult_t* result) {
	CCTResult_t *p_result = NULL;
	int i;
	float v[8][3];
	AABBVertices(center, half, v);
	for (i = 0; i < sizeof(Box_Triangle_Vertices_Indices) / sizeof(Box_Triangle_Vertices_Indices[0]); i += 6) {
		CCTResult_t result_temp;
		if (mathSpherecastTrianglesPlane(o, radius, dir, v, Box_Triangle_Vertices_Indices + i, 6, &result_temp) &&
			(!p_result || p_result->distance > result_temp.distance))
		{
			copy_result(result, &result_temp);
			p_result = result;
		}
	}
	return p_result;
}

CCTResult_t* mathCylindercastPlane(float p[2][3], float radius, float dir[3], float vertices[3][3], CCTResult_t* result) {
	float dn0, dn1, N[3], pn0[3], pn1[3];
	mathPlaneNormalByVertices3(vertices, N);
	mathPointProjectionPlane(p[0], vertices[0], N, pn0, &dn0);
	mathPointProjectionPlane(p[1], vertices[0], N, pn1, &dn1);
	if (fcmpf(dn0 * dn1, 0.0f, CCT_EPSILON) <= 0) {
		result->distance = 0.0f;
		result->hit_point_cnt = -1;
		return result;
	}
	else {
		float p0p1[3], p0p1_lensq, d, cos_theta, pn[3], *hp;
		float delta_d = dn1 - dn0;
		mathVec3Sub(p0p1, p[1], p[0]);
		p0p1_lensq = mathVec3LenSq(p0p1);
		cos_theta = sqrtf((p0p1_lensq - delta_d * delta_d) / p0p1_lensq);
		result->hit_point_cnt = fcmpf(cos_theta, 1.0f, CCT_EPSILON) ? 1 : -1;
		if (fcmpf(dn0, 0.0f, CCT_EPSILON) < 0) {
			if (dn0 < dn1) {
				d = dn1;
				hp = p[1];
				mathVec3Sub(pn, pn1, p[1]);
			}
			else {
				d = dn0;
				hp = p[0];
				mathVec3Sub(pn, pn0, p[0]);
			}
			d += radius * cos_theta;
			if (fcmpf(d, 0.0f, CCT_EPSILON) > 0) {
				result->distance = 0.0f;
				result->hit_point_cnt = -1;
				return result;
			}
		}
		else {
			if (dn0 < dn1) {
				d = dn0;
				hp = p[0];
				mathVec3Sub(pn, pn0, p[0]);
			}
			else {
				d = dn1;
				hp = p[1];
				mathVec3Sub(pn, pn1, p[1]);
			}
			d -= radius * cos_theta;
			if (fcmpf(d, 0.0f, CCT_EPSILON) < 0) {
				result->distance = 0.0f;
				result->hit_point_cnt = -1;
				return result;
			}
		}
		cos_theta = mathVec3Dot(N, dir);
		if (fcmpf(cos_theta, 0.0f, CCT_EPSILON) == 0)
			return NULL;
		d /= cos_theta;
		if (fcmpf(d, 0.0f, CCT_EPSILON) < 0)
			return NULL;
		result->distance = d;
		if (result->hit_point_cnt > 0) {
			float v[3];
			mathVec3Cross(v, p0p1, N);
			mathVec3Cross(v, p0p1, v);
			if (fcmpf(mathVec3Dot(v, pn), 0.0f, CCT_EPSILON) < 0)
				mathVec3Negate(v, v);
			mathVec3Normalized(v, v);
			mathVec3Copy(result->hit_point, hp);
			mathVec3AddScalar(result->hit_point, v, radius);
			mathVec3AddScalar(result->hit_point, dir, d);
		}
		return result;
	}
}

#ifdef	__cplusplus
}
#endif
