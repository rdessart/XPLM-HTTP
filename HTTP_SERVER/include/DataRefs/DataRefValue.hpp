#pragma once

#include <string>
#include <variant>
#include <vector>

using DataRefValue = std::variant<
	int,
	float,
	double,
	std::vector<float>,
	std::vector<int>,
	std::vector<uint8_t>>;
