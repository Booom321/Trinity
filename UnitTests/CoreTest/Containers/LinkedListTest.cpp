#include "LinkedListTest.h"

#include <Trinity/Core/Containers/LinkedList.h>

template<typename LinkedListType>
TBool CheckValid(const LinkedListType& List, std::initializer_list<typename LinkedListType::ElementType> InitList, typename LinkedListType::SizeType ExpectElementCount)
{
	if (List.Length() == ExpectElementCount)
	{
		if (List.Length() != (typename LinkedListType::SizeType)InitList.size())
		{
			return false;
		}

		const typename LinkedListType::NodeType* CurrentNode = List.GetHead();

		typename LinkedListType::ConstPointerType Begin = InitList.begin();
		typename LinkedListType::ConstPointerType End = InitList.end();
		
		while (CurrentNode && Begin != End)
		{
			if (!(CurrentNode->Value == *Begin))
			{
				return false;
			}

			CurrentNode = CurrentNode->Next;
			++Begin;
		}

		// Additional testing to be sure (for TDoublyLinkedList)
		if constexpr (TAreTheSameType<LinkedListType, TDoublyLinkedList<typename LinkedListType::ElementType>>::Value)
		{
			const typename LinkedListType::NodeType* CurrentNode = List.GetTail();

			Begin	= InitList.begin();
			End		= InitList.end();

			while (CurrentNode && Begin != End)
			{
				if (!(CurrentNode->Value == *--End))
				{
					return false;
				}

				CurrentNode = CurrentNode->Previous;
			}
		}

		return true;
	}

	return false;
}


