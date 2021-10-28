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
		CBoundingBox bounds;
		// --- PUT YOUR CODE HERE ---
		bounds.m_minPoint = Vec3f::all(-Infty);
		bounds.m_maxPoint - Vec3f::all(Infty);
		return bounds;
	}

private:
	Vec3f m_normal;	///< Point on the plane
	Vec3f m_origin;	///< Normal to the plane
};
