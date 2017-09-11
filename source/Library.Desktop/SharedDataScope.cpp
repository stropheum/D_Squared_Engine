#include "pch.h"
#include "SharedDataScope.h"
#include "Scope.h"


namespace Library
{
	RTTI_DEFINITIONS(SharedDataScope)

	SharedDataScope::SharedDataScope():
		SharedData(), mScope(nullptr)
	{
	}

	SharedDataScope::~SharedDataScope()
	{
		delete mScope;
	}

	XmlParseMaster::SharedData* SharedDataScope::Clone()
	{
		SharedDataScope* clone = new SharedDataScope();
		if (mScope != nullptr)
		{
			clone->mScope = new Scope(*mScope);
		}
		return clone;
	}

	/// RTTI ToString representation of the shared data class
	/// @Return: A string representation of the class Name
	std::string SharedDataScope::ToString() const
	{
		return "Shared Data Scope";
	}

	/// RTTI Equals comparison
	/// @Param rhs: The RTTI object being compared against
	/// @Return: True if the objects are shared data and identical pointers
	bool SharedDataScope::Equals(const RTTI* rhs) const
	{
		if (this == rhs) { return true; }
		if (rhs == nullptr) { return false; }
		SharedDataScope* data = rhs->As<SharedDataScope>();
		if (data == nullptr) { return false; }
		return
			(mScope == data->mScope) ||
			(mScope != nullptr) && 
			(data->mScope != nullptr) &&
			(*mScope == *data->mScope);
	}
}
