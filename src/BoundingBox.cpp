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
	m_minPoint = Min3f(m_minPoint, p);
	m_maxPoint = Max3f(m_maxPoint, p);
}
	
void CBoundingBox::extend(const CBoundingBox& box)
{
	m_minPoint = Min3f(m_minPoint, box.getMinPoint());
	m_maxPoint = Max3f(m_maxPoint, box.getMaxPoint());
}

std::pair<CBoundingBox, CBoundingBox> CBoundingBox::split(int dim, float val) const
{
	// create new box to represent up to val
	CBoundingBox lBox;
	lBox.extend(*this);
	lBox.m_maxPoint[dim] = val;
	
	//create new box to represent from val onwards
	CBoundingBox rBox;
	rBox.extend(*this);
	rBox.m_minPoint[dim] = val;
	
	return std::make_pair(lBox, rBox);
}

bool CBoundingBox::overlaps(const CBoundingBox& box) const
{
	//check for overlap in each axis, return false is an axis has no overlap
	for (int i = 0; i < 3; i++) {
		if (!((m_maxPoint[i] > box.getMaxPoint()[i] and box.getMaxPoint()[i] > m_minPoint[i]) or
			(box.getMaxPoint()[i] > m_maxPoint[i] and m_maxPoint[i] > box.getMinPoint()[i]))) {
			return false;
		}
	}
	return true;
}
	
void CBoundingBox::clip(const Ray& ray, double& t0, double& t1) const
{
    for (int i = 0; i < 3; i++) {
		if (ray.dir.val[i] != 0) {
			if (ray.dir.val[i] > 0) {
				t0 = MAX((m_minPoint.val[i] - ray.org.val[i]) / ray.dir.val[i], t0);
				t1 = MIN((m_maxPoint.val[i] - ray.org.val[i]) / ray.dir.val[i], t1);
			}
			else {
				t0 = MAX((m_maxPoint.val[i] - ray.org.val[i]) / ray.dir.val[i], t0);
				t1 = MIN((m_minPoint.val[i] - ray.org.val[i]) / ray.dir.val[i], t1);
			}
			if (t0 > t1)
				return;
		}
	}
}
	
