#ifndef _ENGINE_COMPONENT_COMPONENT_H_
#define _ENGINE_COMPONENT_COMPONENT_H_

#include <CppUtil/Basic/Element.h>

namespace CppUtil {
	namespace Engine {
		class SObj;

		class Component : public Basic::Element {
		protected:
			Component(Basic::Ptr<SObj> sobj) : wSObj(sobj) { }
			virtual ~Component() = default;

		protected:
			virtual void Init() override;

		public:
			const Basic::Ptr<SObj> GetSObj() const { return wSObj.lock(); }
			void SetSObj(Basic::Ptr<SObj> sobj) { wSObj = sobj; }

		private:
			Basic::WPtr<SObj> wSObj;
		};
	}
}

#endif // !_ENGINE_COMPONENT_COMPONENT_H_
