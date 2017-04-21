#include "pch.h"
#include "CppUnitTest.h"
#include "TestSharedData.h"
#include "Datum.h"
#include "GameTime.h"
#include "Factory.h"
#include "World.h"
#include "Sector.h"
#include "Entity.h"
#include "Action.h"
#include "ActionList.h"
#include "WorldState.h"
#include "ActionCreateAction.h"
#include "Scope.h"
#include "ActionDestroyAction.h"
#include "ActionListIf.h"
#include "SharedDataScope.h"
#include "XmlParseHelperEntity.h"
#include "ActionIncrement.h"
#include "Event.h"
#include "Foo.h"
#include "MyEventSubscriber.h"
#include "IntEventSubscriber.h"
#include "ListClearingSubscriber.h"
#include "EnqueueSubscriber.h"
#include "QueueClearSubscriber.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;
using namespace std;
using namespace std::chrono;


namespace TestLibraryDesktop
{
	TEST_CLASS(AsyncTest)
	{
	public:

		EntityFactory mEntityFactory;
		ActionListFactory mActionListFactory;
		ActionCreateActionFactory mActionCreateActionFactory;
		ActionDestroyActionFactory mActionDestroyActionFactory;
		ActionListIfFactory mActionListIfFactory;
		ActionIncrementFactory mActionIncrementFactory;

		/// Sets up leak detection logic
		static void initializeLeakDetection()
		{
#if _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
#endif //_DEBUG
		}

		/// Detects if memory state has been corrupted
		static void finalizeLeakDetection()
		{
#if _DEBUG
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif //_DEBUG
		}

		TEST_METHOD_INITIALIZE(methodInitialize)
		{
			initializeLeakDetection();
		}

		TEST_METHOD_CLEANUP(methodCleanup)
		{
			finalizeLeakDetection();
		}

		TEST_METHOD(TestEvents)
		{
			for (int i = 0; i < 1000; i++)
			{
				EventQueue queue;
				Event<Foo> event2(Foo(2));
				Event<Foo> event3(Foo(3));
				Event<Foo> event4(Foo(4));

				GameTime time;
				queue.enqueue(event2, time, milliseconds(10));
				queue.enqueue(event3, time, milliseconds(0));
				queue.enqueue(event4, time, milliseconds(10));

				MyEventSubscriber sub1;

				time.SetCurrentTime(high_resolution_clock::time_point(milliseconds(10000000)));
				Assert::AreEqual(3u, queue.size());

				queue.update(time);
				std::int32_t data = sub1.mValue.getData();
				Assert::IsTrue(data == 2 || data == 3 || data == 4);
			}
		}

		TEST_METHOD(TestMultipleSubscribers)
		{
			EventQueue queue;
			Event<std::int32_t> event(10);
			GameTime time;
			queue.enqueue(event, time, milliseconds(0));

			time.SetCurrentTime(high_resolution_clock::time_point(milliseconds(500000)));
			Assert::AreEqual(1u, queue.size());

			IntEventSubscriber sub1;
			IntEventSubscriber sub2;
			IntEventSubscriber sub3;
			IntEventSubscriber sub4; 
			IntEventSubscriber sub5;

			queue.update(time);
			Assert::AreEqual(0u, queue.size());

			bool notified1 = sub1.mNotified;
			bool notified2 = sub2.mNotified;
			bool notified3 = sub3.mNotified;
			bool notified4 = sub4.mNotified;
			bool notified5 = sub5.mNotified;

			int32_t val1 = sub1.mValue;
			int32_t val2 = sub2.mValue;
			int32_t val3 = sub3.mValue;
			int32_t val4 = sub4.mValue; 
			int32_t val5 = sub5.mValue;
				 
			Assert::AreEqual(true, notified1);
			Assert::AreEqual(true, notified2);
			Assert::AreEqual(true, notified3);
			Assert::AreEqual(true, notified4);
			Assert::AreEqual(true, notified5);

			Assert::AreEqual(10, val1);
			Assert::AreEqual(10, val2);
			Assert::AreEqual(10, val3);
			Assert::AreEqual(10, val4);
			Assert::AreEqual(10, val5);
		}

		TEST_METHOD(TestUnsubscribeEvent)
		{
			EventQueue queue;
			Event<Foo> event1(Foo(10));
			Event<Foo> event2(Foo(10));
			Event<Foo> event3(Foo(10));

			GameTime time;
			queue.enqueue(event1, time, milliseconds(0));
			queue.enqueue(event2, time, milliseconds(0));
			queue.enqueue(event3, time, milliseconds(0));

			time.SetCurrentTime(high_resolution_clock::time_point(milliseconds(500000)));
			Assert::AreEqual(3u, queue.size());

			MyEventSubscriber eventSub;
			ListClearingSubscriber clearSub;
			queue.update(time);
			Assert::AreEqual(0u, queue.size());
		}

		TEST_METHOD(TestEnqueueEvent)
		{
			EventQueue queue;
			Event<std::int32_t> event1(10);
			GameTime time;
			queue.enqueue(event1, time, milliseconds(0));
			time.SetCurrentTime(high_resolution_clock::time_point(milliseconds(50000)));
			EnqueueSubscriber sub(queue, time, milliseconds(0));
			Assert::AreEqual(1u, queue.size());
			queue.update(time);
			Assert::AreEqual(0u, queue.size());
		}

		TEST_METHOD(TestClearQueue)
		{
			EventQueue queue;
			Event<Foo> event1(Foo(10));
			Event<Foo> event2(Foo(10));
			Event<Foo> event3(Foo(10));
			Event<Foo> event4(Foo(10));
			Event<Foo> event5(Foo(10));

			GameTime time;
			queue.enqueue(event1, time, milliseconds(0));
			queue.enqueue(event2, time, milliseconds(0));
			queue.enqueue(event3, time, milliseconds(0));
			queue.enqueue(event4, time, milliseconds(0));
			queue.enqueue(event5, time, milliseconds(0));

			time.SetCurrentTime(high_resolution_clock::time_point(milliseconds(50000)));

			QueueClearSubscriber sub(queue);
			Assert::AreEqual(5u, queue.size());
			queue.update(time);
			Assert::AreEqual(0u, queue.size());
		}

		TEST_METHOD(TestHugeNumbers)
		{
			EventQueue queue;

			Vector<Event<Foo>> events;
			for (std::uint32_t i = 0; i < 500; i++)
			{
				events.pushBack(Event<Foo>(Foo(i)));
			}

			MyEventSubscriber sub1;
			MyEventSubscriber sub2;
			MyEventSubscriber sub3;
			MyEventSubscriber sub4;
			MyEventSubscriber sub5;
			MyEventSubscriber sub6;
			MyEventSubscriber sub7;
			MyEventSubscriber sub8;
			MyEventSubscriber sub9; 

			GameTime time;
			
			for (std::uint32_t i = 0; i < events.size(); i++)
			{
				queue.enqueue(events[i], time, milliseconds(0));
			}

			time.SetCurrentTime(high_resolution_clock::time_point(milliseconds(50000)));

			Assert::AreEqual(500u, queue.size());
			queue.update(time);
			Assert::AreEqual(0u, queue.size());
		}

		static _CrtMemState sStartMemState;
	};
	_CrtMemState AsyncTest::sStartMemState;
}