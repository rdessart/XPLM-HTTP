#include "DataRef.hpp"



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

bool DataRef::setValue(json value) const
{
	if (mIsReadOnly) return false;
	switch (mType)
	{
		case DataRefType::Int:
		{
			int val = value.get<int>();
			XPLMSetDatai(mDataRef, val);
			return true;
		}
		case DataRefType::Float:
		{
			float val = value.get<float>();
			XPLMSetDataf(mDataRef, val);
			return true;
		}
		case DataRefType::Double:
		{
			double val = value.get<double>();
			XPLMSetDatad(mDataRef, val);
			return true;
		}
		default:
			return false;
	};
	return true;
}




bool DataRef::getValue(DataRefValue& outValue) const
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