TRNT_IMPL_TEST_CASE(Containers, TSinglyLinkedList)
{
	using TInt32LinkedList = TSinglyLinkedList<TInt32>;

	// constructor   
	{
		TInt32LinkedList l1;
		TRNT_TEST_EXPECT_TRUE(l1.GetHead() == nullptr && l1.GetTail() == nullptr && l1.Length() == 0);

		TInt32LinkedList l2{ 1,2,3,4 };
		TRNT_TEST_EXPECT_TRUE(CheckValid<TInt32LinkedList>(l2, { 1,2,3,4 }, 4));

		TInt32LinkedList l3_1(l2);
		TInt32LinkedList l3_2 = l2; 
		TRNT_TEST_EXPECT_TRUE(CheckValid<TInt32LinkedList>(l3_1, { 1,2,3,4 }, 4));
		TRNT_TEST_EXPECT_TRUE(l3_1 == l2);
		TRNT_TEST_EXPECT_TRUE(l3_1 == l3_2);

		TInt32LinkedList l4(Move(l2));
		TRNT_TEST_EXPECT_TRUE(CheckValid<TInt32LinkedList>(l4, { 1,2,3,4 }, 4));
		TRNT_TEST_EXPECT_TRUE(l2.GetHead() == nullptr && l2.Length() == 0);
	}

	// operator=
	{
		TInt32LinkedList l1;
		l1 = { 12,23,34,45,56,67,78,89 };
		TRNT_TEST_EXPECT_TRUE(CheckValid<TInt32LinkedList>(l1, { 12,23,34,45,56,67,78,89 }, 8));

		TInt32LinkedList l2{ 11,22,33,44,55 };
		l1 = l2;
		TRNT_TEST_EXPECT_TRUE(l1 == l2);
		TRNT_TEST_EXPECT_TRUE(CheckValid<TInt32LinkedList>(l1, { 11,22,33,44,55 }, 5));

		l1 = Move(l2);
		TRNT_TEST_EXPECT_TRUE(l2.GetHead() == nullptr && l2.GetTail() == nullptr && l2.Length() == 0);
		TRNT_TEST_EXPECT_TRUE(CheckValid<TInt32LinkedList>(l1, { 11,22,33,44,55 }, 5));
	}

	// iterator
	{
		TInt32LinkedList l1{ 1,2,3,4,5,6,7 };

		int i = 0;
		for (TInt32LinkedList::ConstIteratorType It = l1.cbegin(); It != l1.cend(); ++It)
		{
			++i;
			TRNT_TEST_EXPECT_TRUE(It->Value == i);
		}

		i = 0;
		for (TInt32LinkedList::NodeType& Val : l1)
		{
			++i;
			TRNT_TEST_EXPECT_TRUE(Val.Value == i);
		}
	}

	// compare
	{
		TInt32LinkedList l1{ 1,2,3,4,5,6,7 };
		TInt32LinkedList l2{ 1,2,3,4,5,6,7 };
		TInt32LinkedList l3{ 11,22,33,44,55,66,77 };
		TRNT_TEST_EXPECT_EQ(l1, l2);

		TRNT_TEST_EXPECT_NEQ(l1, l3);
	}

	{
		TInt32LinkedList l{ 1,2,3,4,5,6,7,8 };

		auto IsOdd = [](TInt32 Element) -> TBool { return Element % 2 == 1; };

		TRNT_TEST_EXPECT_TRUE(l.Find(3) != nullptr);
		TRNT_TEST_EXPECT_TRUE(l.Find(9999) == nullptr);

		TRNT_TEST_EXPECT_TRUE(l.FindIf(IsOdd) != nullptr);

		TRNT_TEST_EXPECT_TRUE(l.Contains(4));
	}

	{
		TInt32LinkedList l{ 1,2,3,4,5,6,7,8 };

		TRNT_TEST_EXPECT_TRUE(l.Length() == 8 && (l.First() == 1) && l.GetHead()->Value == 1 && l.GetTail()->Value == 8);
	}

	{
		TInt32LinkedList l{ 1,2,3 };

		l.EmplaceAtHead(0);
		TRNT_TEST_EXPECT_TRUE(CheckValid<TInt32LinkedList>(l, { 0,1,2,3 }, 4));
		TRNT_TEST_EXPECT_TRUE(l.GetHead()->Value == 0 && l.GetTail()->Value == 3);
		
		l.EmplaceAtTail(4);
		TRNT_TEST_EXPECT_TRUE(CheckValid<TInt32LinkedList>(l, { 0,1,2,3,4 }, 5));
		TRNT_TEST_EXPECT_TRUE(l.GetHead()->Value == 0 && l.GetTail()->Value == 4);

		l.Clear();

		l.InsertAtHead(1);
		TRNT_TEST_EXPECT_TRUE(CheckValid<TInt32LinkedList>(l, { 1 }, 1));

		l.InsertAtTail(2);
		TRNT_TEST_EXPECT_TRUE(CheckValid<TInt32LinkedList>(l, { 1,2 }, 2));

		l.Clear();

		l.InsertAtHead(new TInt32LinkedList::NodeType(-1));
		l.InsertAtHead(new TInt32LinkedList::NodeType(-2));
		TRNT_TEST_EXPECT_TRUE(CheckValid<TInt32LinkedList>(l, { -2, -1 }, 2));

		l.InsertAtTail(new TInt32LinkedList::NodeType(0));
		l.InsertAtTail(new TInt32LinkedList::NodeType(1));

		TRNT_TEST_EXPECT_TRUE(CheckValid<TInt32LinkedList>(l, { -2, -1, 0, 1 }, 4));
		TRNT_TEST_EXPECT_TRUE(l.GetHead()->Value == -2 && l.GetTail()->Value == 1);
	}

	// removal
	{
		TInt32LinkedList l{ 1,2,3,4,5,6,7 };

		l.RemoveAtHead();
		TRNT_TEST_EXPECT_TRUE(CheckValid<TInt32LinkedList>(l, { 2,3,4,5,6,7 }, 6));

		l.RemoveAtTail();
		TRNT_TEST_EXPECT_TRUE(CheckValid<TInt32LinkedList>(l, { 2,3,4,5,6 }, 5));
		TRNT_TEST_EXPECT_TRUE(l.GetHead()->Value == 2 && l.GetTail()->Value == 6);

		TInt32 Ret = l.PopFront();
		TRNT_TEST_EXPECT_TRUE(CheckValid<TInt32LinkedList>(l, { 3,4,5,6 }, 4));
		TRNT_TEST_EXPECT_TRUE(Ret == 2);

		TInt32LinkedList Empty{};

		TInt32LinkedList::NodeType* HeadNode = Empty.RemoveAtHead();
		TInt32LinkedList::NodeType* TailNode = Empty.RemoveAtTail();

		TRNT_TEST_EXPECT_TRUE(HeadNode == nullptr && TailNode == nullptr);

		// Assertion failed: TInt32 Num = Empty.PopFront();
	}
}

