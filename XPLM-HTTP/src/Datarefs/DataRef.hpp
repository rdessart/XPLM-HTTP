#pragma once
#include <string>
#include <variant>
#include <vector>
#include <nlohmann/json.hpp>
#include <XPLM/XPLMDataAccess.h>

enum class DataRefType
{
	Undefined = xplmType_Unknown,
	Int = xplmType_Int,
	Float = xplmType_Float,
	Double = xplmType_Double,
	FloatArray = xplmType_FloatArray,
	IntArray = xplmType_IntArray,
	Data = xplmType_Data
};

using DataRefValue = std::variant<
	int,
	float,
	double,
	std::vector<float>,
	std::vector<int>,
	std::vector<uint8_t>>;

using json = nlohmann::json;

class DataRef
{
public:
	DataRef() = delete;
	DataRef(const std::string& link);
	DataRef(const std::string& link, DataRefType type);

	DataRefType getType() const { return mType; }
	std::string getLink() const { return mLink; }
	bool isReadOnly() const { return mIsReadOnly; }
	bool isValid() const { return mIsValid; }
	XPLMDataRef getDataRef() const { return mDataRef; }

	//bool setValue(DataRefValue const value) const;
	bool setValue(json const value) const;
	//bool getValue(DataRefValue& outValue) const;
	bool getValue(json& outValue) const;

	static DataRefType StringToType(const std::string& input);
	static std::string TypeToString(const DataRefType type);

protected:
	DataRefType determineType(XPLMDataRef dataRef) const;
protected:
	DataRefType mType = DataRefType::Undefined;
	std::string mLink = "";
	bool mIsReadOnly = true;
	bool mIsValid = false;
	XPLMDataRef mDataRef = nullptr;
};

