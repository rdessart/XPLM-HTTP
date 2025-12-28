#pragma once

enum class DataRefType {
	Undefined = 0,
	Int = 1,
	Float = 2,
	Double = 4,
	FloatArray = 8,
	IntArray = 16,
	Data = 32,
};

static inline DataRefType DataRefTypeFromString(const std::string& typeStr) {
	if (typeStr == "int") {
		return DataRefType::Int;
	} else if (typeStr == "float") {
		return DataRefType::Float;
	} else if (typeStr == "double") {
		return DataRefType::Double;
	} else if (typeStr == "float_array") {
		return DataRefType::FloatArray;
	} else if (typeStr == "int_array") {
		return DataRefType::IntArray;
	} else if (typeStr == "data") {
		return DataRefType::Data;
	} else {
		return DataRefType::Undefined;
	}
}