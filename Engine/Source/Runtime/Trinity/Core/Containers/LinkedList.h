#pragma once

#include <initializer_list>

#include "Trinity/Core/Defines.h"
#include "Trinity/Core/Assert/AssertionMacros.h"
#include "Trinity/Core/Types/DataTypes.h"
#include "Trinity/Core/TypeTraits/RemoveReference.h"

template<typename LinkedListType>
class TLinkedListHelper
{
public:
	static void Clear(LinkedListType& List)
	{
		typename LinkedListType::NodeType* Node;

		while (List.Head != nullptr)
		{
			Node = List.Head->Next;
			delete List.Head;
			List.Head = Node;
		}

		List.Head = nullptr;
		List.ElementCount = 0;
	}

	static TBool AreEqual(const LinkedListType& Lhs, const LinkedListType& Rhs)
	{
		if (Lhs.ElementCount != Rhs.ElementCount)
		{
			return false;
		}

		typename LinkedListType::NodeType* CurrentLhsNode = Lhs.Head;
		typename LinkedListType::NodeType* CurrentRhsNode = Rhs.Head;

		while (CurrentLhsNode)
		{
			if (!(CurrentLhsNode->Value == CurrentRhsNode->Value))
			{
				return false;
			}
			CurrentLhsNode = CurrentLhsNode->Next;
			CurrentRhsNode = CurrentRhsNode->Next;
		}

		return true;
	}

public:
	static typename LinkedListType::NodeType* Find(const LinkedListType& List, typename LinkedListType::ConstReferenceType Element)
	{
		typename LinkedListType::NodeType* CurrentNode = List.Head;

		while (CurrentNode != nullptr)
		{
			if (CurrentNode->Value == Element)
			{
				break;
			}

			CurrentNode = CurrentNode->Next;
		}

		return CurrentNode;
	}

	static TBool Contains(const LinkedListType& List, typename LinkedListType::ConstReferenceType Element)
	{
		typename LinkedListType::NodeType* CurrentNode = List.Head;

		while (CurrentNode != nullptr)
		{
			if (CurrentNode->Value == Element)
			{
				return true;
			}

			CurrentNode = CurrentNode->Next;
		}

		return false;
	}

	template<typename PredicateType>
	static typename LinkedListType::NodeType* FindIf(const LinkedListType& List, PredicateType Predicate)
	{
		typename LinkedListType::NodeType* CurrentNode = List.Head;

		while (CurrentNode != nullptr)
		{
			if (Predicate(CurrentNode->Value))
			{
				break;
			}

			CurrentNode = CurrentNode->Next;
		}

		return CurrentNode;
	}

	template<typename PredicateType>
	static TBool ContainsIf(const LinkedListType& List, PredicateType Predicate)
	{
		typename LinkedListType::NodeType* CurrentNode = List.Head;

		while (CurrentNode != nullptr)
		{
			if (Predicate(CurrentNode->Value))
			{
				return true;
			}

			CurrentNode = CurrentNode->Next;
		}

		return false;
	}
};

/////////////////////////////////////////////////////////////////////////////////////////////////////

template<template<typename, typename> class LinkedListIteratorType, typename NodeType, typename ElementType>
class TLinkedListIteratorBase
{
public:
	TRNT_FORCE_INLINE TLinkedListIteratorBase(NodeType* CurrentNode)
		: CurrentNode(CurrentNode)
	{}

public:
	TRNT_FORCE_INLINE NodeType& operator*() { return *CurrentNode; }
	TRNT_FORCE_INLINE const NodeType& operator*() const { return *CurrentNode; }

	TRNT_FORCE_INLINE NodeType* operator->() { return CurrentNode; }
	TRNT_FORCE_INLINE const NodeType* operator->() const { return CurrentNode; }

