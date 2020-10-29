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
	this->m_maxPoint[0] = max(p[0], this->m_maxPoint[0]);
	this->m_minPoint[0] = min(p[0], this->m_minPoint[0]);

	this->m_maxPoint[1] = max(p[1], this->m_maxPoint[1]);
	this->m_minPoint[1] = min(p[1], this->m_minPoint[1]);

	this->m_maxPoint[2] = max(p[2], this->m_maxPoint[2]);
	this->m_minPoint[2] = min(p[2], this->m_minPoint[2]);
	
}
	
void CBoundingBox::extend(const CBoundingBox& box)
{
	this->extend(box.m_minPoint);
	this->extend(box.m_maxPoint);
	// --- PUT YOUR CODE HERE ---
}

std::pair<CBoundingBox, CBoundingBox> CBoundingBox::split(int dim, float val) const
{
	// --- PUT YOUR CODE HERE ---

	CBoundingBox  box1;
	CBoundingBox  box2;
	
    box1.m_maxPoint = this->m_maxPoint;
	box1.m_maxPoint[dim] = this->m_maxPoint[dim] - val;
	box1.m_minPoint = this->m_minPoint;

	box2.m_minPoint = this->m_minPoint;
	box2.m_minPoint[dim] = this->m_minPoint[dim] + val;
	box2.m_maxPoint = box2.m_maxPoint;

	auto res = std::make_pair(box1, box2);

	return res;
}

bool CBoundingBox::overlaps(const CBoundingBox& box) const
{
	if (this->m_minPoint[0] < box.m_maxPoint[0] && box.m_minPoint[0] < this->m_maxPoint[0] &&
		this->m_minPoint[1] < box.m_maxPoint[1] && box.m_minPoint[1] < this->m_maxPoint[1] &&
		this->m_minPoint[2] < box.m_maxPoint[2] && box.m_minPoint[2] < this->m_maxPoint[2])
	{

		return true;
	}
	else
	{
		return false;
	}
}
	
void CBoundingBox::clip(const Ray& ray, double& t0, double& t1) const
{
	// --- PUT YOUR CODE HERE ---

	Vec3f vec0, vec1;

	float f_dim = std::numeric_limits<float>::infinity() * (-1);
	float s_dim = std::numeric_limits<float>::infinity();

	for (int i = 0; i < 3; i++) {
		if (ray.dir[i] == 0) {
			

			vec0[i] = (this->m_minPoint[i] - ray.org[i]) / ray.dir[i];
			vec1[i] = (this->m_maxPoint[i] - ray.org[i]) / ray.dir[i];

			if (s_dim<0 || f_dim > s_dim) {
				t0 = std::numeric_limits<float>::infinity();
				t1 = std::numeric_limits<float>::infinity();
				return;
			}
			if (vec0[i] > f_dim) {
				f_dim = vec0[i];
			}
			if (vec1[i] < s_dim) {
				s_dim = vec1[i];
			}
			if (vec0[i] > vec1[i]) {
				auto tmp = vec1;
				vec1 = vec0;
				vec0 = tmp;
			}

		}
		else {
			
			vec0[i] = (this->m_minPoint[i] - ray.org[i]) / ray.dir[i];
			vec1[i] = (this->m_maxPoint[i] - ray.org[i]) / ray.dir[i];
			if (s_dim<0 || f_dim >s_dim) {
				t0 = std::numeric_limits<float>::infinity();
				t1 = std::numeric_limits<float>::infinity();
				return;
			}
			if (vec0[i] > f_dim) {
				f_dim = vec0[i];
			}
			if (vec1[i] < s_dim) {
				s_dim = vec1[i];
			}
			if (vec0[i] > vec1[i]) {
				auto tmp = vec1;
				vec1 = vec0;
				vec0 = tmp;
			}

		}
	}
	t0 = f_dim;
	t1 = s_dim;
}
	
