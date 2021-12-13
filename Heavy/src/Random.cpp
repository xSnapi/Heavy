#include <hvpch.h>
#include "Random.hpp"

namespace hv {
	Random& Random::Get() { return s_instance; }

	int Random::Int(int min, int max) { return std::uniform_int_distribution<int>(min, max)(m_randomEngine); }

	float Random::Float(float min, float max) { return std::uniform_real_distribution<float>(min, max)(m_randomEngine); }

	double Random::Double(double min, double max) { return std::uniform_real_distribution<double>(min, max)(m_randomEngine); }

	Random::Random() {
		m_randomEngine.seed(std::random_device()());
	}

	Random::~Random() {

	}

	Random Random::s_instance;
}