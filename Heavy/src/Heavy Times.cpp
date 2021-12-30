#include <hvpch.h>
#include "Heavy Times.hpp"

namespace hv {
	Times Times::Instance;

	const float& dt  = Times::Instance.DT;
	const float& fdt = Times::Instance.FDT;
}