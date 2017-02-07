#include "pch.h"
#include "Datum.h"

namespace Datum
{
	/// Constructor
	Datum::Datum():
		mType(DatumType::Unknown), mCapacity(13), mSize(0), mDataIsExternal(false)
	{}

	/// Overloaded constructor
	/// @Param type: The type of the Datum object
	Datum::Datum(DatumType type) :
		mType(type), mCapacity(13), mSize(0), mDataIsExternal(false)
	{}

	/// Destructor
	Datum::~Datum()
	{}

	/// Copy constructor
	/// @Param rhs: Datum object being copied
	Datum::Datum(const Datum& rhs):
		mType(rhs.mType), mData(rhs.mData), mCapacity(rhs.mCapacity), mSize(rhs.mSize), mDataIsExternal(rhs.mDataIsExternal)
	{}

	/// Move copy constructor
	/// @Param rhs: Datum object being copied
	Datum::Datum(Datum&& rhs) :
		mType(rhs.mType), mData(rhs.mData), mCapacity(rhs.mCapacity), mSize(rhs.mSize), mDataIsExternal(rhs.mDataIsExternal)
	{
		switch (rhs.mType)
		{
			case DatumType::Integer:
				if (rhs.mData.i != nullptr) free(rhs.mData.i);
				break;
			case DatumType::Float:
				if (rhs.mData.f != nullptr) free(rhs.mData.f);
				break;
			case DatumType::String:
				if (rhs.mData.s != nullptr) free(rhs.mData.s);
				break;
			case DatumType::Pointer:
				if (rhs.mData.r != nullptr) free(rhs.mData.r);
				break;
			default: 
				break;
		}
		rhs.mType = DatumType::Unknown;
		rhs.mCapacity = NULL;
		rhs.mSize = NULL;
	}

	/// Datum assignment operator
	/// @Param rhs: Datum object being assigned to
	/// @Return; The newly copied Datum object
	Datum& Datum::operator=(const Datum& rhs)
	{
		// TODO: Implement datum assignment operator
		UNREFERENCED_PARAMETER(rhs);
		return *this;
	}

	/// Datum move assignment operator
	/// @Param rhs: Datum object being assigned to
	/// @Return: The newly copied Datum object
	Datum& Datum::operator=(Datum&& rhs)
	{
		// TODO: Implement Datum move assignment operator
		UNREFERENCED_PARAMETER(rhs);
		return *this;
	}

	/// Type assignment operator
	/// @Param rhs: DatumType being assigned to
	/// @Return: The newly assigned Datum object
	/// @Exception: Thrown if attempting to reassign a new type to a Datum object
	Datum& Datum::operator=(const DatumType& rhs)
	{
		setType(rhs);
		return *this;
	}

	/// Assignment operator for std::int32_t
	/// @Param rhs: Integer being assigned to
	/// @Return: The newly assigned Datum object
	/// @Exception: Thrown if attempting to assign to invalid Datum type or if size is greater than 1
	Datum& Datum::operator=(const std::int32_t& rhs)
	{
		if (mType != DatumType::Integer || mSize > 1) throw std::exception("Invalid assignment invocation");
		mData.i[0] = rhs;
		return *this;
	}

	/// Assignment operator for float
	/// @Param rhs: Float being assigned to
	/// @Return: The newly assigned Datum object
	/// @Exception: Thrown if attempting to assign to invalid Datum type or if size is greater than 1
	Datum& Datum::operator=(const float& rhs)
	{
		if (mType != DatumType::Float || mSize > 1) throw std::exception("Invalid assignment invocation");
		mData.f[0] = rhs;
		return *this;
	}

	/// Assignment operator for std::string
	/// @Param rhs: String being assigned to
	/// @Return: The newly assigned Datum object
	/// @Exception: Thrown if attempting to assign to invalid Datum type or if size is greater than 1
	Datum& Datum::operator=(const std::string& rhs)
	{
		if (mType != DatumType::String || mSize > 1) throw std::exception("Invalid assignment invocation");
		mData.s[0] = rhs;
		return *this;
	}

