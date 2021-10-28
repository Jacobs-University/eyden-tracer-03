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
	this->m_minPoint = Min3f(p, this->m_minPoint);
	this->m_maxPoint = Max3f(p, this->m_maxPoint);
}
	
void CBoundingBox::extend(const CBoundingBox& box)
{
	// --- PUT YOUR CODE HERE ---
	this->extend(box.m_minPoint);
	this->extend(box.m_maxPoint);
}

std::pair<CBoundingBox, CBoundingBox> CBoundingBox::split(int dim, float val) const
{
	// --- PUT YOUR CODE HERE ---
	if (dim < 3) {
		CBoundingBox first(this->m_minPoint, val);
		CBoundingBox second(val, this->m_maxPoint);
		auto res = std::make_pair(first, second);

		return res;
	}

	return std::make_pair(*this, *this);
}

bool CBoundingBox::overlaps(const CBoundingBox& box) const
{
	// --- PUT YOUR CODE HERE ---
	for (int i = 0; i < 3; ++i) {
		if (this->m_minPoint.val[i] > box.m_minPoint.val[i]) {
			return false;
		}

		if (this->m_maxPoint.val[i] < box.m_maxPoint.val[i]) {
			return false;
		}
	}
	return true;
}
	
void CBoundingBox::clip(const Ray& ray, double& t0, double& t1) const
{
	// --- PUT YOUR CODE HERE ---

	for (int i = 0; i < 3; ++i) {

		float a = (this->m_minPoint.val[i] - ray.org.val[i]) / ray.dir.val[i];
		float b = (this->m_maxPoint.val[i] - ray.org.val[i]) / ray.dir.val[i];

		if (ray.dir.val[i] < 0.0f) {
			float temp = a;
			a = b;
			b = temp;
		}

		t0 = a > t0 ? a : t0;
		t1 = b < t1 ? b : t1;

		if (t0 > t1) {
			return;
		}
	}
}
	
