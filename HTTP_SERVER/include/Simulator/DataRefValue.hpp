#pragma once

#include <string>
#include <variant>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

using DataRefValue = std::variant<
	int,
	float,
	double,
	std::vector<float>,
	std::vector<int>,
	std::vector<uint8_t>>;


static inline json DataRefValueToJson(const DataRefValue& value)
{
	if (const int* ival = std::get_if<int>(&value))
	{
		return json(*ival);
	}
	else if (const float* fval = std::get_if<float>(&value))
	{
		return json(*fval);
	}
	else if (const double* dval = std::get_if<double>(&value))
	{
		return json(*dval);
	}
	else if (const std::vector<float>* fvec = std::get_if<std::vector<float>>(&value))
	{
		return json(*fvec);
	}
	else if (const std::vector<int>* ivec = std::get_if<std::vector<int>>(&value))
	{
		return json(*ivec);
	}
	else if (const std::vector<uint8_t>* u8vec = std::get_if<std::vector<uint8_t>>(&value))
	{
		return json(*u8vec);
	}
	return json();
};

//static inline std::optional<DataRefValue> JsonToValue(DataRefType t, const json& j)
//{
//	switch (t)
//	{
//	case DataRefType::Int: return j.get<int>();
//	case DataRefType::Float: return j.get<float>();
//	case DataRefType::Double: return j.get<double>();
//	case DataRefType::Data:
//		return j.get<std::vector<uint8_t>>();
//	case DataRefType::IntArray:
//		return j.get<std::vector<int>>();
//	case DataRefType::FloatArray:
//		return j.get<std::vector<float>>();
//	default:
//		break;
//	}
//	throw std::runtime_error("Invalid type");
//	return std::nullopt;
//}