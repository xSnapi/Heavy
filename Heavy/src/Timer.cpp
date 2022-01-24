#include <hvpch.h>
#include "Timer.hpp"

#include "Heavy Times.hpp"

namespace hv {
	Timer::Timer() {

	}

	Timer::~Timer() {

	}

	void Timer::Update() {
		m_elapsedTime += hv::dt;
		while (m_elapsedTime >= m_updateTime) {

			if(m_fun)
				m_fun();

			m_elapsedTime -= m_updateTime;
		}
	}

	void Timer::SetUpdateTime(const float& time) { m_updateTime = time; }

	void Timer::SetFunction(std::function<void()> fun) { m_fun = fun; }

	void Timer::Restart() { m_elapsedTime = 0.0f; }

	float Timer::GetUpdateTime() const { return m_updateTime; }

	std::function<void()> Timer::GetFunction() const { return m_fun; }
}