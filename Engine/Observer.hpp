#pragma once
#include "Hash.hpp"
#include <vector>
#include <string_view>
#include <any>


namespace dae
{
	class Event;
	class Observer;
	class Subject;

	class Event final
	{
	public:
		consteval explicit Event(const std::string_view eventName) : Event{ sdbm_hash(eventName) } {};
		~Event() = default;
		Event(const Event& other) = delete;
		Event(Event&& other) = delete;
		Event& operator=(const Event& other) = delete;
		Event& operator=(Event&& other) = delete;

		void SetData(std::any data) {
			m_Data = data;
		}
		template<typename DataType>
		DataType GetData() const {
			//assert(m_Data.has_value() && m_Data::type == typeid(DataType));
			return std::any_cast<DataType>(m_Data);
		}

		constexpr bool operator==(const std::string_view eventName) const {
			return m_EventId == sdbm_hash(eventName);
		}
		bool operator==(const uint32_t id) const {
			return m_EventId == id;
		}
		bool operator==(const Event& other) const {
			return m_EventId == other.m_EventId;
		}
	private:
		constexpr explicit Event(uint32_t eventId) : m_EventId{ eventId } {};
		uint32_t m_EventId{};
		std::any m_Data{};
	};

	class Observer
	{
	public:
		explicit Observer() = default;
		virtual ~Observer() = default;
		Observer(const Observer& other) = delete;
		Observer(Observer&& other) = delete;
		Observer& operator=(const Observer& other) = delete;
		Observer& operator=(Observer&& other) = delete;

		virtual void OnNotify(const Event& event) = 0;
	private:
		friend Subject;
		Subject* m_Subject = nullptr;
	};

	class Subject final
	{
	public:
		explicit Subject() = default;
		~Subject() = default;
		Subject(const Subject& other) = delete;
		Subject(Subject&& other) = delete;
		Subject& operator=(const Subject& other) = delete;
		Subject& operator=(Subject&& other) = delete;

		void AddObserver(Observer* observer) 
		{
			m_Observers.emplace_back(observer);
			observer->m_Subject = this;
		}
		void RemoveObserver(Observer* observer)
		{
			std::erase(m_Observers, observer);
			observer->m_Subject = nullptr;
		}
		void Notify(const Event& event)
		{
			for (Observer* observer : m_Observers)
				observer->OnNotify(event);
		}
	private:
		std::vector<Observer*> m_Observers{};
	};	
}