	/// Assignment operator for float
	/// @Param rhs: Float being assigned to
	/// @Return: The newly assigned Datum object
	/// @Exception: Thrown if attempting to assign to invalid Datum type or if size is greater than 1
	Datum& Datum::operator=(Library::RTTI* const& rhs)
	{
		if (mType != DatumType::Pointer || mSize > 1) throw std::exception("Invalid assignment invocation");
		mData.r[0] = rhs;
		return *this;
	}

	/// Comparison operator for DatumType value
	/// @Param rhs: The DatumType being compared against
	/// @Return: True if the types are the same
	bool Datum::operator==(const DatumType& rhs) const
	{
		return mType == rhs;
	}

	/// Comparison operator for std::int32_t
	/// @Param rhs: The signed integer being compared against
	/// @Return: True if the integer values are the same. False if size is not 1 or if type is invalid
	bool Datum::operator==(const std::int32_t& rhs) const
	{
		return mType == DatumType::Integer &&
			mSize == 1 && mData.i[0] == rhs;
	}

	/// Comparison operator for float
	/// @Param rhs: The float being compared against
	/// @Return: True if the float values are the same. False if size is not 1 or if type is invalid
	bool Datum::operator==(const float& rhs) const
	{
		return mType == DatumType::Float &&
			mSize == 1 && mData.f[0] == rhs;
	}

	/// Comparison operator for std::string
	/// @Param rhs: The string being compared against
	/// @Return: True if the string values are the same. False if size is not 1 or if type is invalid
	bool Datum::operator==(const std::string& rhs) const
	{
		return mType == DatumType::String &&
			mSize == 1 && mData.s[0] == rhs;
	}

	/// Comparison operator for RTTI pointers
	/// @Param rhs: The RTTI pointer being compared against
	/// @Return: True if the pointers are the same. False if size is not 1 or if type is invalid
	bool Datum::operator==(const Library::RTTI*& rhs) const
	{
		return mType == DatumType::Pointer &&
			mSize == 1 && mData.r[0] == rhs;
	}

	/// Inequality operator for DatumType
	/// @Param: The DatumType being compared against
	/// @Return: True if the types are not equivalent
	bool Datum::operator!=(const DatumType& rhs) const
	{
		return !(operator==(rhs));
	}

	/// Inequality operator for std::int32_t
	/// @Param: The integer being compared against
	/// @Return: True if the integers are not equivalent
	bool Datum::operator!=(const std::int32_t& rhs) const
	{
		return !(operator==(rhs));
	}

	/// Inequality operator for float
	/// @Param: The float being compared against
	/// @Return: True if the floats are not equivalent
	bool Datum::operator!=(const float& rhs) const
	{
		return !(operator==(rhs));
	}

	/// Inequality operator for std::string
	/// @Param: The string being compared against
	/// @Return: True if the strings are not equivalent
	bool Datum::operator!=(const std::string& rhs) const
	{
		return !(operator==(rhs));
	}

	/// Inequality operator for RTTI pointer
	/// @Param: The RTTI pointer being compared against
	/// @Return: True if the pointers are not equivalent
	bool Datum::operator!=(const Library::RTTI*& rhs) const
	{
		return !(operator==(rhs));
	}

	/// The type associated with this object
	/// @Return: The DatumType of this Datum object
	DatumType Datum::type() const
	{
		return mType;
	}

	/// Set the DatumType of the Datum object
	/// @Param type: The new type of the Datum object
	/// @Exception: Thrown if attempting to reassign a new type to a Datum object
	void Datum::setType(const DatumType& type)
	{
		if (mType == DatumType::Unknown || mType == type)  mType = type;
		else throw std::exception("Attempting to change type on Datum object");
	}

	/// The number of values in the Datum object
	/// @Return: The number of items associated with this Datum object
	std::uint32_t Datum::size() const
	{
		return mSize;
	}

	/// Set number of values and reserve memory if needed
	/// @Param size: the new number of values
	void Datum::setSize(std::uint32_t size)
	{
		// TODO: Implement setSize method
		UNREFERENCED_PARAMETER(size);
	}

	/// Clear the array without shrinking the capacity
	void Datum::clear()
	{
		// TODO: Implement clear method
	}

