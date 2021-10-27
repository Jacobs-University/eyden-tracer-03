// Triangle Geometrical Primitive class
// Written by Sergey Kosov in 2005 for Rendering Competition
#pragma once

#include "IPrim.h"

// ================================ Triangle Primitive Class ================================
/**
 * @brief Triangle Geometrical Primitive class
 */
class CPrimTriangle : public IPrim
{
public:
	/**
	 * @brief Constructor
	 * @param pShader Pointer to the shader to be applied for the prim
	 * @param a Position of the first vertex
	 * @param b Position of the second vertex
	 * @param c Position of the third vertex
	 */
	CPrimTriangle(ptr_shader_t pShader, const Vec3f& a, const Vec3f& b, const Vec3f& c)
		: IPrim(pShader)
		, m_a(a)
		, m_b(b)
		, m_c(c)
		, m_edge1(b - a)
		, m_edge2(c - a)
	{}
	virtual ~CPrimTriangle(void) = default;

	virtual bool intersect(Ray& ray) const override
	{
		const Vec3f edge1 = m_b - m_a;
		const Vec3f edge2 = m_c - m_a;

		const Vec3f pvec = ray.dir.cross(edge2);

		const float det = edge1.dot(pvec);
		if (fabs(det) < Epsilon) return false;

		const float inv_det = 1.0f / det;

		const Vec3f tvec = ray.org - m_a;
		float lambda = tvec.dot(pvec);
		lambda *= inv_det;

		if (lambda < 0.0f || lambda > 1.0f) return false;

		const Vec3f qvec = tvec.cross(edge1);
		float mue = ray.dir.dot(qvec);
		mue *= inv_det;

		if (mue < 0.0f || mue + lambda > 1.0f) return false;

		float f = edge2.dot(qvec);
		f *= inv_det;
		if (ray.t <= f || f < Epsilon) return false;

		ray.t = f;
		ray.hit = shared_from_this();
		return true;
	}

	virtual Vec3f getNormal(const Ray& ray) const override
	{
		return normalize(m_edge1.cross(m_edge2));
	}

	virtual CBoundingBox getBoundingBox(void) const override
	{
		CBoundingBox res;
		res.extend(m_a);
		res.extend(m_b);
		res.extend(m_c);
		return res;
	}


private:
	Vec3f m_a;		///< Position of the first vertex
	Vec3f m_b;		///< Position of the second vertex
	Vec3f m_c;		///< Position of the third vertex
	Vec3f m_edge1;	///< Edge AB
	Vec3f m_edge2;	///< Edge AC
};
