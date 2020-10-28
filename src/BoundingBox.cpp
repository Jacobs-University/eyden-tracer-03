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
    // The function given above does most of the job for us here.
    m_minPoint = Min3f(p, m_minPoint);
    m_maxPoint = Max3f(p, m_maxPoint);
}
	
void CBoundingBox::extend(const CBoundingBox& box)
{
	extend(box.m_minPoint);
	extend(box.m_maxPoint);
}

std::pair<CBoundingBox, CBoundingBox> CBoundingBox::split(int dim, float val) const
{
	auto res = std::make_pair(*this, *this);
	// this order must be maintained because of what the nodes in the tree expect.
	res.first.m_maxPoint[dim] = val;
	res.second.m_minPoint[dim] = val;
	return res;
}

bool CBoundingBox::overlaps(const CBoundingBox& box) const
{
    // this is rather trivial since AABB only need coordinate arithmetic.
    for (int i = 0; i < 3; i++){
        if (this->m_minPoint[i] > box.m_maxPoint[i])
            return false;
        if (this->m_maxPoint[i] < box.m_minPoint[i])
            return false;
    }
    return true;
}
	
void CBoundingBox::clip(const Ray& ray, double& t0, double& t1) const
{
    for (int i = 0; i < 3; i++){
        if (ray.dir[i] == 0)
            continue;
        float tmin = (m_minPoint[i] - ray.org[i]) / ray.dir[i];
        float tmax = (m_maxPoint[i] - ray.org[i]) / ray.dir[i];
        t0 = tmin > t0 ? tmin : t0;
        t1 = tmax < t1 ? tmax : t1;
        if (ray.dir[i] < 0)
            swap(t0, t1);
        if (t0 > t1) break;
    }
}
	
