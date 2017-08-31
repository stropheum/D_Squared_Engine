#pragma once
#include "ActionList.h"


namespace Library
{
	class ActionListIf :
		public ActionList
	{
	public:
		/// Constructor
		ActionListIf();

		/// Destructor(defautled)
		~ActionListIf() = default;

		/// Executes the if statement. Runs through "then" attribute updates if condition is true, "else" attributes otherwise
		/// @Param worldState: The world state object that stores the current world state
		virtual void update(WorldState& worldState) override;

		/// Mutator method for the if statement's condition
		/// @Param condition: The new condition for the if statement
		void setCondition(std::int32_t condition);

		/// Accessor method for the if statement's condition
		/// @Return: The condition for the if statement
		std::int32_t getCondition() const;

		Action* createThenAction(const std::string& className, const std::string& instanceName);
		Action* createElseAction(const std::string& className, const std::string& instanceName);

	private:
		std::int32_t mCondition; /// The condition to determine which set of action will be executed
	};

	ActionFactory(ActionListIf)
}
