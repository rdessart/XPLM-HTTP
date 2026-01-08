#include "DataRef.hpp"
#include <algorithm>

DataRef::DataRef(const std::string& link) :
	DataRef(link, DataRefType::Undefined)
{
}

DataRef::DataRef(const std::string& link, DataRefType type):
	mLink(link)
{
	mDataRef = XPLMFindDataRef(link.c_str());
	if (mDataRef == nullptr) return;
	mType = (type == DataRefType::Undefined) ? determineType(mDataRef) : type;
	mIsValid = true;
	mIsReadOnly = !XPLMCanWriteDataRef(mDataRef);
}

bool DataRef::setValue(json const value) const
{
	if (mIsReadOnly) return false;
	switch (mType)
	{
	case DataRefType::Int:
	{
		XPLMSetDatai(mDataRef, value.get<int>());
		break;
	}
	case DataRefType::Float:
	{
		XPLMSetDataf(mDataRef, value.get<float>());
		break;
	}
	case DataRefType::Double:
	{
		XPLMSetDatad(mDataRef, value.get<double>());
		break;
	}
	default:
		return false;
	};
	return true;
}

bool DataRef::getValue(json& outValue) const
{
	switch (mType)
	{
	case DataRefType::Int:
	{
		outValue = XPLMGetDatai(mDataRef);
		break;
	}
	case DataRefType::Float:
	{
		outValue = XPLMGetDataf(mDataRef);
		break;
	}
	case DataRefType::Double:
	{
		outValue = XPLMGetDatad(mDataRef);
		break;
	}
	default:
		return false;
	};
	return true;
}

DataRefType DataRef::StringToType(const std::string& input)
{
	std::string output(input);
	std::transform(input.begin(), input.end(), output.begin(), ::toupper);

	if (input == "INT") return DataRefType::Int;
	if (input == "FLOAT") return DataRefType::Float;
	if (input == "DOUBLE") return DataRefType::Double;
	if (input == "FLOAT_ARRAY") return DataRefType::FloatArray;
	if (input == "INT_ARRAY") return DataRefType::IntArray;
	if (input == "DATA") return DataRefType::Data;

	return DataRefType::Undefined;
}

std::string DataRef::TypeToString(const DataRefType type)
{
	switch (type)
	{
	case DataRefType::Undefined:
		return "undefined";
	case DataRefType::Int:
		return "int";
	case DataRefType::Float:
		return "int";
	case DataRefType::Double:
		return "int";
	case DataRefType::FloatArray:
		return "int";
	case DataRefType::IntArray:
		return "int";
	case DataRefType::Data:
		return "int";
	}
}

DataRefType DataRef::determineType(XPLMDataRef dataRef) const
{
	XPLMDataTypeID type = XPLMGetDataRefTypes(dataRef);
	switch (type)
	{
		case xplmType_Int:
			return DataRefType::Int;
		case xplmType_Float:
			return DataRefType::Float;
		case xplmType_Double:
			return DataRefType::Double;
		case xplmType_FloatArray:
			return DataRefType::FloatArray;
		case xplmType_IntArray:
			return DataRefType::IntArray;
		case xplmType_Data:
			return DataRefType::Data;
		case (xplmType_Double | xplmType_Float) :
			return DataRefType::Double; // Prefer Double in case of multiple types
	default:
		break;
	}
	return DataRefType::Undefined;
}
