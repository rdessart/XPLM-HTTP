#pragma once
#include <string>
#include <variant>
#include <vector>
#include <nlohmann/json.hpp>
#include <XPLM/XPLMDataAccess.h>
#include <DataRefs/DataRefType.hpp>
#include <DataRefs/DataRefValue.hpp>

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

	bool setValue(json value) const;
	bool getValue(DataRefValue& outValue) const;

protected:
	DataRefType determineType(XPLMDataRef dataRef) const;
protected:
	DataRefType mType = DataRefType::Undefined;
	std::string mLink = "";
	bool mIsReadOnly = true;
	bool mIsValid = false;
	XPLMDataRef mDataRef = nullptr;
};

