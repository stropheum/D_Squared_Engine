#include "pch.h"
#include "XmlParseHelperEntity.h"
#include "SharedDataScope.h"
#include "Scope.h"
#include <sstream>
#include "World.h"
#include "Sector.h"
#include "Entity.h"
#include "Action.h"
#include "ActionListIf.h"


namespace Library
{
	RTTI_DEFINITIONS(XmlParseHelperEntity)

		XmlParseHelperEntity::XmlParseHelperEntity() :
		mState(State::NotParsing), mPreviousState(State::NotParsing), mMatrixName(""), mMatrixComponentCount(0), mScopeHasBeenInitialized(false)
	{
		for (std::uint32_t i = 0; i < 4; i++)
		{
			for (std::uint32_t j = 0; j < 4; j++)
			{
				mMatrixComponents[i][j] = "";
			}
		}
	}

	void XmlParseHelperEntity::Initialize(XmlParseMaster* const xmlParseMaster)
	{
		mXmlParseMaster = xmlParseMaster;
	}

	IXmlParseHelper* XmlParseHelperEntity::Clone()
	{
		XmlParseHelperEntity* clone = new XmlParseHelperEntity();

		clone->mState = mState;
		clone->mMatrixComponentCount = mMatrixComponentCount;
		for (std::uint32_t i = 0; i < 4; i++)
		{
			for (std::uint32_t j = 0; j < 4; j++)
			{
				clone->mMatrixComponents[i][j] = mMatrixComponents[i][j];
			}
		}

		return clone;
	}

	bool XmlParseHelperEntity::StartElementHandler(
		XmlParseMaster::SharedData& sharedData, const std::string& element,
		const HashMap<std::string, std::string> attributes)
	{
		// Store off state to revert once the end element handler has been met
		mPreviousState = 
			(mState == State::NotParsing ||mState == State::ParsingWorld || mState == State::ParsingSector || mState == State::ParsingEntity) 
			? mState 
			: mPreviousState; 

		SharedDataScope* data = sharedData.As<SharedDataScope>();
		if (data == nullptr) { return false; }
		Scope* scope = data->mScope;

		if (element == "Integer")
		{
			mState = State::ParsingInteger;
			Datum& datum = scope->Append(attributes.Find("Name")->second);
			datum.SetType(DatumType::Integer);
			datum.SetFromString(attributes.Find("Value")->second);
		}

		else if (element == "Float")
		{
			mState = State::ParsingFloat;
			Datum& datum = scope->Append(attributes.Find("Name")->second);
			datum.SetType(DatumType::Float);
			datum.SetFromString(attributes.Find("Value")->second);
		}

		else if (element == "Vector")
		{
			mState = (mState == State::ParsingMatrix) ? State::ParsingMatrix : State::ParsingVector;

			std::string x, y, z, w;
			x = attributes.Find("X")->second;
			y = attributes.Find("Y")->second;
			z = attributes.Find("Z")->second;
			w = attributes.Find("W")->second;

			if (mState == State::ParsingVector)
			{
				assert(attributes.Find("Name") != attributes.end());
				std::pair<std::string, std::string> pair = *attributes.Find("Name");
				Datum& datum = scope->Append(attributes.Find("Name")->second);
				datum.SetType(DatumType::Vector);

				std::stringstream ss;
				ss << "vec4(" << x << ", " << y << ", " << z << ", " << w << ")";
				datum.SetFromString(ss.str());
			}

			else if (mState == State::ParsingMatrix)
			{
				mMatrixComponents[mMatrixComponentCount][0] = x;
				mMatrixComponents[mMatrixComponentCount][1] = y;
				mMatrixComponents[mMatrixComponentCount][2] = z;
				mMatrixComponents[mMatrixComponentCount][3] = w;

				mMatrixComponentCount++;
				assert(mMatrixComponentCount <= 4);
			}
		}

		else if (element == "Matrix")
		{
			mState = State::ParsingMatrix;
			mMatrixName = attributes.Find("Name")->second;
			// Only use this to Set state to start grabbing component vectors
		}

		else if (element == "String")
		{
			mState = State::ParsingString;
			Datum& datum = scope->Append(attributes.Find("Name")->second);
			datum.SetType(DatumType::String);
			datum.Set(attributes.Find("Value")->second);
		}

		else if (element == "World")
		{
			if (mState != State::NotParsing || mScopeHasBeenInitialized)
			{
				throw std::exception("Already parsing while attempting to parse a world");
			}

			mState = State::ParsingWorld;
			data->mScope = new World();
			data->mScope->As<World>()->SetName(attributes.Find("Name")->second);
			mScopeHasBeenInitialized = true;
		}

		else if(element == "Sector" || !mScopeHasBeenInitialized)
		{
			assert(data->mScope->Is(World::TypeIdClass()));
			if (mState != State::ParsingWorld)
			{
				throw std::exception("Attempting to parse a sector when not in a world");
			}

			mState = State::ParsingSector;

			World* world = data->mScope->As<World>();
			Sector* sector = world->CreateSector(attributes.Find("Name")->second);
			data->mScope = sector;
		}

		else if (element == "Entity" || !mScopeHasBeenInitialized)
		{
			assert(data->mScope->Is(Sector::TypeIdClass()));
			if(mState != State::ParsingSector)
			{
				throw std::exception("Attempting to parse an entity when not in a sector");
			}

			mState = State::ParsingEntity;
			std::string className = attributes.Find("ClassName")->second;
			std::string instanceName = attributes.Find("InstanceName")->second;

			Sector* sector = data->mScope->As<Sector>();
			Entity* entity = sector->CreateEntity(className, instanceName);

			data->mScope = entity;
		}

		else if (element == "Action")
		{
			assert(data->mScope->Is(Entity::TypeIdClass()) || data->mScope->Is(Action::TypeIdClass()));
			if (mState != State::ParsingEntity && mState != State::ParsingAction)
			{
				throw std::exception("Attempting to parse an action when not in an entity");
			}

			mState = State::ParsingAction;
			std::string className = attributes.Find("ClassName")->second;
			std::string instanceName = attributes.Find("InstanceName")->second;

			Action* action = nullptr;

			if (data->mScope->Is(Entity::TypeIdClass()))
			{
				mPreviousState = State::ParsingEntity;
				Entity* entity = data->mScope->As<Entity>();
				action = entity->CreateAction(className, instanceName);

				if (action->Is(ActionListIf::TypeIdClass()))
				{
					ActionListIf* actionList = action->As<ActionListIf>();
					assert(actionList != nullptr);
					std::string condition = attributes.Find("Condition")->second;
					actionList->SetCondition(std::stoi(condition));
				}
			}
			
			else if (data->mScope->Is(Action::TypeIdClass()))
			{
				mPreviousState = State::ParsingAction;
				Action* parent = data->mScope->As<Action>();
				if (parent->Is(ActionListIf::TypeIdClass()))
				{
					if (instanceName == "Then")
					{
						action = parent->As<ActionListIf>()->CreateThenAction(className, instanceName);
					}
					else if (instanceName == "Else")
					{
						action = parent->As<ActionListIf>()->CreateElseAction(className, instanceName);
					}
				}

				else if (parent->Is(ActionList::TypeIdClass()))
				{
					action = parent->As<ActionList>()->CreateAction(className, instanceName);
				}
			}

			data->mScope = action;
		}

		return true;
	}