TRNT_IMPL_TEST_CASE(Containers, TDoublyLinkedList)
{
	using TInt32LinkedList = TDoublyLinkedList<TInt32>;

	// constructors
	{
		TInt32LinkedList l;
		TRNT_TEST_EXPECT_TRUE(
			l.IsEmpty() && l.Length() == 0 && l.GetHead() == nullptr && l.GetTail() == nullptr
		);

		TInt32LinkedList l1{ 1,2,3,4,5,6,7 };
		TRNT_TEST_EXPECT_TRUE(CheckValid<TInt32LinkedList>(l1, { 1,2,3,4,5,6,7 }, 7));

		TInt32LinkedList l2(l1);
		TInt32LinkedList l3 = l1;
		TRNT_TEST_EXPECT_TRUE(CheckValid<TInt32LinkedList>(l2, { 1,2,3,4,5,6,7 }, 7));
		TRNT_TEST_EXPECT_TRUE(l2 == l1 && l3 == l1);

		TInt32LinkedList l4(Move(l2));
		TRNT_TEST_EXPECT_TRUE(CheckValid<TInt32LinkedList>(l4, { 1,2,3,4,5,6,7 }, 7));
		TRNT_TEST_EXPECT_TRUE(l2.GetHead() == nullptr && l2.Length() == 0);
	}

	// operator=
	{
		TInt32LinkedList l1;
		l1 = { 1,2,3,4,5,6,7 };
		TRNT_TEST_EXPECT_TRUE(CheckValid<TInt32LinkedList>(l1, { 1,2,3,4,5,6,7 }, 7));

		TInt32LinkedList l2{};
		l1 = l2;
		TRNT_TEST_EXPECT_TRUE(l1 == l2);
		TRNT_TEST_EXPECT_TRUE(CheckValid<TInt32LinkedList>(l1, {}, 0));

		l2 = { 1,2,3,4,5,6,7,8,9 };

		l1 = Move(l2);
		TRNT_TEST_EXPECT_TRUE(l2.GetHead() == nullptr && l2.GetTail() == nullptr && l2.Length() == 0);
		TRNT_TEST_EXPECT_TRUE(CheckValid<TInt32LinkedList>(l1, { 1,2,3,4,5,6,7,8,9 }, 9));
	}

	// iterator
	{
		TInt32LinkedList l1{ 1,2,3,4,5,6,7 };

		int i = 0;
		for (TInt32LinkedList::ConstIteratorType It = l1.cbegin(); It != l1.cend(); ++It)
		{
			++i;
			TRNT_TEST_EXPECT_TRUE(It->Value == i);
		}

		i = 0;
		for (TInt32LinkedList::NodeType& Val : l1)
		{
			++i;
			TRNT_TEST_EXPECT_TRUE(Val.Value == i);
		}
	}

	// compare
	{
		TInt32LinkedList l1{ 1,2,3,4,5,6,7 };
		TInt32LinkedList l2{ 1,2,3,4,5,6,7 };
		TInt32LinkedList l3{ -1, -2, -3, -4, -5 };
		TRNT_TEST_EXPECT_EQ(l1, l2);

		TRNT_TEST_EXPECT_NEQ(l1, l3);
	}

	{
		TInt32LinkedList l{ 1,2,3,4,5,6,7,8 };

		auto IsOdd = [](TInt32 Element) -> TBool { return Element % 2 == 1; };

		TRNT_TEST_EXPECT_TRUE(l.Find(3) != nullptr);
		TRNT_TEST_EXPECT_TRUE(l.Find(9999) == nullptr);

		TRNT_TEST_EXPECT_TRUE(l.FindIf(IsOdd) != nullptr);

		TRNT_TEST_EXPECT_TRUE(l.Contains(4));
	}

	{
		TInt32LinkedList l{ 1,2,3,4,5,6,7,8 };

		TRNT_TEST_EXPECT_TRUE(l.Length() == 8 && l.First() == 1 && l.Last() == 8 && l.GetHead()->Value == 1 && l.GetTail()->Value == 8);
	}

	// removal
	{
		TInt32LinkedList EmptyList;

		// Because this list is empty, both RemoveAtHead() and RemoveAtTail() do nothing!
		EmptyList.RemoveAtHead();
		EmptyList.RemoveAtTail();
		TRNT_TEST_EXPECT_TRUE(CheckValid<TInt32LinkedList>(EmptyList, {}, 0));

		TInt32LinkedList l{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };

		l.RemoveAtHead();
		TRNT_TEST_EXPECT_TRUE(CheckValid<TInt32LinkedList>(l, { 2,3,4,5,6,7,8,9,10,11,12,13,14,15 }, 14));

		l.RemoveAtTail();
		TRNT_TEST_EXPECT_TRUE(CheckValid<TInt32LinkedList>(l, { 2,3,4,5,6,7,8,9,10,11,12,13,14 }, 13));

		typename TInt32LinkedList::NodeType* Node = l.RemoveAt(l.GetHead());
		TRNT_TEST_EXPECT_EQ(Node, l.GetHead());
		TRNT_TEST_EXPECT_TRUE(CheckValid<TInt32LinkedList>(l, { 3,4,5,6,7,8,9,10,11,12,13,14 }, 12));

		Node = l.RemoveAt(l.GetTail());
		TRNT_TEST_EXPECT_EQ(Node, l.GetTail());
		TRNT_TEST_EXPECT_TRUE(CheckValid<TInt32LinkedList>(l, { 3,4,5,6,7,8,9,10,11,12,13 }, 11));

		Node = l.GetHead();

		TInt32 i = 3;
		while (i--)
		{
			Node = Node->Next;
		}
		
		Node = l.RemoveAt(Node);
		TRNT_TEST_EXPECT_EQ(Node->Value, 7);
		TRNT_TEST_EXPECT_TRUE(CheckValid<TInt32LinkedList>(l, { 3,4,5,7,8,9,10,11,12,13 }, 10));

		l = { 1,2,3,4,5,6 };
		
		TRNT_TEST_EXPECT_EQ(l.PopFront(), 1);
		TRNT_TEST_EXPECT_EQ(l.PopBack(), 6);
		TRNT_TEST_EXPECT_TRUE(CheckValid<TInt32LinkedList>(l, { 2, 3, 4, 5 }, 4));

		l.Clear();

		// l.PopFront();	// assertion failed!
		// l.PopBack();		// assertion failed!
	}
	
	// Insertion
	{
		TInt32LinkedList l;
		l.EmplaceAtHead(1);
		l.EmplaceAtTail(3);
		l.EmplaceAtHead(2);

		TRNT_TEST_EXPECT_TRUE(CheckValid<TInt32LinkedList>(l, { 2, 1, 3 }, 3));
		
		l.EmplaceAt(l.GetHead(), 5);
		l.EmplaceAt(l.GetTail(), 10);
		l.EmplaceAt(l.GetHead()->Next->Next->Next, 4);
		TRNT_TEST_EXPECT_TRUE(CheckValid<TInt32LinkedList>(l, { 5, 2, 1, 4, 10, 3 }, 6));

		l.Clear();

		l.InsertAtHead(3);
		l.InsertAtHead(new TInt32LinkedList::NodeType(4));

		TRNT_TEST_EXPECT_TRUE(CheckValid<TInt32LinkedList>(l, { 4, 3 }, 2));

		l.InsertAtTail(3);
		l.InsertAtTail(new TInt32LinkedList::NodeType(5));
		TRNT_TEST_EXPECT_TRUE(CheckValid<TInt32LinkedList>(l, { 4, 3, 3, 5 }, 4));

		l.InsertAt(l.GetHead(), 3);
		l.InsertAt(l.GetTail(), 3);
		TRNT_TEST_EXPECT_TRUE(CheckValid<TInt32LinkedList>(l, { 3, 4, 3, 3, 3, 5 }, 6));
	}
}