	TRNT_FORCE_INLINE ElementType& GetValue() { return CurrentNode->Value; }
	TRNT_FORCE_INLINE ElementType& GetValue() const { return CurrentNode->Value; }

public:
	TRNT_FORCE_INLINE TBool operator==(const LinkedListIteratorType<NodeType, ElementType>& Rhs) const { return CurrentNode == Rhs.CurrentNode; }
	TRNT_FORCE_INLINE TBool operator!=(const LinkedListIteratorType<NodeType, ElementType>& Rhs) const { return CurrentNode != Rhs.CurrentNode; }

protected:
	NodeType* CurrentNode;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// TSinglyLinkedList /////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename Type>
class TSinglyLinkedListNode
{
public:
	template<typename... ArgumentsType>
	TRNT_FORCE_INLINE TSinglyLinkedListNode(ArgumentsType&&... Args)
		: Value(Forward<ArgumentsType>(Args)...)
	{}

	~TSinglyLinkedListNode() = default;

	Type Value;
	TSinglyLinkedListNode* Next = nullptr;
};

template<typename NodeType, typename ElementType>
class TSinglyLinkedListIterator : public TLinkedListIteratorBase<TSinglyLinkedListIterator, NodeType, ElementType>
{
public:
	using Super = TLinkedListIteratorBase<TSinglyLinkedListIterator, NodeType, ElementType>;

public:
	TRNT_FORCE_INLINE TSinglyLinkedListIterator(NodeType* CurrentNode)
		: Super(CurrentNode)
	{
	}

public:
	TRNT_FORCE_INLINE TSinglyLinkedListIterator& operator++()
	{
		TRNT_ASSERT_IS_NOT_NULL(this->CurrentNode);
		this->CurrentNode = this->CurrentNode->Next;
		return *this;
	}

	TRNT_FORCE_INLINE TSinglyLinkedListIterator operator++(int)
	{
		TRNT_ASSERT_IS_NOT_NULL(this->CurrentNode);
		TSinglyLinkedListIterator Tmp{ this->CurrentNode };
		this->CurrentNode = this->CurrentNode->Next;
		return Tmp;
	}
};

template<typename Type>
class TSinglyLinkedList
{
public:
	using LinkedListHelper = TLinkedListHelper<TSinglyLinkedList<Type>>;

	friend class LinkedListHelper;

	using ElementType = Type;

	using ReferenceType = ElementType&;
	using ConstReferenceType = const ElementType&;

	using PointerType = ElementType*;
	using ConstPointerType = const ElementType*;

	using SizeType = TInt32;

	using NodeType = TSinglyLinkedListNode<ElementType>;

	using IteratorType = TSinglyLinkedListIterator<NodeType, ElementType>;
	using ConstIteratorType = TSinglyLinkedListIterator<const NodeType, const ElementType>;

public:
	TRNT_FORCE_INLINE TSinglyLinkedList()
		: Head(nullptr), ElementCount(0)
	{}

	TRNT_FORCE_INLINE TSinglyLinkedList(TSinglyLinkedList&& List) noexcept
		: Head(List.Head), ElementCount(List.ElementCount)
	{
		List.Head = nullptr;
		List.ElementCount = 0;
	}

	TSinglyLinkedList(const TSinglyLinkedList& List)
		: ElementCount(List.ElementCount)
	{
		if (List.Head == nullptr)
		{
			Head = nullptr;
		}
		else
		{
			NodeType* CurrentListNode = List.Head;

			this->Head = new NodeType(CurrentListNode->Value);
			NodeType* CurrentThisListNode = this->Head;

			CurrentListNode = CurrentListNode->Next;
			while (CurrentListNode != nullptr)
			{
				CurrentThisListNode->Next = new NodeType(CurrentListNode->Value);
				CurrentThisListNode = CurrentThisListNode->Next;

				CurrentListNode = CurrentListNode->Next;
			}
		}
	}

	TSinglyLinkedList(std::initializer_list<ElementType> List)
		: ElementCount((SizeType)List.size())
	{
		if (ElementCount == 0)
		{
			Head = nullptr;
		}
		else
		{
			ConstPointerType ListBegin = List.begin();
			ConstPointerType ListEnd = List.end();

			Head = new NodeType(*ListBegin++);
			NodeType* CurrentNode = Head;

			while (ListBegin != ListEnd)
			{
				CurrentNode->Next = new NodeType(*ListBegin);
				CurrentNode = CurrentNode->Next;

				++ListBegin;
			}
		}
	}

