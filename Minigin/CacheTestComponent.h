#pragma once
#include "Component.h"
#include <functional>
#include <array>

namespace dae
{
	class CacheTestComponent final : public Component
	{
	public:
		explicit CacheTestComponent(GameObject* parent);
		virtual ~CacheTestComponent() = default;
		CacheTestComponent(const CacheTestComponent& other) = delete;
		CacheTestComponent(CacheTestComponent&& other) = delete;
		CacheTestComponent& operator=(const CacheTestComponent& other) = delete;
		CacheTestComponent& operator=(CacheTestComponent&& other) = delete;

		virtual void Update(float /*elapsedSec*/) override;
	private:
		long Measure(std::function<void()> function) const;
		template<typename type>
		void IncrementBuffer(type* buffer, int bufferSize, int step);

		std::array<float, 11> m_TestSteps{ 1,2,4,8,16,32,64,128,256,512,1024 };
		std::vector<float> m_IntTestData;
		std::vector<float> m_ObjectTestData;
		std::vector<float> m_SmallObjectTestData;
		int m_NrOfSamples{10};

		struct Matrix
		{
			int matrix[16] = {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			};
		};
		struct TestObject
		{
			Matrix matrix;
			int id;
			void operator*=(int scale) { id *= scale; }
		};
		struct TestObjectSmall
		{
			Matrix* matrix;
			int id;
			void operator*=(int scale) { id *= scale; }
		};
	};

	template<typename type>
	inline void CacheTestComponent::IncrementBuffer(type* buffer, int bufferSize, int step)
	{
		for (int index{}; index < bufferSize; index += step)
		{
			buffer[index] *= 2;
		}
	}
}

