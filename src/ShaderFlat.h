#pragma once

#include "IShader.h"

/**
 * @brief Flat shader class
 */
class CShaderFlat : public IShader
{
public:
	/**
	 * @brief Constructor
	 * @details This is a texture-free and light-source-free shader
	 * @param color The color of the object
	 */
	CShaderFlat(const Vec3f& color) : m_color(color) {}

	virtual Vec3f shade(const Ray& ray = Ray()) const override
	{
		return m_color;
	}

private:
	Vec3f m_color;
};