	~TSinglyLinkedList()
	{
		LinkedListHelper::Clear(*this);
	}

	TRNT_FORCE_INLINE void Clear()
	{
		LinkedListHelper::Clear(*this);
	}

public:
	TBool operator==(const TSinglyLinkedList& Other) const
	{
		return LinkedListHelper::AreEqual(*this, Other);
	}

	TRNT_FORCE_INLINE TBool operator!=(const TSinglyLinkedList& Other) const
	{
		return !LinkedListHelper::AreEqual(*this, Other);
	}

public:
	TSinglyLinkedList& operator=(const TSinglyLinkedList& Other)
	{
		if (this != &Other)
		{
			if (Head != nullptr)
			{
				NodeType* Node;
				while (Head != nullptr)
				{
					Node = Head->Next;
					delete Head;
					Head = Node;
				}
			}

			if (Other.Head != nullptr)
			{
				NodeType* CurrentListNode = Other.Head;

				this->Head = new NodeType(CurrentListNode->Value);
				NodeType* CurrentThisListNode = this->Head;

				CurrentListNode = CurrentListNode->Next;

				while (CurrentListNode != nullptr)
				{
					CurrentThisListNode->Next = new NodeType(CurrentListNode->Value);
					CurrentThisListNode = CurrentThisListNode->Next;

					CurrentListNode = CurrentListNode->Next;
				}
			}

			ElementCount = Other.ElementCount;
		}
		return *this;
	}

	TSinglyLinkedList& operator=(TSinglyLinkedList&& Other) noexcept
	{
		if (this != &Other)
		{
			if (Head != nullptr)
			{
				NodeType* Node;
				while (Head != nullptr)
				{
					Node = Head->Next;
					delete Head;
					Head = Node;
				}
			}

			Head = Other.Head;
			ElementCount = Other.ElementCount;

			Other.Head = nullptr;
			Other.ElementCount = 0;
		}
		return *this;
	}

	TSinglyLinkedList& operator=(std::initializer_list<ElementType> Other)
	{
		ElementCount = (SizeType)Other.size();

		if (Head != nullptr)
		{
			NodeType* Node;
			while (Head != nullptr)
			{
				Node = Head->Next;
				delete Head;
				Head = Node;
			}
		}

		if (ElementCount > 0)
		{
			ConstPointerType ListBegin = Other.begin();
			ConstPointerType ListEnd = Other.end();

			Head = new NodeType(*ListBegin++);
			NodeType* CurrentNode = Head;

			while (ListBegin != ListEnd)
			{
				CurrentNode->Next = new NodeType(*ListBegin);
				CurrentNode = CurrentNode->Next;

				++ListBegin;
			}
		}

		return *this;
	}

public:
	TRNT_FORCE_INLINE SizeType Length() const
	{
		return ElementCount;
	}

	TRNT_FORCE_INLINE TBool IsEmpty() const
	{
		return ElementCount == 0;
	}

	TRNT_FORCE_INLINE ReferenceType First()
	{
		TRNT_ASSERT(ElementCount > 0);
		return Head->Value;
	}

	TRNT_FORCE_INLINE ConstReferenceType First() const
	{
		TRNT_ASSERT(ElementCount > 0);
		return Head->Value;
	}

	TRNT_FORCE_INLINE NodeType* GetHead()
	{
		return Head;
	}

	TRNT_FORCE_INLINE const NodeType* GetHead() const
	{
		return Head;
	}

	NodeType* GetTail()
	{
		if (Head == nullptr)
		{
			return nullptr;
		}

		NodeType* CurrentNode = Head;
		while (CurrentNode->Next != nullptr)
		{
			CurrentNode = CurrentNode->Next;
		}
		return CurrentNode;
	}

	const NodeType* GetTail() const
	{
		if (Head == nullptr)
		{
			return nullptr;
		}

		NodeType* CurrentNode = Head;
		while (CurrentNode->Next != nullptr)
		{
			CurrentNode = CurrentNode->Next;
		}
		return CurrentNode;
	}

