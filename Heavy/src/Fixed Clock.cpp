#include <hvpch.h>
#include "Fixed Clock.hpp"

// Little bit of code here is taken from 
// https://github.com/Bromeon/Thor/blob/master/src/StopWatch.cpp

namespace hv {
	FixedClock::FixedClock(bool start) {
		if (start)
			Start();
	}

	FixedClock::~FixedClock() {

	}

	void FixedClock::Start() {
		if (!m_running) {
			m_running = true;
			m_clock.restart();
		}
	}

	void FixedClock::Stop() {
		if (m_running) {
			m_running = false;
			m_stoppedTime = m_clock.getElapsedTime();
		}
	}

	void FixedClock::Restart() {
		m_stoppedTime = sf::Time::Zero;
		m_running = false;
		Start();
	}

	sf::Time FixedClock::GetElapsedTime() const {
		if (m_running)
			return m_stoppedTime + m_clock.getElapsedTime();

		return m_stoppedTime;
	}
}