	bool XmlParseHelperEntity::EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& element)
	{
		SharedDataScope* data = sharedData.As<SharedDataScope>();

		assert(data->Depth() > 0);
		if (data == nullptr) { return false; }

		if (element == "Integer")
		{
			assert(mState == State::ParsingInteger);
		}

		else if (element == "Float")
		{
			assert(mState == State::ParsingFloat);
		}

		else if (element == "Vector")
		{
			assert(mState == State::ParsingVector || mState == State::ParsingMatrix);
		}

		else if (element == "Matrix")
		{
			assert(mMatrixComponentCount == 4);
			std::stringstream ss;

			// String format: mat4x4((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))
			ss << "mat4x4(";
			for (std::uint32_t i = 0; i < 4; i++)
			{
				ss << "(";
				for (std::uint32_t j = 0; j < 4; j++)
				{
					ss << mMatrixComponents[i][j];
					if (j < 3)
					{	// Append commas in the first three elements of the row
						ss << ",";
					}
				}
				ss << ")";
				if (i < 3)
				{	// Append commas in the first three rows of the matrix
					ss << ",";
				}
			}
			ss << ")";

			Datum& datum = data->mScope->Append(mMatrixName);
			datum.SetType(DatumType::Matrix);
			datum.SetFromString(ss.str());

			mMatrixComponentCount = 0;
			assert(mState == State::ParsingMatrix);
		}

		else if (element == "String")
		{
			assert(mState == State::ParsingString);
		}

		else if (element == "World")
		{
			mPreviousState = State::NotParsing;
			assert(mState == State::ParsingWorld);
			assert(data->mScope->GetParent() == nullptr);
		}

		else if (element == "Sector")
		{
			mPreviousState = State::ParsingWorld;
			assert(mState == State::ParsingSector);
			assert(data->mScope->GetParent() != nullptr);
			data->mScope = data->mScope->GetParent();
		}

		else if (element == "Entity")
		{
			mPreviousState = State::ParsingSector;
			assert(mState == State::ParsingEntity);
			assert(data->mScope->GetParent() != nullptr);
			data->mScope = data->mScope->GetParent();
		}

		else if (element == "Action")
		{
			assert(mState == State::ParsingAction || mState == State::ParsingEntity);
			assert(data->mScope->GetParent() != nullptr);
			if (data->mScope->GetParent()->Is(Action::TypeIdClass()))
			{
				mPreviousState = State::ParsingAction;
			}
			else if(data->mScope->GetParent()->Is(Entity::TypeIdClass()))
			{
				mPreviousState = State::ParsingEntity;
			}
			else
			{
				throw std::exception("Attempting to enter an invalid state after parsing action");
			}
			data->mScope = data->mScope->GetParent();
		}

		mState = mPreviousState;
		if (data->Depth() == 0) mState = State::NotParsing;
		return true;
	}

	std::string XmlParseHelperEntity::ToString() const
	{
		return "Xml Parse Helper Scope";
	}

	bool XmlParseHelperEntity::Equals(const RTTI* rhs) const
	{
		if (this == rhs) { return true; }
		if (rhs == nullptr) { return false; }

		XmlParseHelperEntity* rhsConverted = rhs->As<XmlParseHelperEntity>();
		return (rhsConverted != nullptr) ? operator==(*rhsConverted) : false;
	}

	bool XmlParseHelperEntity::operator==(const XmlParseHelperEntity& rhs) const
	{
		bool matricesEquivalent = true;
		for (std::uint32_t i = 0; i < 4; i++)
		{
			for (std::uint32_t j = 0; j < 4; j++)
			{
				if (mMatrixComponents[i][j] != rhs.mMatrixComponents[i][j])
				{
					matricesEquivalent = false;
				}
			}
		}

		return	mXmlParseMaster == rhs.mXmlParseMaster &&
			mState == rhs.mState &&
			matricesEquivalent &&
			mScopeHasBeenInitialized == rhs.mScopeHasBeenInitialized;
	}
}


