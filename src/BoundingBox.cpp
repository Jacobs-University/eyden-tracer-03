#include "BoundingBox.h"
#include "ray.h"

namespace {
	inline Vec3f Min3f(const Vec3f a, const Vec3f b)
	{
		return Vec3f(MIN(a.val[0], b.val[0]), MIN(a.val[1], b.val[1]), MIN(a.val[2], b.val[2]));
	}

	inline Vec3f Max3f(const Vec3f a, const Vec3f b)
	{
		return Vec3f(MAX(a.val[0], b.val[0]), MAX(a.val[1], b.val[1]), MAX(a.val[2], b.val[2]));
	}
}

void CBoundingBox::extend(const Vec3f& p)
{
	// --- PUT YOUR CODE HERE ---
	m_minPoint = Min3f(p, m_minPoint);
	m_maxPoint = Max3f(p, m_maxPoint);
}
	
void CBoundingBox::extend(const CBoundingBox& box)
{
	// --- PUT YOUR CODE HERE ---
	extend(box.m_minPoint);
	extend(box.m_maxPoint);
}

std::pair<CBoundingBox, CBoundingBox> CBoundingBox::split(int dim, float val) const
{
	// --- PUT YOUR CODE HERE ---
	auto res = std::make_pair(*this, *this);
	res.first.m_maxPoint[dim] = val;
	res.second.m_minPoint[dim] = val;
	return res;
}

bool CBoundingBox::overlaps(const CBoundingBox& box) const
{
	// --- PUT YOUR CODE HERE ---
	for (int i = 0; i < 3; i++) {
		if (box.m_minPoint[i] - Epsilon > m_maxPoint[i])
			return false;
		else if (box.m_maxPoint[i] - Epsilon < m_minPoint[i])
			return false;
		else 
			return true;
	}
	return false;
}

void CBoundingBox::clip(const Ray& ray, double& t0, double& t1) const
{
	// --- PUT YOUR CODE HERE ---
    float d, den;
	for (int i = 0; i < 3; i++) {
		den = 1.0f / ray.dir.val[i];
		d = (m_minPoint.val[i] - ray.org.val[i]) * den;
		if (ray.dir.val[i] != 0) {
			if (d > t0) t0 = d;
			if (d < t1) t1 = d;
		} 
		else {
			if (d > t0) t0 = d;
			if (d < t1) t1 = d;
		} 
		// if (i == 0 || i == 1)
		if (t0 > t1) return;
	}
}
	
// Reference: https://github.com/Jacobs-University/eyden-tracer-04/blob/master/src/BoundingBox.cpp
