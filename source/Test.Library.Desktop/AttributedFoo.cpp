#include "pch.h"
#include "AttributedFoo.h"
#include <Datum.h>


namespace Library
{
	Datum::DatumValues* storage;
	AttributedFoo::AttributedFoo():
		Attributed(), mInt(0), mFloat(0.0f), mVector(0), mMatrix(0), mString(""), mScope()
	{
		init();
	}

	AttributedFoo::~AttributedFoo()
	{
		delete[] iArray;
		delete[] fArray;
		delete[] vArray;
		delete[] mArray;
		delete[] sArray;
	}

	AttributedFoo::AttributedFoo(const AttributedFoo& rhs) :
		Attributed(rhs)
	{
		operator=(rhs);
	}

	AttributedFoo& AttributedFoo::operator=(const AttributedFoo& rhs)
	{
		init();

		(*this)["External Integer"].setStorage(&mInt, 1u);
		(*this)["External Float"].setStorage(&mFloat, 1u);
		(*this)["External Vector"].setStorage(&mVector, 1u);
		(*this)["External Matrix"].setStorage(&mMatrix, 1u);
		(*this)["External String"].setStorage(&mString, 1u);

		(*this)["External Integer Array"].setStorage(iArray, 10u);
		(*this)["External Float Array"].setStorage(fArray, 10u);
		(*this)["External Vector Array"].setStorage(vArray, 10u);
		(*this)["External Matrix Array"].setStorage(mArray, 10u);
		(*this)["External String Array"].setStorage(sArray, 10u);

		mInt = rhs.mInt;
		mFloat = rhs.mFloat;
		mVector = rhs.mVector;
		mMatrix = rhs.mMatrix;
		mString = rhs.mString;
		mScope = rhs.mScope;

		for (std::uint32_t i = 0; i < 10; i++)
		{
			iArray[i] = rhs.iArray[i];
			fArray[i] = rhs.fArray[i];
			vArray[i] = rhs.vArray[i];
			mArray[i] = rhs.mArray[i];
			sArray[i] = rhs.sArray[i];
		}

		return (*this);
	}

	AttributedFoo::AttributedFoo(AttributedFoo&& rhs):
		Attributed::Attributed(std::move(rhs))
	{
		operator=(std::move(rhs));
	}

	AttributedFoo& AttributedFoo::operator=(AttributedFoo&& rhs)
	{
		(*this)["External Integer"].setStorage(&mInt, 1u);
		(*this)["External Float"].setStorage(&mFloat, 1u);
		(*this)["External Vector"].setStorage(&mVector, 1u);
		(*this)["External Matrix"].setStorage(&mMatrix, 1u);
		(*this)["External String"].setStorage(&mString, 1u);

		(*this)["External Integer Array"].setStorage(iArray, 10u);
		(*this)["External Float Array"].setStorage(fArray, 10u);
		(*this)["External Vector Array"].setStorage(vArray, 10u);
		(*this)["External Matrix Array"].setStorage(mArray, 10u);
		(*this)["External String Array"].setStorage(sArray, 10u);

		mInt = rhs.mInt;
		mFloat = rhs.mFloat;
		mVector = rhs.mVector;
		mMatrix = rhs.mMatrix;
		mString = rhs.mString;
		mScope = rhs.mScope;

		iArray = rhs.iArray;
		fArray = rhs.fArray;
		vArray = rhs.vArray;
		mArray = rhs.mArray;
		sArray = rhs.sArray;

		rhs.mInt = 0;
		rhs.mFloat = 0.0f;
		rhs.mVector = glm::vec4(0);
		rhs.mMatrix = glm::mat4(0);
		rhs.mString = "";
		rhs.iArray = nullptr;
		rhs.fArray = nullptr;
		rhs.vArray = nullptr;
		rhs.mArray = nullptr;
		rhs.sArray = nullptr;

		return (*this);
	}

	void AttributedFoo::init()
	{
		mPrescribedAttributes.pushBack(
			Signature("External Integer", DatumType::Integer, &mInt, 1u, &mInt)
		);

		mPrescribedAttributes.pushBack(
			Signature("External Float", DatumType::Float, &mFloat, 1u, &mFloat)
		);

		mPrescribedAttributes.pushBack(
			Signature("External Vector", DatumType::Vector, &mVector, 1u, &mVector)
		);

		mPrescribedAttributes.pushBack(
			Signature("External Matrix", DatumType::Matrix, &mMatrix, 1u, &mMatrix)
		);

		mPrescribedAttributes.pushBack(
			Signature("External String", DatumType::String, &mString, 1u, &mString)
		);

		mPrescribedAttributes.pushBack(
			Signature("Internal Scope", DatumType::Scope, &mScope, 1u, &mScope)
		);

		iArray = new std::int32_t[10];
		fArray = new float[10];
		vArray = new glm::vec4[10];
		mArray = new glm::mat4[10];
		sArray = new std::string[10];

		for (std::uint32_t i = 0; i < 10; i++)
		{
			iArray[i] = 10;
			fArray[i] = 10.0f;
		}

		mPrescribedAttributes.pushBack(
			Signature("External Integer Array", DatumType::Integer, iArray, 10u, iArray)
		);

		mPrescribedAttributes.pushBack(
			Signature("External Float Array", DatumType::Float, fArray, 10u, fArray)
		);

		mPrescribedAttributes.pushBack(
			Signature("External Vector Array", DatumType::Vector, vArray, 10u, vArray)
		);

		mPrescribedAttributes.pushBack(
			Signature("External Matrix Array", DatumType::Matrix, mArray, 10u, mArray)
		);

		mPrescribedAttributes.pushBack(
			Signature("External String Array", DatumType::String, sArray, 10u, sArray)
		);

		populate();
	}
}
