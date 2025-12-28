#pragma once
#include <string>
#include <variant>
#include <vector>

#include <XPLM/XPLMDataAccess.h>
#include <DataRefType.hpp>
#include <DataRefValue.hpp>

class DataRef
{
public:
	DataRef(const std::string& link);
	DataRef(const std::string& link, DataRefType type);

	DataRefType getType() const { return mType; }
	std::string getLink() const { return mLink; }
	bool isReadOnly() const { return mIsReadOnly; }
	bool isValid() const { return mIsValid; }
	XPLMDataRef getDataRef() const { return mDataRef; }

	bool setValue(DataRefValue value) const;
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

