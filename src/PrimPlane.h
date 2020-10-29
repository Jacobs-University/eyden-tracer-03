// Plane Geaometrical Primitive class
// Written by Sergey Kosov in 2005 for Rendering Competition
#pragma once

#include "IPrim.h"

// ================================ Infinite Plane Primitive Class ================================
/**
 * @brief The Plane Geometrical Primitive class
 */
class CPrimPlane : public IPrim
{
public:
	/**
	 * @brief Constructor
	 * @param pShader Pointer to the shader to be applied for the prim
	 * @param origin Point on the plane
	 * @param normal Normal to the plane
	 */
	CPrimPlane(ptr_shader_t pShader, Vec3f origin, Vec3f normal)
		: IPrim(pShader)
		, m_normal(normal)
		, m_origin(origin)
	{
		normalize(m_normal);
	}
	virtual ~CPrimPlane(void) = default;

	virtual bool intersect(Ray& ray) const override
	{
		float dist = (m_origin - ray.org).dot(m_normal) / ray.dir.dot(m_normal);
		if (dist < Epsilon || isinf(dist) || dist > ray.t) return false;

		ray.t = dist;
		ray.hit = shared_from_this();
		return true;
	}

	virtual Vec3f getNormal(const Ray& ray) const override
	{
		return m_normal;
	}

	virtual CBoundingBox getBoundingBox(void) const override
	{
		// --- PUT YOUR CODE HERE ---
		/*	we check for a union between the plane and the box.
		*	the bounding box extends the plane, so in all of the
		*	3 vector directions, except in normal direction.
		*/
		//the plane goes to infinity in two axis


		Vec3f PointOne, PointTwo;
		//check which axis is the normal
		for(int i=0; i<3; i++)
			if(m_normal[i] == 1)
			{
				PointOne[i] = m_origin[i];
				PointTwo[i] = m_origin[i];
			}
			else
			{
				PointOne[i] = -Infty;
				PointTwo[i] = Infty;
			}
		CBoundingBox bounds(PointOne, PointTwo);
		return bounds;
	}

private://i think these are reversed??
	Vec3f m_normal;	///< Point on the plane
	Vec3f m_origin;	///< Normal to the plane
};
