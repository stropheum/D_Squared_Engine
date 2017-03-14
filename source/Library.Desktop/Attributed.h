#pragma once
#include "Scope.h"

namespace Library
{
	class Attributed : public Scope
	{
		RTTI_DECLARATIONS(Attributed, Scope)

	public:

#pragma region Constructors, Copy/Move semantics

		/// Constructor
		Attributed();
		/// Virtual destructor to ensure that class is abstract
		virtual ~Attributed(){};

		/// Copy constructor
		/// @Param rhs: The Attributed object being copied
		Attributed(const Attributed& rhs);
		/// Assignment Operator
		/// @Param rhs: The attributed object being assigned to
		Attributed& operator=(const Attributed& rhs);

		/// Move copy constructor
		/// @Param rhs: The attributed object being copied
		Attributed(Attributed&& rhs);
		/// Move assignment operator
		/// @Param rhs; The attributed object being assigned to
		Attributed& operator=(Attributed&& rhs);

#pragma endregion

#pragma region Public Methods

		/// Populates the attributes
		void populate();

		/// Determines if the specified string is a prescribed attribute
		/// @Param name: The attribute being checked
		/// @Return: True if the attribute is a prescribed attribute
		bool isPrescribedAttribute(std::string name) const;

		/// Determines if the specified string is an auxiliary attribute
		/// @Param name: The attribute being checked
		/// @Return: True if the attribute is an auxiliary attribute
		bool isAuxiliaryAttribute(std::string name) const;

		/// Determines if the specified string is an attribute
		/// @Param name: The attribute being checked
		/// @Return: True if the attribute is an attribute
		bool isAttribute(std::string name) const;

		/// Adds an auxiliary attribute to the current collection
		/// @Param name: The name of the new auxiliary attribute
		/// @Return: A reference to the Datum created from the appended attribute
		Datum& appendAuxiliaryAttribute(std::string name);

#pragma endregion

	protected:
#pragma region Signature
	
		class Signature
		{
		public:
			/// Constructor For the integer signature type
			/// @Param name: The name of the signature
			/// @Param type: The type of data being passed in
			/// @Param initialValue: The initial value of the signature's data
			/// @Param size: The number of elements in the data
			/// @Param storage: The pointer to the external storage
			/// There are constructor overloads for each type of data, where InitialValue and storage will be set uniquely
			Signature(const std::string name, const DatumType type, std::int32_t* const initialValue,
				const std::uint32_t size, std::int32_t* storage):
				Name(name), Type(type), Size(size)
			{
				InitialValue.i = initialValue;
				if (storage != nullptr)
				{
					Storage.i = storage;
					if (InitialValue.i != nullptr)
					{
						for (std::uint32_t i = 0; i < Size; i++)
						{
							Storage.i[i] = InitialValue.i[i];
						}
					}
				}
			}

			/// Constructor For the float signature type
			/// @Param name: The name of the signature
			/// @Param type: The type of data being passed in
			/// @Param initialValue: The initial value of the signature's data
			/// @Param size: The number of elements in the data
			/// @Param storage: The pointer to the external storage
			/// There are constructor overloads for each type of data, where InitialValue and storage will be set uniquely
			Signature(const std::string name, const DatumType type, float* const initialValue,
				const std::uint32_t size, float* const storage) :
				Name(name), Type(type), Size(size)
			{
				InitialValue.f = initialValue;
				if (storage != nullptr)
				{
					Storage.f = storage;
					if (InitialValue.f != nullptr)
					{
						for (std::uint32_t i = 0; i < Size; i++)
						{
							Storage.f[i] = *InitialValue.f;
						}
					}
				}
			}

			/// Constructor For the vector signature type
			/// @Param name: The name of the signature
			/// @Param type: The type of data being passed in
			/// @Param initialValue: The initial value of the signature's data
			/// @Param size: The number of elements in the data
			/// @Param storage: The pointer to the external storage
			/// There are constructor overloads for each type of data, where InitialValue and storage will be set uniquely
			Signature(const std::string name, const DatumType type, glm::vec4* const initialValue,
				const std::uint32_t size, glm::vec4* const storage) :
				Name(name), Type(type), Size(size)
			{
				InitialValue.v = initialValue;
				if (storage != nullptr)
				{
					Storage.v = storage;
					if (InitialValue.v != nullptr)
					{
						for (std::uint32_t i = 0; i < Size; i++)
						{
							Storage.v[i] = InitialValue.v[i];
						}
					}
				}
			}