	/// Sets the external storage to the specified pointer
	/// @Param data: The external storage being utilized
	/// @Pram size: The number of elements in the external storage
	void Datum::setStorage(std::int32_t* data, std::uint32_t size)
	{
		if (mType != DatumType::Unknown) throw std::exception("Attempting to reassign Datum Type");
		mData.i = data;
		mSize = size;
	}

	/// Sets the external storage to the specified pointer
	/// @Param data: The external storage being utilized
	/// @Pram size: The number of elements in the external storage
	void Datum::setStorage(float* data, std::uint32_t size)
	{
		if (mType != DatumType::Unknown) throw std::exception("Attempting to reassign Datum Type");
		mData.f = data;
		mSize = size;
	}

	/// Sets the external storage to the specified pointer
	/// @Param data: The external storage being utilized
	/// @Pram size: The number of elements in the external storage
	void Datum::setStorage(std::string* data, std::uint32_t size)
	{
		if (mType != DatumType::Unknown) throw std::exception("Attempting to reassign Datum Type");
		mData.s = data;
		mSize = size;
	}

	/// Sets the external storage to the specified pointer
	/// @Param data: The external storage being utilized
	/// @Pram size: The number of elements in the external storage
	void Datum::setStorage(Library::RTTI** data, std::uint32_t size)
	{
		if (mType != DatumType::Unknown) throw std::exception("Attempting to reassign Datum Type");
		mData.r = data;
		mSize = size;
	}

	/// Sets the value of a given index of the data
	/// @Param value: The value being assigned
	/// @Param index: The index of the value being assigned to
	void Datum::set(const std::int32_t& value, const std::uint32_t index) const
	{
		if (mType != DatumType::Integer) throw std::exception("Calling set on invalid type");
		mData.i[index] = value;
	}

	/// Sets the value of a given index of the data
	/// @Param value: The value being assigned
	/// @Param index: The index of the value being assigned to
	void Datum::set(const float& value, const std::uint32_t index) const
	{
		if (mType != DatumType::Float) throw std::exception("Calling set on invalid type");
		mData.f[index] = value;
	}

	/// Sets the value of a given index of the data
	/// @Param value: The value being assigned
	/// @Param index: The index of the value being assigned to
	void Datum::set(const std::string& value, const std::uint32_t index) const
	{
		if (mType != DatumType::String) throw std::exception("Calling set on invalid type");
		mData.s[index] = value;
	}

	/// Sets the value of a given index of the data
	/// @Param value: The value being assigned
	/// @Param index: The index of the value being assigned to
	void Datum::set(Library::RTTI* const value, const std::uint32_t index) const
	{
		if (mType != DatumType::Pointer) throw std::exception("Calling set on invalid type");
		mData.r[index] = value;
	}

	template<>
	std::int32_t& Datum::get(const std::uint32_t index)
	{
		if (mType != DatumType::Integer) throw std::exception("Calling get on invalid type");
		if (mData.i == nullptr) throw std::exception("Attempting to dereference nullptr");
		if (index >= mSize) throw std::exception("Accessing beyond array bounds");
		return mData.i[index];
		
	}

	template<>
	float& Datum::get(const std::uint32_t index)
	{
		if (mType != DatumType::Float) throw std::exception("Calling get on invalid type");
		if (mData.f == nullptr) throw std::exception("Attempting to dereference nullptr");
		if (index >= mSize) throw std::exception("Accessing beyond array bounds");
		return mData.f[index];
		
	}

	template<>
	std::string& Datum::get(const std::uint32_t index)
	{
		if (mType != DatumType::String) throw std::exception("Calling get on invalid type");
		if (mData.s == nullptr) throw std::exception("Attempting to dereference nullptr");
		if (index >= mSize) throw std::exception("Accessing beyond array bounds");
		return mData.s[index];
	}

	template<>
	Library::RTTI*& Datum::get(const std::uint32_t index)
	{
		if (mType != DatumType::Pointer) throw std::exception("Calling get on invalid type");
		if (mData.r == nullptr) throw std::exception("Attempting to dereference nullptr");
		if (index >= mSize) throw std::exception("Accessing beyond array bounds");
		return mData.r[index];
	}

	/// Reserves the number of elements in the local buffer
	void Datum::reserve(std::uint32_t capacity)
	{
		UNREFERENCED_PARAMETER(capacity);
	}
}
