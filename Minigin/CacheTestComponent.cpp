#include "CacheTestComponent.h"
#include <imgui.h>
#include <imgui_plot.h>
#include <iostream>
#include <chrono>
#include <numeric>

using namespace dae;

CacheTestComponent::CacheTestComponent(GameObject* parent) : Component{ parent }
{
}

void dae::CacheTestComponent::Update(float)
{
	ImGui::Begin("Exercise 1");

	//Shared plot config
	ImGui::PlotConfig config{};
	config.frame_size = ImVec2(180, 120);
	config.tooltip.show = true;
	config.tooltip.format = "Stepsize %g: %.8g microseconds";
	config.values.xs = m_TestSteps.data();

	ImGui::InputInt("# samples", &m_NrOfSamples, 1, 10);

	if (ImGui::Button("Test cache with integers")) {
		m_IntTestData.clear();
		int* buffer = new int[static_cast<size_t>(std::pow(2, 26))];
		for (float step : m_TestSteps) {
			m_IntTestData.push_back(static_cast<float>(Measure(std::bind(&CacheTestComponent::IncrementBuffer<int>, this, buffer, static_cast<int>(std::pow(2, 26)), static_cast<int>(step)))));
		}
		delete[] buffer;
	}

	if (m_IntTestData.size() > 0) {
		config.values.ys = m_IntTestData.data();
		config.values.count = static_cast<int>(m_IntTestData.size());
		config.scale.min = 0.0f;
		config.scale.max = *std::ranges::max_element(m_IntTestData);
		config.values.color = IM_COL32(255, 0, 0, 255);
		ImGui::Plot("Integers", config);
	}

	ImGui::End();

	ImGui::Begin("Exercise 2");

	ImGui::InputInt("# samples", &m_NrOfSamples, 1, 10);

	if (ImGui::Button("Test cache with gameObjects")) {
		m_ObjectTestData.clear();
		TestObject* buffer = new TestObject[static_cast<size_t>(std::pow(2, 26))];
		for (float step : m_TestSteps) {
			m_ObjectTestData.push_back(static_cast<float>(Measure(std::bind(&CacheTestComponent::IncrementBuffer<TestObject>, this, buffer, static_cast<int>(std::pow(2, 26)), static_cast<int>(step)))));
		}
		delete[] buffer;
	}

	if (m_ObjectTestData.size() > 0) {
		config.values.ys = m_ObjectTestData.data();
		config.values.count = static_cast<int>(m_ObjectTestData.size());
		config.scale.min = 0.0f;
		config.scale.max = *std::ranges::max_element(m_ObjectTestData);
		config.values.color = IM_COL32(0, 255, 0, 255);
		ImGui::Plot("GameObjects", config);
	}

	if (ImGui::Button("Test cache with small gameObjects")) {
		m_SmallObjectTestData.clear();
		TestObjectSmall* buffer = new TestObjectSmall[static_cast<size_t>(std::pow(2, 26))];
		for (float step : m_TestSteps) {
			m_SmallObjectTestData.push_back(static_cast<float>(Measure(std::bind(&CacheTestComponent::IncrementBuffer<TestObjectSmall>, this, buffer, static_cast<int>(std::pow(2, 26)), static_cast<int>(step)))));
		}
		delete[] buffer;
	}

	if (m_SmallObjectTestData.size() > 0) {
		config.values.ys = m_SmallObjectTestData.data();
		config.values.count = static_cast<int>(m_SmallObjectTestData.size());
		config.scale.min = 0.0f;
		config.scale.max = *std::ranges::max_element(m_SmallObjectTestData);
		config.values.color = IM_COL32(0, 0, 255, 255);
		ImGui::Plot("SmallGameObjects", config);
	}

	if (m_SmallObjectTestData.size() > 0 and m_ObjectTestData.size() > 0) {
		ImGui::Text("Combined graph");
		//Remove the single plot data
		config.values.color = 0;
		config.values.ys = nullptr;
		config.tooltip.show = false;

		const float* ys_list[2]{ m_ObjectTestData.data(), m_SmallObjectTestData.data() };
		const ImU32 colors[2]{ IM_COL32(0, 255, 0, 255), IM_COL32(0, 0, 255, 255) };
		config.values.colors = colors;
		config.values.ys_list = ys_list;
		config.values.ys_count = 2;
		config.values.count = static_cast<int>(m_ObjectTestData.size());
		config.scale.min = 0.0f;
		config.scale.max = *std::ranges::max_element(m_ObjectTestData);
		ImGui::Plot("CombinedGraphs", config);
	}

	ImGui::End();
}

long dae::CacheTestComponent::Measure(std::function<void()> function) const
{
	//Discard first result as it tends to be inaccurate
	function();
	std::vector<long> measurements(m_NrOfSamples);

	for (int measurement{}; measurement < m_NrOfSamples; ++measurement) {
		const auto begin = std::chrono::high_resolution_clock::now();
		function();
		const auto end = std::chrono::high_resolution_clock::now();
		measurements[measurement] = static_cast<long>(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
	}
	return std::accumulate(measurements.begin(), measurements.end(), 0) / m_NrOfSamples;
}
