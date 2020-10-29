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
	// resize min and max point to be just right for p
	m_minPoint = Min3f(p, m_minPoint);
    m_maxPoint = Max3f(p, m_maxPoint);
}
	
void CBoundingBox::extend(const CBoundingBox& box)
{
	// extend the box using new min and max points, could this also work with getMinPoint() and getMaxPoint()?
	extend(box.m_minPoint);
	extend(box.m_maxPoint);
}

std::pair<CBoundingBox, CBoundingBox> CBoundingBox::split(int dim, float val) const
{
	// --- PUT YOUR CODE HERE ---
	auto res = std::make_pair(*this, *this);
	return res;
}

bool CBoundingBox::overlaps(const CBoundingBox& box) const
{
	// --- PUT YOUR CODE HERE ---
	return false;
}
	
void CBoundingBox::clip(const Ray& ray, double& t0, double& t1) const
{
    
}
	