			/// Constructor For the matrix signature type
			/// @Param name: The name of the signature
			/// @Param type: The type of data being passed in
			/// @Param initialValue: The initial value of the signature's data
			/// @Param size: The number of elements in the data
			/// @Param storage: The pointer to the external storage
			/// There are constructor overloads for each type of data, where InitialValue and storage will be set uniquely
			Signature(const std::string name, const DatumType type, glm::mat4* const initialValue,
				const std::uint32_t size, glm::mat4* const storage) :
				Name(name), Type(type), Size(size)
			{
				InitialValue.m = initialValue;
				if (storage != nullptr)
				{
					Storage.m = storage;
					if (InitialValue.m != nullptr)
					{
						for (std::uint32_t i = 0; i < Size; i++)
						{
							Storage.m[i] = InitialValue.m[i];
						}
					}
				}
			}

			/// Constructor For the string signature type
			/// @Param name: The name of the signature
			/// @Param type: The type of data being passed in
			/// @Param initialValue: The initial value of the signature's data
			/// @Param size: The number of elements in the data
			/// @Param storage: The pointer to the external storage
			/// There are constructor overloads for each type of data, where InitialValue and storage will be set uniquely
			Signature(const std::string name, const DatumType type, std::string* const initialValue,
				const std::uint32_t size, std::string* const storage) :
				Name(name), Type(type), Size(size)
			{
				InitialValue.s = initialValue;
				if (storage != nullptr)
				{
					Storage.s = storage;
					if (InitialValue.s != nullptr)
					{
						for (std::uint32_t i = 0; i < Size; i++)
						{
							Storage.s[i] = InitialValue.s[i];
						}
					}
				}
			}

			/// Constructor For the scope signature type
			/// @Param name: The name of the signature
			/// @Param type: The type of data being passed in
			/// @Param initialValue: The initial value of the signature's data
			/// @Param size: The number of elements in the data
			/// @Param storage: The pointer to the external storage
			/// There are constructor overloads for each type of data, where InitialValue and storage will be set uniquely
			Signature(const std::string name, const DatumType type, Scope** const initialValue,
				const std::uint32_t size, Scope** const storage):
				Name(name), Type(type), Size(size)
			{
				InitialValue.sc = initialValue;
				if (storage != nullptr)
				{
					Storage.sc = storage;
					if (InitialValue.sc != nullptr)
					{
						for (std::uint32_t i = 0; i < Size; i++)
						{
							Storage.sc[i] = InitialValue.sc[i];
						}
					}
				}
			}

			/// Constructor For the RTTI pointer signature type
			/// @Param name: The name of the signature
			/// @Param type: The type of data being passed in
			/// @Param initialValue: The initial value of the signature's data
			/// @Param size: The number of elements in the data
			/// @Param storage: The pointer to the external storage
			/// There are constructor overloads for each type of data, where InitialValue and storage will be set uniquely
			Signature(const std::string name, const DatumType type, Library::RTTI** initialValue,
				const std::uint32_t size, RTTI** const storage) :
				Name(name), Type(type), Size(size)
			{
				InitialValue.r = initialValue;
				if (storage != nullptr)
				{
					Storage.r = storage;
					if (InitialValue.r != nullptr)
					{
						for (std::uint32_t i = 0; i < Size; i++)
						{
							Storage.r[i] = InitialValue.r[i];
						}
					}
				}
			}

			std::string Name;
			DatumType Type;
			Datum::DatumValues InitialValue;
			std::uint32_t Size;
			Datum::DatumValues Storage;
		};

#pragma endregion

	private:
#pragma region Private Methods

		Signature& getSignature(const std::string& name);

#pragma endregion

	protected:
#pragma region Protected Variables

		/// This vector is used for populating prescribed attributes in children of attributed
		Vector<Signature> mPrescribedAttributes;

		/// This vector is used for populating auxiliary attributes in children of Attributed
		Vector<Signature> mAuxiliaryAttributes;
#pragma endregion

	};
}


