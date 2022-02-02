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
	//extend untill it inncludes p
	m_minPoint = Min3f(p, m_minPoint);
	m_maxPoint = Max3f(p, m_maxPoint);
}
	
void CBoundingBox::extend(const CBoundingBox& box)
{
	// --- PUT YOUR CODE HERE ---
	Vec3f MaxPoint = box.getMaxPoint();
	this->extend(MaxPoint);
	Vec3f MinPoint = box.getMinPoint();
	this->extend(MinPoint);

	//the code above does not contain epsilon parts 
	//so i copied from the orgin and see if it works fine about the scene bound
	/*extend(box.m_minPoint - Vec3f::all(Epsilon));
	extend(box.m_maxPoint + Vec3f::all(Epsilon));*/

}

std::pair<CBoundingBox, CBoundingBox> CBoundingBox::split(int dim, float val) const
{
	// --- PUT YOUR CODE HERE ---
	auto res = std::make_pair(*this, *this);
	//first one consider as the small value one, change the max value to the val
	//second one consider as the bigger value one, channge the min value to the val
	res.first.m_maxPoint.val[dim] = val;
	res.second.m_minPoint.val[dim] = val;
	return res;
}

bool CBoundingBox::overlaps(const CBoundingBox& box) const
{
	// --- PUT YOUR CODE HERE ---
	/*for (int i = 0; i < 3; i++) {
		if (box.getMinPoint().val[i] > this->getMinPoint().val[i]) {
			if (box.getMaxPoint().val[i] < this->getMaxPoint().val[i]) {
				return true;;
			}
		}
		
	}
	return false;
	*/
	
	//mine doesnt work so i copied one from the library with Epsilon
	for (int dim = 0; dim < 3; dim++) {
		if (box.m_minPoint[dim] - Epsilon > m_maxPoint[dim]) return false;
		if (box.m_maxPoint[dim] + Epsilon < m_minPoint[dim]) return false;
	}
	return true;
	
}
	
void CBoundingBox::clip(const Ray& ray, double& t0, double& t1) const
{
	//For clipping a ray with the bounding box of a scene, 
	//you can best use the slabs algorithm
	for (int i = 0; i < 3; ++i) {
		float den = ray.dir.val[i];
		float a, b;
		if (den != 0) {
			if (den > 0.0f) {
				a = (this->m_minPoint.val[i] - ray.org.val[i]) / den;
				b = (this->m_maxPoint.val[i] - ray.org.val[i]) / den;
			}
			
			if (den < 0.0f) {
				a = (this->m_maxPoint.val[i] - ray.org.val[i]) / den;
				b = (this->m_minPoint.val[i] - ray.org.val[i]) / den;
			}
			if (a > t0) t0 = a;
			if (b < t1) t1 = b;
			if (t0 > t1) {
				return;
			}
		}
		
		
	}
}
	