	TRNT_FORCE_INLINE IteratorType begin() noexcept { return IteratorType(Head); }
	TRNT_FORCE_INLINE ConstIteratorType begin() const noexcept { return ConstIteratorType(Head); }
	TRNT_FORCE_INLINE ConstIteratorType cbegin() const noexcept { return ConstIteratorType(Head); }

	TRNT_FORCE_INLINE IteratorType end() noexcept { return IteratorType(nullptr); }
	TRNT_FORCE_INLINE ConstIteratorType end() const noexcept { return ConstIteratorType(nullptr); }
	TRNT_FORCE_INLINE ConstIteratorType cend() const noexcept { return ConstIteratorType(nullptr); }

public:
	TRNT_FORCE_INLINE NodeType* Find(ConstReferenceType Element) const
	{
		return LinkedListHelper::Find(*this, Element);
	}

	TRNT_FORCE_INLINE TBool Contains(ConstReferenceType Element) const
	{
		return LinkedListHelper::Contains(*this, Element);
	}

	template<typename PredicateType>
	TRNT_FORCE_INLINE NodeType* FindIf(PredicateType Predicate) const
	{
		return LinkedListHelper::FindIf(*this, Predicate);
	}

	template<typename PredicateType>
	TRNT_FORCE_INLINE TBool ContainsIf(PredicateType Predicate) const
	{
		return LinkedListHelper::ContainsIf(*this, Predicate);
	}

public:
	template<typename... ArgumentsType>
	NodeType* EmplaceAtHead(ArgumentsType&&... Args)
	{
		NodeType* NewNode = new NodeType(Forward<ArgumentsType>(Args)...);

		NewNode->Next = Head;
		Head = NewNode;

		++ElementCount;

		return Head;
	}

	template<typename... ArgumentsType>
	NodeType* EmplaceAtTail(ArgumentsType&&... Args)
	{
		NodeType* NewNode = new NodeType(Forward<ArgumentsType>(Args)...);

		NodeType* CurrentNode = Head;

		if (Head == nullptr)
		{
			Head = NewNode;
		}
		else
		{
			while (CurrentNode->Next != nullptr)
			{
				CurrentNode = CurrentNode->Next;
			}

			CurrentNode->Next = NewNode;
		}

		++ElementCount;
		return NewNode;
	}

	NodeType* InsertAtHead(NodeType* NewNode)
	{
		if (NewNode == nullptr)
		{
			return nullptr;
		}

		NewNode->Next = Head;
		Head = NewNode;
		++ElementCount;

		return Head;
	}

	NodeType* InsertAtTail(NodeType* NewNode)
	{
		if (NewNode == nullptr)
		{
			return nullptr;
		}

		NodeType* CurrentNode = Head;

		if (Head == nullptr)
		{
			Head = NewNode;
		}
		else
		{
			while (CurrentNode->Next != nullptr)
			{
				CurrentNode = CurrentNode->Next;
			}

			CurrentNode->Next = NewNode;
		}
		++ElementCount;
		return NewNode;
	}

	NodeType* InsertAtHead(ConstReferenceType Element)
	{
		return InsertAtHead(new NodeType(Element));
	}

	NodeType* InsertAtTail(ConstReferenceType Element)
	{
		return InsertAtTail(new NodeType(Element));
	}

	NodeType* InsertAtHead(ElementType&& Element)
	{
		return InsertAtHead(new NodeType(Move(Element)));
	}

	NodeType* InsertAtTail(ElementType&& Element)
	{
		return InsertAtTail(new NodeType(Move(Element)));
	}

	NodeType* RemoveAtHead()
	{
		if (ElementCount == 0)
		{
			return nullptr;
		}

		NodeType* Tmp = Head->Next;
		delete Head;
		Head = Tmp;

		--ElementCount;
		return Head;
	}

	NodeType* RemoveAtTail()
	{
		if (ElementCount == 0)
		{
			return nullptr;
		}

		NodeType* CurrentNode = Head;

		while (CurrentNode->Next->Next != nullptr)
		{
			CurrentNode = CurrentNode->Next;
		}

		delete CurrentNode->Next;
		CurrentNode->Next = nullptr;

		--ElementCount;
		return CurrentNode;
	}

