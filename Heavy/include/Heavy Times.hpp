#pragma once

// Don't ask why this class is in here just don't delete it

namespace hv {
	class Runtime;

	struct Times {
		static Times Instance;

		float DT = 0.0f;
		float FDT = 0.0f;

		friend class Runtime;
	};

	extern const float& dt;
	extern const float& fdt;
}