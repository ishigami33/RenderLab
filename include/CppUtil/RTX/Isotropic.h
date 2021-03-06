#ifndef _RTX_RT_MATERIAL_ISOTROPIC_H_
#define _RTX_RT_MATERIAL_ISOTROPIC_H_

#include <CppUtil/RTX/Material.h>
#include <glm/vec3.hpp>

namespace RTX {
	class Texture;

	class Isotropic : public Material{
		MATERIAL_SETUP(Isotropic)
	public:
		Isotropic(const glm::vec3 & color);

		// 返回值为 true 说明光线继续传播
		// 返回值为 false 说明光线不再传播
		virtual bool Scatter(const HitRecord & rec) const;
		const CppUtil::Basic::CPtr<Texture> GetTexture() const { return tex; }
	protected:
		CppUtil::Basic::CPtr<Texture> tex;
	};
}

#endif // !_RTX_RT_MATERIAL_ISOTROPIC_H_