	ElementType PopFront()
	{
		TRNT_ASSERT(ElementCount != 0);

		ElementType Result{ Head->Value };

		NodeType* Tmp = Head->Next;
		delete Head;
		Head = Tmp;

		--ElementCount;
		return Result;
	}

private:
	NodeType* Head;
	SizeType ElementCount;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// TDoublyLinkedList /////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename Type>
class TDoublyLinkedListNode
{
public:
	template<typename... ArgumentsType>
	TRNT_FORCE_INLINE TDoublyLinkedListNode(ArgumentsType&&... Args)
		: Value(Forward<ArgumentsType>(Args)...)
	{}

	~TDoublyLinkedListNode() = default;

	Type Value;
	TDoublyLinkedListNode* Next = nullptr;
	TDoublyLinkedListNode* Previous = nullptr;
};

template<typename NodeType, typename ElementType>
class TDoublyLinkedListIterator : public TLinkedListIteratorBase<TDoublyLinkedListIterator, NodeType, ElementType>
{
public:
	using Super = TLinkedListIteratorBase<TDoublyLinkedListIterator, NodeType, ElementType>;

public:
	TRNT_FORCE_INLINE TDoublyLinkedListIterator(NodeType* CurrentNode)
		: Super(CurrentNode)
	{}

public:
	TRNT_FORCE_INLINE TDoublyLinkedListIterator& operator++()
	{
		TRNT_ASSERT_IS_NOT_NULL(this->CurrentNode);
		this->CurrentNode = this->CurrentNode->Next;
		return *this;
	}

	TRNT_FORCE_INLINE TDoublyLinkedListIterator operator++(int)
	{
		TRNT_ASSERT_IS_NOT_NULL(this->CurrentNode);
		TDoublyLinkedListIterator Tmp{ this->CurrentNode };
		this->CurrentNode = this->CurrentNode->Next;
		return Tmp;
	}

	TRNT_FORCE_INLINE TDoublyLinkedListIterator& operator--()
	{
		TRNT_ASSERT_IS_NOT_NULL(this->CurrentNode);
		this->CurrentNode = this->CurrentNode->Previous;
		return *this;
	}

	TRNT_FORCE_INLINE TDoublyLinkedListIterator operator--(int)
	{
		TRNT_ASSERT_IS_NOT_NULL(this->CurrentNode);
		TDoublyLinkedListIterator Tmp{ this->CurrentNode };
		this->CurrentNode = this->CurrentNode->Previous;
		return Tmp;
	}
};

template<typename Type>
class TDoublyLinkedList
{
public:
	using LinkedListHelper = TLinkedListHelper<TDoublyLinkedList<Type>>;

	friend class LinkedListHelper;

	using ElementType = Type;

	using ReferenceType = ElementType&;
	using ConstReferenceType = const ElementType&;

	using PointerType = ElementType*;
	using ConstPointerType = const ElementType*;

	using SizeType = TInt32;

	using NodeType = TDoublyLinkedListNode<ElementType>;

	using IteratorType = TDoublyLinkedListIterator<NodeType, ElementType>;
	using ConstIteratorType = TDoublyLinkedListIterator<const NodeType, const ElementType>;

public:
	TRNT_FORCE_INLINE TDoublyLinkedList()
		: Head(nullptr), Tail(nullptr), ElementCount(0)
	{}

	TRNT_FORCE_INLINE TDoublyLinkedList(TDoublyLinkedList&& List) noexcept
		: Head(List.Head), Tail(List.Tail), ElementCount(List.ElementCount)
	{
		List.ElementCount = 0;
		List.Head = nullptr;
		List.Tail = nullptr;
	}

	TDoublyLinkedList(const TDoublyLinkedList& List)
		: ElementCount(List.ElementCount)
	{
		if (List.Head == nullptr)
		{
			Head = Tail = nullptr;
		}
		else
		{
			NodeType* CurrentListNode = List.Head;
			Head = Tail = new NodeType(CurrentListNode->Value);
			CurrentListNode = CurrentListNode->Next;

			NodeType* CurrentThisListNode = this->Head;
			while (CurrentListNode != nullptr)
			{
				CurrentThisListNode->Next = new NodeType(CurrentListNode->Value);
				CurrentThisListNode->Next->Previous = CurrentThisListNode;

				CurrentThisListNode = CurrentThisListNode->Next;

				CurrentListNode = CurrentListNode->Next;
			}

			Tail = CurrentThisListNode;
		}
	}

