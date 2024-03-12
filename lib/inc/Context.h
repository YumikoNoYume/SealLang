#pragma once

#include <unordered_map>
#include <string>

#include "Value.h"

namespace seal {
	struct Context {
		std::unordered_map<std::string, Value> variables;
	};
}

