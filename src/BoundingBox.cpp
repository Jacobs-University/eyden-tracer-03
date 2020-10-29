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
	//if p is not in the box, extend.
	/*here we just compare the min and max bounds with the point
	* and this checks automatically if the point is inside the box.
	* and extends the box if the point is outside of it.
	* 
	* source: http://www.pbr-book.org/3ed-2018/Geometry_and_Transformations/Bounding_Boxes.html
	*/
	m_minPoint = Min3f(m_minPoint, p);
	m_maxPoint = Max3f(m_maxPoint, p);

}
	
void CBoundingBox::extend(const CBoundingBox& box)
{
	// --- PUT YOUR CODE HERE ---
	/* same as with the point check, but min and max
	*  of the second box are used for the test.
	* 
	*  source: http://www.pbr-book.org/3ed-2018/Geometry_and_Transformations/Bounding_Boxes.html
	*/
	extend(box.getMinPoint());
	extend(box.getMaxPoint());


}

std::pair<CBoundingBox, CBoundingBox> CBoundingBox::split(int dim, float val) const
{
	// --- PUT YOUR CODE HERE ---
	auto res = std::make_pair(*this, *this);
	/*splits into two parts;
	*/
	res.first.m_maxPoint[dim] = val;
	res.second.m_minPoint[dim] = val;

	return res;
}

bool CBoundingBox::overlaps(const CBoundingBox& box) const
{
	// --- PUT YOUR CODE HERE ---
	/*	check each min and max for both boxes
	*	to see if they overlap.
	*/
	if (box.m_minPoint.val[0] > m_maxPoint.val[0])
		return false;
	if (box.m_maxPoint.val[0] < m_minPoint.val[0])
		return false;
	if (box.m_minPoint.val[1] > m_maxPoint.val[1])
		return false;
	if (box.m_maxPoint.val[1] < m_minPoint.val[1])
		return false;
	if (box.m_minPoint.val[2] > m_maxPoint.val[2])
		return false;
	if (box.m_maxPoint.val[2] < m_minPoint.val[2])
		return false;

	return true;
}
	
void CBoundingBox::clip(const Ray& ray, double& t0, double& t1) const
{

	/* we use a ray-aabb intersection algorithm
	*	source: https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection
	*/

	// --- PUT YOUR CODE HERE ---
	double raydir;
	double xmin, xmax, ymin, ymax, zmin, zmax;//distances

	raydir = 1.0f / ray.dir.val[0];//avoid division by zero by using inverse

	//distance of BB to ray origin for min&max points
	//for 3 axes x, y, z.
	xmin = (m_minPoint.val[0] - ray.org.val[0])* raydir;
	xmax = (m_maxPoint.val[0] - ray.org.val[0])* raydir;
	if (xmax < xmin)
		swap(xmin, xmax); 
	//we want the smallest max and the largest min
	if (xmin > t0) t0 = xmin;
	if (xmax < t1) t1 = xmax;

	raydir = 1.0f / ray.dir.val[1];

	ymin = (m_minPoint.val[1] - ray.org.val[1])* raydir;
	ymax = (m_maxPoint.val[1] - ray.org.val[1])* raydir;
	if (ymax < ymin)
		swap(ymin, ymax);
	if (ymin > t0) t0 = ymin;
	if (ymax < t1) t1 = ymax;
	
	if (((xmin > ymax) || (ymin > xmax) || (t0 > t1)))
		//ray missed the box
		return;

	if (ymin > xmin)
		xmin = ymin;

	if (ymax < xmax)
		xmax = ymax;
	
	raydir = 1.0f / ray.dir.val[2];

	zmin = (m_minPoint.val[2] - ray.org.val[2]) * raydir;
	zmax = (m_maxPoint.val[2] - ray.org.val[2]) * raydir;
	if (zmax < zmin)
		swap(zmin, zmax);
	if (zmin > t0) t0 = zmin;
	if (zmax < t1) t1 = zmax;

	if (((xmin > zmax) || (zmin > xmax) || (t0 > t1)))
		return;//ray missed the box
	
}
	