	TDoublyLinkedList(std::initializer_list<ElementType> List)
		: ElementCount((SizeType)List.size())
	{
		if (ElementCount == 0)
		{
			Head = Tail = nullptr;
		}
		else
		{
			ConstPointerType ListBegin = List.begin();
			ConstPointerType ListEnd = List.end();

			Head = Tail = new NodeType(*ListBegin++);
			NodeType* CurrentNode = Head;

			while (ListBegin != ListEnd)
			{
				CurrentNode->Next = new NodeType(*ListBegin);
				CurrentNode->Next->Previous = CurrentNode;

				CurrentNode = CurrentNode->Next;
				++ListBegin;
			}

			Tail = CurrentNode;
		}
	}

	~TDoublyLinkedList()
	{
		LinkedListHelper::Clear(*this);
	}

public:
	void Clear()
	{
		LinkedListHelper::Clear(*this);
	}

public:
	TBool operator==(const TDoublyLinkedList& Other) const
	{
		return LinkedListHelper::AreEqual(*this, Other);
	}

	TRNT_FORCE_INLINE TBool operator!=(const TDoublyLinkedList& Other) const
	{
		return !LinkedListHelper::AreEqual(*this, Other);
	}

public:
	TDoublyLinkedList& operator=(const TDoublyLinkedList& Other)
	{
		if (this != &Other)
		{
			if (Head != nullptr)
			{
				NodeType* Node;
				while (Head != nullptr)
				{
					Node = Head->Next;
					delete Head;
					Head = Node;
				}
			}

			if (Other.Head != nullptr)
			{
				NodeType* CurrentListNode = Other.Head;
				Head = Tail = new NodeType(CurrentListNode->Value);
				NodeType* CurrentThisListNode = this->Head;

				CurrentListNode = CurrentListNode->Next;

				while (CurrentListNode != nullptr)
				{
					CurrentThisListNode->Next = new NodeType(CurrentListNode->Value);
					CurrentThisListNode->Next->Previous = CurrentThisListNode;

					CurrentThisListNode = CurrentThisListNode->Next;

					CurrentListNode = CurrentListNode->Next;
				}

				Tail = CurrentThisListNode;
			}

			ElementCount = Other.ElementCount;
		}
		return *this;
	}

	TDoublyLinkedList& operator=(TDoublyLinkedList&& Other) noexcept
	{
		if (this != &Other)
		{
			if (Head != nullptr)
			{
				NodeType* Node;
				while (Head != nullptr)
				{
					Node = Head->Next;
					delete Head;
					Head = Node;
				}
			}

			Head = Other.Head;
			Tail = Other.Tail;
			ElementCount = Other.ElementCount;

			Other.Head = nullptr;
			Other.Tail = nullptr;
			Other.ElementCount = 0;
		}
		return *this;
	}

	TDoublyLinkedList& operator=(std::initializer_list<ElementType> List)
	{
		ElementCount = (SizeType)List.size();

		if (Head != nullptr)
		{
			NodeType* Node;
			while (Head != nullptr)
			{
				Node = Head->Next;
				delete Head;
				Head = Node;
			}
		}

		if (ElementCount > 0)
		{
			ConstPointerType ListBegin = List.begin();
			ConstPointerType ListEnd = List.end();

			Head = Tail = new NodeType(*ListBegin++);
			NodeType* CurrentNode = Head;

			while (ListBegin != ListEnd)
			{
				CurrentNode->Next = new NodeType(*ListBegin);
				CurrentNode->Next->Previous = CurrentNode;

				CurrentNode = CurrentNode->Next;
				++ListBegin;
			}

			Tail = CurrentNode;
		}

		return *this;
	}


public:
	TRNT_FORCE_INLINE SizeType Length() const
	{
		return ElementCount;
	}

	TRNT_FORCE_INLINE TBool IsEmpty() const
	{
		return ElementCount == 0;
	}

