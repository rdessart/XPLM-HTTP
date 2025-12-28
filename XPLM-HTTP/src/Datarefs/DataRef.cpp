#include "DataRef.h"

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

bool DataRef::setValue(DataRefValue value) const
{
	if (mIsReadOnly) return false;
	switch (mType)
	{
		case DataRefType::Int:
		{
			if (int* val = std::get_if<int>(&value))
			{
				XPLMSetDatai(mDataRef, *val);
				return true;
			}
			break;
		}
		case DataRefType::Float:
		{
			if (float* val = std::get_if<float>(&value))
			{
				XPLMSetDataf(mDataRef, *val);
				return true;
			}
			break;
		}
		case DataRefType::Double:
		{
			if (double* val = std::get_if<double>(&value))
			{
				XPLMSetDatad(mDataRef, *val);
				return true;
			}
			break;
		}
		default:
			break;
	};
	return false;
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
		break;
	};
	return false;
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
}