	TRNT_FORCE_INLINE ReferenceType First()
	{
		TRNT_ASSERT(ElementCount > 0);
		return Head->Value;
	}

	TRNT_FORCE_INLINE ConstReferenceType First() const
	{
		TRNT_ASSERT(ElementCount > 0);
		return Head->Value;
	}

	TRNT_FORCE_INLINE ReferenceType Last()
	{
		TRNT_ASSERT(ElementCount > 0);
		return Tail->Value;
	}

	TRNT_FORCE_INLINE ConstReferenceType Last() const
	{
		TRNT_ASSERT(ElementCount > 0);
		return Tail->Value;
	}

	TRNT_FORCE_INLINE NodeType* GetHead()
	{
		return Head;
	}

	TRNT_FORCE_INLINE const NodeType* GetHead() const
	{
		return Head;
	}

	TRNT_FORCE_INLINE NodeType* GetTail()
	{
		return Tail;
	}

	TRNT_FORCE_INLINE const NodeType* GetTail() const
	{
		return Tail;
	}

	TRNT_FORCE_INLINE IteratorType begin() noexcept { return IteratorType(Head); }
	TRNT_FORCE_INLINE ConstIteratorType begin() const noexcept { return ConstIteratorType(Head); }
	TRNT_FORCE_INLINE ConstIteratorType cbegin() const noexcept { return ConstIteratorType(Head); }

	TRNT_FORCE_INLINE IteratorType end() noexcept { return IteratorType(nullptr); }
	TRNT_FORCE_INLINE ConstIteratorType end() const noexcept { return ConstIteratorType(nullptr); }
	TRNT_FORCE_INLINE ConstIteratorType cend() const noexcept { return ConstIteratorType(nullptr); }

public:
	TRNT_FORCE_INLINE NodeType* Find(ConstReferenceType Element) const
	{
		return LinkedListHelper::Find(*this, Element);
	}

	TRNT_FORCE_INLINE TBool Contains(ConstReferenceType Element) const
	{
		return LinkedListHelper::Contains(*this, Element);
	}

	template<typename PredicateType>
	TRNT_FORCE_INLINE NodeType* FindIf(PredicateType Predicate) const
	{
		return LinkedListHelper::FindIf(*this, Predicate);
	}

	template<typename PredicateType>
	TRNT_FORCE_INLINE TBool ContainsIf(PredicateType Predicate) const
	{
		return LinkedListHelper::ContainsIf(*this, Predicate);
	}

public:
	template<typename... ArgumentsType>
	NodeType* EmplaceAtHead(ArgumentsType&&... Args)
	{
		NodeType* NewNode = new NodeType(Forward<ArgumentsType>(Args)...);

		NewNode->Next = Head;

		if (Head != nullptr)
		{
			NewNode->Next->Previous = NewNode;
		}
		else
		{
			Tail = NewNode;
		}

		Head = NewNode;
		++ElementCount;
		return Head;
	}

	template<typename... ArgumentsType>
	NodeType* EmplaceAtTail(ArgumentsType&&... Args)
	{
		NodeType* NewNode = new NodeType(Forward<ArgumentsType>(Args)...);

		NewNode->Previous = Tail;

		if (Tail != nullptr)
		{
			NewNode->Previous->Next = NewNode;
		}
		else
		{
			Head = NewNode;
		}

		Tail = NewNode;
		++ElementCount;
		return Tail;
	}

	template<typename... ArgumentsType>
	NodeType* EmplaceAt(NodeType* NodePosition, ArgumentsType&&... Args)
	{
		if (NodePosition == nullptr || NodePosition == Head)
		{
			return EmplaceAtHead(Forward<ArgumentsType>(Args)...);
		}

		NodeType* NewNode = new NodeType(Forward<ArgumentsType>(Args)...);

		NewNode->Previous = NodePosition->Previous;
		NewNode->Next = NodePosition;

		NodePosition->Previous->Next = NewNode;
		NodePosition->Previous = NewNode;

		++ElementCount;
		return NewNode;
	}

	NodeType* InsertAt(NodeType* NodePosition, NodeType* NewNode)
	{
		if (NodePosition == nullptr || NodePosition == Head)
		{
			return InsertAtHead(NewNode);
		}

		NewNode->Previous = NodePosition->Previous;
		NewNode->Next = NodePosition;

		NodePosition->Previous->Next = NewNode;
		NodePosition->Previous = NewNode;

		++ElementCount;

		return NewNode;
	}

	NodeType* InsertAtHead(NodeType* NewNode)
	{
		if (NewNode == nullptr)
		{
			return nullptr;
		}

		NewNode->Next = Head;

		if (Head != nullptr)
		{
			NewNode->Next->Previous = NewNode;
		}
		else
		{
			Tail = NewNode;
		}

		Head = NewNode;
		++ElementCount;
		return Head;
	}

	NodeType* InsertAtTail(NodeType* NewNode)
	{
		if (NewNode == nullptr)
		{
			return nullptr;
		}

		NewNode->Previous = Tail;

		if (Tail != nullptr)
		{
			NewNode->Previous->Next = NewNode;
		}
		else
		{
			Head = NewNode;
		}

		Tail = NewNode;
		++ElementCount;
		return Tail;
	}

	NodeType* InsertAtHead(ConstReferenceType Element)
	{
		return InsertAtHead(new NodeType(Element));
	}

	NodeType* InsertAtHead(ElementType&& Element)
	{
		return InsertAtHead(new NodeType(Move(Element)));
	}

	NodeType* InsertAtTail(ConstReferenceType Element)
	{
		return InsertAtTail(new NodeType(Element));
	}

	NodeType* InsertAtTail(ElementType&& Element)
	{
		return InsertAtTail(new NodeType(Move(Element)));
	}

	NodeType* InsertAt(NodeType* NodePosition, ConstReferenceType Element)
	{
		return InsertAt(NodePosition, new NodeType(Element));
	}

	NodeType* InsertAt(NodeType* NodePosition, ElementType&& Element)
	{
		return InsertAt(NodePosition, new NodeType(Move(Element)));
	}

	NodeType* RemoveAtHead()
	{
		if (ElementCount == 0)
		{
			return nullptr;
		}

		NodeType* OldHead = Head->Next;
		delete Head;
		Head = OldHead;

		if (Head == nullptr)
		{
			Tail = nullptr;
		}
		else
		{
			Head->Previous = nullptr;
		}
		--ElementCount;
		return Head;
	}

	NodeType* RemoveAtTail()
	{
		if (ElementCount == 0)
		{
			return nullptr;
		}

		NodeType* OldTail = Tail->Previous;
		delete Tail;
		Tail = OldTail;

		if (Tail == nullptr)
		{
			Head = nullptr;
		}
		else
		{
			Tail->Next = nullptr;
		}
		--ElementCount;
		return Tail;
	}

	NodeType* RemoveAt(NodeType* NodePosition)
	{
		if (ElementCount == 0 /*&& NodePosition == nullptr*/)
		{
			return nullptr;
		}

		NodeType* Ret = nullptr;
		if (NodePosition == Head)
		{
			Head = Head->Next;
			Ret = Head;
		}
		else if (NodePosition == Tail)
		{
			Tail = Tail->Previous;
			Ret = Tail;
		}
		else
		{
			Ret = NodePosition->Next;
			NodePosition->Previous->Next = NodePosition->Next;
			NodePosition->Next->Previous = NodePosition->Previous;
		}

		if (Head != nullptr)
		{
			Head->Previous = nullptr;
			Tail->Next = nullptr;
		}
		else
		{
			Tail = nullptr;
		}

		delete NodePosition;
		--ElementCount;

		return Ret;
	}

	ElementType PopFront()
	{
		TRNT_ASSERT(ElementCount != 0);
		ElementType Result{ Head->Value };
		RemoveAtHead();
		return Result;
	}

	ElementType PopBack()
	{
		TRNT_ASSERT(ElementCount != 0);
		ElementType Result{ Tail->Value };
		RemoveAtTail();
		return Result;
	}

private:
	NodeType* Head;
	NodeType* Tail;
	TInt32 ElementCount;
};