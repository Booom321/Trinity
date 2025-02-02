#pragma once

#include "Trinity/Core/Containers/LinkedList.h"
#include "Trinity/Core/Hash/HashFunctions.h"
#include "Trinity/Core/Types/DataTypes.h"
#include "Trinity/Core/Types/Pair.h"

#pragma warning(push)
#pragma warning(disable : 6011)

template<typename KeyType>
class TDefaultKeyHasher
{
public:
	static TRNT_FORCE_INLINE TSize_T GetHashCode(const KeyType& Key)
	{
		return TNsHash::GetHashCode(Key);
	}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename KeyType, typename ValueType>
class THashMapElement
{
public:
	using HashCodeType = TSize_T;

public:
	TRNT_FORCE_INLINE THashMapElement(HashCodeType HashCode, const KeyType& Key, const ValueType& Value)
		: HashCode(HashCode), Key(Key), Value(Value)
	{}

	TRNT_FORCE_INLINE THashMapElement(HashCodeType HashCode, KeyType&& Key, ValueType&& Value) noexcept
		: HashCode(HashCode), Key(Move(Key)), Value(Move(Value))
	{}

	TRNT_FORCE_INLINE THashMapElement(HashCodeType HashCode, const KeyType& Key)
		: HashCode(HashCode), Key(Key), Value()
	{}

	KeyType Key;
	ValueType Value;
	HashCodeType HashCode;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename KeyT, typename ValueT, template<typename> typename KeyHasher = TDefaultKeyHasher>
class THashMap
{
public:
	using KeyType = KeyT;
	using ValueType = ValueT;

	using SizeType = TInt64;
	using HashCodeType = typename THashMapElement<KeyType, ValueType>::HashCodeType;
	using ElementType = THashMapElement<KeyType, ValueType>;

	using KeyHasherType = KeyHasher<KeyType>;

	using PointerType = ElementType*;
	using ReferenceType = ElementType&;
	using ConstPointerType = const ElementType*;
	using ConstReferenceType = const ElementType&;

	using PairType = TPair<KeyType, ValueType>;
	using ElementListType = TDoublyLinkedList<ElementType>;

	static TRNT_CONSTEXPR TFloat MaxLoadFactor = .75f;
	static TRNT_CONSTEXPR TFloat InvalidLoadFactor = -1.0f;
	static TRNT_CONSTEXPR SizeType InitialBucketCount = 8;

private:
	template<typename HashMapIteratorType>
	static void AdvanceIteratorToValidBucket(HashMapIteratorType* Iterator)
	{
		const ElementListType* LastBucket = Iterator->CurrentMap->Buckets + Iterator->CurrentMap->BucketCount;
		const ElementListType* Tmp = Iterator->CurrentBucket;

		while (Iterator->CurrentBucket < LastBucket && Iterator->CurrentBucket->IsEmpty())
		{
			++Iterator->CurrentBucket;
		}

		Iterator->CurrentBucketElement = Iterator->CurrentBucket->GetHead();

		if (Iterator->CurrentBucket >= LastBucket)
		{
			Iterator->CurrentBucketElement = nullptr;
		}
	}

private:
	template<typename T, TBool NeedToConvert>
	class TNeedToConvertPointerToConstPointer;

	template<typename T>
	class TNeedToConvertPointerToConstPointer<T, true>
	{
	public:
		using Type = const T*;
	};

	template<typename T>
	class TNeedToConvertPointerToConstPointer<T, false>
	{
	public:
		using Type = T*;
	};

private:
	template<TBool IsConst>
	class THashMapIteratorBase
	{
	public:
		template<typename HashMapIteratorType>
		static friend void AdvanceIteratorToValidBucket(const HashMapIteratorType* Iterator);

		using HashMapPointerType = typename TNeedToConvertPointerToConstPointer<THashMap, IsConst>::Type;
		using BucketPointerType = typename TNeedToConvertPointerToConstPointer<ElementListType, IsConst>::Type;
		using BucketElementType = typename TNeedToConvertPointerToConstPointer<typename ElementListType::NodeType, IsConst>::Type;

		TRNT_FORCE_INLINE THashMapIteratorBase(HashMapPointerType MapPtr, BucketPointerType BucketPtr, BucketElementType BucketElementPtr)
			: CurrentMap(MapPtr), CurrentBucket(BucketPtr), CurrentBucketElement(BucketElementPtr)
		{
			AdvanceIteratorToValidBucket<THashMapIteratorBase>(this);
		}

		TRNT_FORCE_INLINE THashMapIteratorBase& operator=(const THashMapIteratorBase& It)
		{
			this->CurrentMap = It.CurrentMap;
			this->CurrentBucket = It.CurrentBucket;
			this->CurrentBucketElement = It.CurrentBucketElement;
			// AdvanceIteratorToValidBucket<THashMapIteratorBase>(this);
			return *this;
		}

	public:
		TRNT_FORCE_INLINE const ElementType& operator*() const
		{
			return CurrentBucketElement->Value;
		}

		TRNT_FORCE_INLINE const ElementType* operator->() const
		{
			return &CurrentBucketElement->Value;
		}

	public:
		TRNT_FORCE_INLINE TBool operator==(const THashMapIteratorBase& It) const
		{
			return CurrentMap == It.CurrentMap && CurrentBucket == It.CurrentBucket && CurrentBucketElement == It.CurrentBucketElement;
		}

		TRNT_FORCE_INLINE TBool operator!=(const THashMapIteratorBase& It) const
		{
			return CurrentMap != It.CurrentMap || CurrentBucket != It.CurrentBucket || CurrentBucketElement != It.CurrentBucketElement;
		}

	public:
		TRNT_FORCE_INLINE THashMapIteratorBase& operator++()
		{
			if (CurrentBucketElement != nullptr)
			{
				CurrentBucketElement = CurrentBucketElement->Next;
			}

			if (CurrentBucketElement == nullptr)
			{
				++CurrentBucket;

				if (CurrentBucket < (CurrentMap->Buckets + CurrentMap->BucketCount))
				{
					CurrentBucketElement = CurrentBucket->GetHead();

					if (CurrentBucketElement == nullptr)
					{
						AdvanceIteratorToValidBucket<THashMapIteratorBase>(this);
					}
				}
			}

			return *this;
		}

		TRNT_FORCE_INLINE THashMapIteratorBase operator++(int) const
		{
			THashMapIteratorBase Iterator(*this);
			++Iterator;

			return Iterator;
		}

	public:
		HashMapPointerType CurrentMap;
		BucketPointerType CurrentBucket;
		BucketElementType CurrentBucketElement;
	};

public:
	class TIterator : public THashMapIteratorBase<false>
	{
	public:
		using Super = THashMapIteratorBase<false>;

		TRNT_FORCE_INLINE TIterator(THashMap* MapPtr)
			: Super(MapPtr, MapPtr->Buckets, MapPtr->Buckets->GetHead())
		{}

		TRNT_FORCE_INLINE TIterator(THashMap* MapPtr, ElementListType* BucketPtr, typename ElementListType::NodeType* BucketElementPtr)
			: Super(MapPtr, BucketPtr, BucketElementPtr)
		{}

		TRNT_FORCE_INLINE TIterator(const TIterator& It)
			: Super(It.CurrentMap, It.CurrentBucket, It.CurrentBucketElement)
		{}

		TRNT_FORCE_INLINE ElementType& operator*()
		{
			return this->CurrentBucketElement->Value;
		}

		TRNT_FORCE_INLINE ElementType* operator->()
		{
			return &this->CurrentBucketElement->Value;
		}
	};

	class TConstIterator : public THashMapIteratorBase<true>
	{
	public:
		using Super = THashMapIteratorBase<true>;

		TRNT_FORCE_INLINE TConstIterator(const THashMap* MapPtr)
			: Super(MapPtr, MapPtr->Buckets, MapPtr->Buckets->GetHead())
		{}

		TRNT_FORCE_INLINE TConstIterator(const THashMap* MapPtr, const ElementListType* BucketPtr, const typename ElementListType::NodeType* BucketElementPtr)
			: Super(MapPtr, BucketPtr, BucketElementPtr)
		{}

		TRNT_FORCE_INLINE TConstIterator(const TIterator& It)
			: Super(It.CurrentMap, It.CurrentBucket, It.CurrentBucketElement)
		{}
	};

public:
	THashMap()
		: ElementCount(0),
		  BucketCount(InitialBucketCount),
		  Buckets(static_cast<ElementListType*>(malloc(InitialBucketCount * sizeof(ElementListType))))
	{
		for (SizeType Index = 0; Index < InitialBucketCount; ++Index)
		{
			new (Buckets + Index) ElementListType();
		}
	}

	THashMap(const THashMap& Other)
		: ElementCount(Other.ElementCount),
		  BucketCount(Other.BucketCount),
		  Buckets(static_cast<ElementListType*>(malloc(Other.BucketCount * sizeof(ElementListType))))
	{
		for (SizeType Index = 0; Index < BucketCount; ++Index)
		{
			new (Buckets + Index) ElementListType(Other.Buckets[Index]);
		}
	}

	THashMap(THashMap&& Other) noexcept
		: ElementCount(Other.ElementCount),
		  BucketCount(Other.BucketCount),
		  Buckets(Other.Buckets)
	{
		Other.ElementCount = 0;
		Other.BucketCount = 0;
		Other.Buckets = nullptr;
	}

	explicit THashMap(const SizeType InputBucketCount)
		: ElementCount(0),
		  BucketCount(InputBucketCount <= 0 ? InitialBucketCount : InputBucketCount),
		  Buckets(static_cast<ElementListType*>(malloc(BucketCount * sizeof(ElementListType))))
	{
		for (SizeType Index = 0; Index < BucketCount; ++Index)
		{
			new (Buckets + Index) ElementListType();
		}
	}

	THashMap(std::initializer_list<PairType> Pairs)
		: ElementCount(0),
		  BucketCount(InitialBucketCount),
		  Buckets(static_cast<ElementListType*>(malloc(InitialBucketCount * sizeof(ElementListType))))
	{
		for (SizeType Index = 0; Index < BucketCount; ++Index)
		{
			new (Buckets + Index) ElementListType();
		}
		Insert(Pairs);
	}

	~THashMap()
	{
		if (Buckets == nullptr || BucketCount == 0)
		{
			return;
		}

		for (SizeType Index = 0; Index < BucketCount; ++Index)
		{
			(Buckets + Index)->~ElementListType();
		}

		free(Buckets);
		Buckets = nullptr;
		BucketCount = 0;
		ElementCount = 0;
	}

public:
	THashMap& operator=(const THashMap& Other)
	{
		if (this != &Other)
		{
			if (BucketCount >= Other.BucketCount)
			{
				Clear();
				typename ElementListType::NodeType* Node;
				for (SizeType BucketIndex = 0; BucketIndex < Other.BucketCount; ++BucketIndex)
				{
					if (!Other.Buckets[BucketIndex].IsEmpty())
					{
						Node = Other.Buckets[BucketIndex].GetHead();

						while (Node != nullptr)
						{
							Buckets[Node->Value.HashCode % BucketCount].EmplaceAtTail(Node->Value.HashCode, Node->Value.Key, Node->Value.Value);

							Node = Node->Next;
						}
					}
				}
			}
			else // BucketCount < Other.BucketCount
			{
				for (SizeType Index = 0; Index < BucketCount; ++Index)
				{
					(Buckets + Index)->~ElementListType();
				}

				free(Buckets);

				Buckets = static_cast<ElementListType*>(malloc(Other.BucketCount * sizeof(ElementListType)));
				for (SizeType BucketIndex = 0; BucketIndex < Other.BucketCount; ++BucketIndex)
				{
					new (Buckets + BucketIndex) ElementListType(Other.Buckets[BucketIndex]);
				}
			}

			ElementCount = Other.ElementCount;
			BucketCount = Other.BucketCount;
		}

		return *this;
	}

	THashMap& operator=(THashMap&& Other) noexcept
	{
		if (this != &Other)
		{
			for (SizeType Index = 0; Index < BucketCount; ++Index)
			{
				(Buckets + Index)->~ElementListType();
			}

			free(Buckets);

			Buckets = Other.Buckets;
			ElementCount = Other.ElementCount;
			BucketCount = Other.BucketCount;

			Other.Buckets = nullptr;
			Other.ElementCount = 0;
			Other.BucketCount = 0;
		}

		return *this;
	}

	THashMap& operator=(std::initializer_list<PairType> Pairs)
	{
		Clear();
		Insert(Pairs);
		return *this;
	}

public:
	void Clear()
	{
		if (Buckets == nullptr || BucketCount == 0)
		{
			return;
		}

		for (SizeType Index = 0; Index < BucketCount; ++Index)
		{
			(Buckets + Index)->Clear();
		}

		ElementCount = 0;
	}

	TRNT_FORCE_INLINE SizeType GetElementCount() const noexcept
	{
		return ElementCount;
	}

	TRNT_FORCE_INLINE SizeType GetBucketCount() const noexcept
	{
		return BucketCount;
	}

	TRNT_FORCE_INLINE SizeType GetBucketElementCount(SizeType BucketIndex) const
	{
		TRNT_ASSERT(BucketIndex >= 0 && BucketIndex < BucketCount);
		return Buckets[BucketIndex].GetElementCount();
	}

	TRNT_FORCE_INLINE SizeType GetBucketIndexByKey(const KeyType& Key) const
	{
		return KeyHasherType::GetHashCode(Key) % BucketCount;
	}

	TRNT_FORCE_INLINE TBool IsEmpty() const noexcept
	{
		return ElementCount == 0;
	}

	TRNT_FORCE_INLINE TBool IsBucketEmpty(SizeType BucketIndex) const noexcept
	{
		return Buckets[BucketIndex].IsEmpty();
	}

	TRNT_FORCE_INLINE const ElementListType* GetBucketData() const noexcept
	{
		return Buckets;
	}

public:
	TRNT_FORCE_INLINE TFloat GetLoadFactor() const
	{
		if (!BucketCount)
		{
			return InvalidLoadFactor;
		}
		return static_cast<TFloat>(ElementCount) / static_cast<TFloat>(BucketCount);
	}

	static TRNT_FORCE_INLINE constexpr TFloat GetMaxLoadFactor()
	{
		return MaxLoadFactor;
	}

public:
	typename ElementListType::NodeType* FindByHash(HashCodeType HashCode)
	{
		typename ElementListType::NodeType* Node = Buckets[HashCode % BucketCount].GetHead();

		while (Node != nullptr)
		{
			if (Node->Value.HashCode == HashCode)
			{
				return Node;
			}

			Node = Node->Next;
		}

		return nullptr;
	}

	const typename ElementListType::NodeType* FindByHash(const HashCodeType HashCode) const
	{
		const typename ElementListType::NodeType* Node = Buckets[HashCode % BucketCount].GetHead();

		while (Node != nullptr)
		{
			if (Node->Value.HashCode == HashCode)
			{
				return Node;
			}

			Node = Node->Next;
		}

		return nullptr;
	}

	TRNT_FORCE_INLINE const typename ElementListType::NodeType* Find(const KeyType& Key) const
	{
		const HashCodeType HashCode = KeyHasherType::GetHashCode(Key);
		return FindByHash(HashCode);
	}

	TRNT_FORCE_INLINE typename ElementListType::NodeType* Find(const KeyType& Key)
	{
		HashCodeType HashCode = KeyHasherType::GetHashCode(Key);
		return FindByHash(HashCode);
	}

public:
	TRNT_NODISCARD ValueType& operator[](const KeyType& Key)
	{
		HashCodeType HashCode = KeyHasherType::GetHashCode(Key);
		typename ElementListType::NodeType* FoundNode = FindByHash(HashCode);

		if (FoundNode)
		{
			return FoundNode->Value.Value;
		}

		return EmplaceByHash(HashCode, Key)->Value.Value;
	}

	TRNT_NODISCARD ValueType& operator[](KeyType&& Key)
	{
		HashCodeType HashCode = KeyHasherType::GetHashCode(Key);
		typename ElementListType::NodeType* FoundNode = FindByHash(HashCode);

		if (FoundNode)
		{
			return FoundNode->Value.Value;
		}

		return EmplaceByHash(HashCode, Move(Key))->Value.Value;
	}

	TRNT_NODISCARD ValueType& At(const KeyType& Key)
	{
		typename ElementListType::NodeType* FoundNode = FindByHash(KeyHasherType::GetHashCode(Key));
		TRNT_ASSERT_IS_NOT_NULL(FoundNode);
		return FoundNode->Value.Value;
	}

	TRNT_NODISCARD const ValueType& At(const KeyType& Key) const
	{
		typename ElementListType::NodeType* FoundNode = FindByHash(KeyHasherType::GetHashCode(Key));
		TRNT_ASSERT_IS_NOT_NULL(FoundNode);
		return FoundNode->Value.Value;
	}

public:
	TRNT_FORCE_INLINE TBool Contains(const KeyType& Key) const
	{
		const HashCodeType HashCode = KeyHasherType::GetHashCode(Key);
		return FindByHash(HashCode) != nullptr;
	}

	TRNT_FORCE_INLINE TBool ContainsByHash(const HashCodeType HashCode) const
	{
		return FindByHash(HashCode) != nullptr;
	}

	TRNT_FORCE_INLINE TInt32 Count(const KeyType& Key) const
	{
		const HashCodeType HashCode = KeyHasherType::GetHashCode(Key);

		return FindByHash(HashCode) != nullptr ? 1 : 0;
	}

	TRNT_FORCE_INLINE TInt32 CountByHash(const HashCodeType HashCode) const
	{
		return FindByHash(HashCode) != nullptr ? 1 : 0;
	}

public:
	using IteratorType = TIterator;
	using ConstIteratorType = TConstIterator;

	TRNT_FORCE_INLINE IteratorType begin()
	{
		return IteratorType(this, Buckets, Buckets->GetHead());
	}

	TRNT_FORCE_INLINE IteratorType end()
	{
		ElementListType* LastBucket = Buckets + BucketCount;
		return IteratorType(this, LastBucket, nullptr);
	}

	TRNT_FORCE_INLINE ConstIteratorType begin() const
	{
		return ConstIteratorType(this, Buckets, Buckets->GetHead());
	}

	TRNT_FORCE_INLINE ConstIteratorType end() const
	{
		const ElementListType* LastBucket = Buckets + BucketCount;
		return ConstIteratorType(this, LastBucket, nullptr);
	}

	TRNT_FORCE_INLINE ConstIteratorType cbegin() const
	{
		return ConstIteratorType(this, Buckets, Buckets->GetHead());
	}

	TRNT_FORCE_INLINE ConstIteratorType cend() const
	{
		const ElementListType* LastBucket = Buckets + BucketCount;
		return ConstIteratorType(this, LastBucket, nullptr);
	}

public:
	TBool operator==(const THashMap& Other) const
	{
		if (ElementCount != Other.ElementCount)
		{
			return false;
		}

		typename ElementListType::NodeType* Node;
		for (SizeType BucketIndex = 0; BucketIndex < BucketCount; ++BucketIndex)
		{
			Node = Buckets[BucketIndex].GetHead();

			if (Node == nullptr)
			{
				continue;
			}

			while (Node != nullptr)
			{
				const typename ElementListType::NodeType* FoundNode = Other.FindByHash(Node->Value.HashCode);

				if (FoundNode == nullptr)
				{
					return false;
				}

				if (FoundNode->Value.Value != Node->Value.Value)
				{
					return false;
				}

				Node = Node->Next;
			}
		}

		return true;
	}

	TRNT_FORCE_INLINE TBool operator!=(const THashMap& Other) const
	{
		return !operator==(Other);
	}

public:
	void Rehash(SizeType InputBucketCount)
	{
		SizeType Num = static_cast<SizeType>(static_cast<TFloat>(ElementCount) / MaxLoadFactor) + 1;
		InputBucketCount = TRNT_MAX(InputBucketCount, Num);

		if (InputBucketCount <= BucketCount)
		{
			return;
		}

		SizeType OldBucketCount = BucketCount;
		BucketCount = InputBucketCount;

		ElementListType* Temp = static_cast<ElementListType*>(malloc(BucketCount * sizeof(ElementListType)));

		for (SizeType BucketIndex = 0; BucketIndex < BucketCount; ++BucketIndex)
		{
			new (Temp + BucketIndex) ElementListType();
		}

		typename ElementListType::NodeType* Node;
		for (SizeType BucketIndex = 0; BucketIndex < OldBucketCount; ++BucketIndex)
		{
			Node = Buckets[BucketIndex].GetHead();

			while (Node != nullptr)
			{
				Temp[Node->Value.HashCode % BucketCount].InsertAtTail(new typename ElementListType::NodeType(
					Node->Value.HashCode,
					Move(Node->Value.Key),
					Move(Node->Value.Value)));
			}

			Buckets[BucketIndex].~ElementListType();
		}

		free(Buckets);
		Buckets = Temp;
	}

private:
	void PrivateRehash()
	{
		SizeType OldBucketCount = BucketCount;
		BucketCount = static_cast<SizeType>(static_cast<TFloat>(ElementCount + 1) / MaxLoadFactor) + 1;

		ElementListType* Temp = static_cast<ElementListType*>(malloc(BucketCount * sizeof(ElementListType)));

		for (SizeType BucketIndex = 0; BucketIndex < BucketCount; ++BucketIndex)
		{
			new (Temp + BucketIndex) ElementListType();
		}

		typename ElementListType::NodeType* Node;
		for (SizeType BucketIndex = 0; BucketIndex < OldBucketCount; ++BucketIndex)
		{
			Node = Buckets[BucketIndex].GetHead();

			while (Node != nullptr)
			{
				Temp[Node->Value.HashCode % BucketCount].InsertAtTail(new typename ElementListType::NodeType(
					Node->Value.HashCode,
					Move(Node->Value.Key),
					Move(Node->Value.Value)));

				Node = Node->Next;
			}

			Buckets[BucketIndex].~ElementListType();
		}

		free(Buckets);
		Buckets = Temp;
	}

	template<typename K>
	typename ElementListType::NodeType* InsertOrAssignImpl(K Key, ValueType&& Value)
	{
		if ((static_cast<TFloat>(ElementCount + 1) / static_cast<TFloat>(BucketCount)) > MaxLoadFactor)
		{
			PrivateRehash();
		}

		HashCodeType HashCode = KeyHasherType::GetHashCode(Key);
		typename ElementListType::NodeType* FoundNode = FindByHash(HashCode);

		if constexpr (TAreTheSameType<KeyType&, std::remove_const<K>::type>::Value)
		{
			if (FoundNode == nullptr) // Key not found
			{
				++ElementCount;
				return Buckets[HashCode % BucketCount].InsertAtTail(new typename ElementListType::NodeType(HashCode, Key, Move(Value)));
			}

			FoundNode->Value.Value = Move(Value);
		}
		else if constexpr (TAreTheSameType<KeyType&&, K>::Value)
		{
			if (FoundNode == nullptr) // Key not found
			{
				++ElementCount;
				return Buckets[HashCode % BucketCount].InsertAtTail(new typename ElementListType::NodeType(HashCode, Move(Key), Move(Value)));
			}

			FoundNode->Value.Value = Move(Value);
		}

		return FoundNode;
	}

public:
	template<typename... ArgsType>
	typename ElementListType::NodeType* Emplace(ArgsType&&... Args)
	{
		if ((static_cast<TFloat>(ElementCount + 1) / static_cast<TFloat>(BucketCount)) > MaxLoadFactor)
		{
			PrivateRehash();
		}

		PairType Pair(Forward<ArgsType>(Args)...);
		HashCodeType HashCode = KeyHasherType::GetHashCode(Pair.First);

		typename ElementListType::NodeType* FoundNode = FindByHash(HashCode);

		if (FoundNode == nullptr) // Key not found
		{
			++ElementCount;
			return Buckets[HashCode % BucketCount].InsertAtTail(new typename ElementListType::NodeType(HashCode, Move(Pair.First), Move(Pair.Second)));
		}

		return nullptr;
	}

	template<typename... ArgsType>
	typename ElementListType::NodeType* EmplaceByHash(HashCodeType HashCode, ArgsType&&... Args)
	{
		if ((static_cast<TFloat>(ElementCount + 1) / static_cast<TFloat>(BucketCount)) > MaxLoadFactor)
		{
			PrivateRehash();
		}

		typename ElementListType::NodeType* FoundNode = FindByHash(HashCode);

		if (FoundNode == nullptr) // Key not found
		{
			++ElementCount;
			return Buckets[HashCode % BucketCount].InsertAtTail(new typename ElementListType::NodeType(HashCode, Forward<ArgsType>(Args)...));
		}

		return nullptr;
	}

public:
	TRNT_FORCE_INLINE typename ElementListType::NodeType* Insert(const KeyType& Key, const ValueType& Value)
	{
		return EmplaceByHash(KeyHasherType::GetHashCode(Key), Key, Value);
	}

	TRNT_FORCE_INLINE typename ElementListType::NodeType* Insert(KeyType&& Key, const ValueType& Value)
	{
		return EmplaceByHash(KeyHasherType::GetHashCode(Key), Move(Key), Value);
	}

	TRNT_FORCE_INLINE typename ElementListType::NodeType* Insert(const KeyType& Key, ValueType&& Value)
	{
		return EmplaceByHash(KeyHasherType::GetHashCode(Key), Key, Move(Value));
	}

	TRNT_FORCE_INLINE typename ElementListType::NodeType* Insert(KeyType&& Key, ValueType&& Value)
	{
		return EmplaceByHash(KeyHasherType::GetHashCode(Key), Move(Key), Move(Value));
	}

	TRNT_FORCE_INLINE typename ElementListType::NodeType* InsertByHash(HashCodeType HashCode, const KeyType& Key, const ValueType& Value)
	{
		return EmplaceByHash(HashCode, Key, Value);
	}

	TRNT_FORCE_INLINE typename ElementListType::NodeType* InsertByHash(HashCodeType HashCode, KeyType&& Key, const ValueType& Value)
	{
		return EmplaceByHash(HashCode, Move(Key), Value);
	}

	TRNT_FORCE_INLINE typename ElementListType::NodeType* InsertByHash(HashCodeType HashCode, const KeyType& Key, ValueType&& Value)
	{
		return EmplaceByHash(HashCode, Key, Move(Value));
	}

	TRNT_FORCE_INLINE typename ElementListType::NodeType* InsertByHash(HashCodeType HashCode, KeyType&& Key, ValueType&& Value)
	{
		return EmplaceByHash(HashCode, Move(Key), Move(Value));
	}

	TRNT_FORCE_INLINE typename ElementListType::NodeType* Insert(const PairType& Pair)
	{
		return EmplaceByHash(KeyHasherType::GetHashCode(Pair.First), Pair.First, Pair.Second);
	}

	TRNT_FORCE_INLINE typename ElementListType::NodeType* Insert(PairType&& Pair)
	{
		return EmplaceByHash(KeyHasherType::GetHashCode(Pair.First), Move(Pair.First), Move(Pair.Second));
	}

	TRNT_FORCE_INLINE typename ElementListType::NodeType* InsertOrAssign(const KeyType& Key, ValueType&& Value)
	{
		return InsertOrAssignImpl<const KeyType&>(Key, Move(Value));
	}

	TRNT_FORCE_INLINE typename ElementListType::NodeType* InsertOrAssign(KeyType&& Key, ValueType&& Value)
	{
		return InsertOrAssignImpl<KeyType&&>(Move(Key), Move(Value));
	}

	TRNT_FORCE_INLINE void Insert(std::initializer_list<PairType> Pairs, TBool AssignIfExist = false)
	{
		const PairType* Begin = Pairs.begin();
		const PairType* End = Pairs.end();

		while (Begin != End)
		{
			HashCodeType HashCode = KeyHasherType::GetHashCode(Begin->First);
			typename ElementListType::NodeType* FoundNode = FindByHash(HashCode);

			if (FoundNode == nullptr)
			{
				++ElementCount;
				Buckets[HashCode % BucketCount].InsertAtTail(new typename ElementListType::NodeType(HashCode, Begin->First, Begin->Second));
			}
			else
			{
				if (AssignIfExist)
				{
					FoundNode->Value.Value = Begin->Second;
				}
			}

			++Begin;
		}

		if ((static_cast<TFloat>(ElementCount) / static_cast<TFloat>(BucketCount)) > MaxLoadFactor)
		{
			PrivateRehash();
		}
	}

public:
	TBool Remove(const KeyType& Key)
	{
		HashCodeType HashCode = KeyHasherType::GetHashCode(Key);

		typename ElementListType::NodeType* FoundNode = FindByHash(HashCode);

		if (FoundNode == nullptr)
		{
			return false;
		}

		Buckets[HashCode % BucketCount].RemoveAt(FoundNode);
		--ElementCount;

		return true;
	}

	TBool RemoveByHash(const HashCodeType HashCode)
	{
		typename ElementListType::NodeType* FoundNode = FindByHash(HashCode);

		if (FoundNode == nullptr)
		{
			return false;
		}

		Buckets[HashCode % BucketCount].RemoveAt(FoundNode);
		--ElementCount;

		return true;
	}

public:
	SizeType GetKeys(TDynamicArray<KeyType>& Output) const
	{
		Output.Reserve(ElementCount);

		for (SizeType BucketIndex = 0; BucketIndex < BucketCount; ++BucketIndex)
		{
			typename ElementListType::NodeType* Node = Buckets[BucketIndex].GetHead();

			while (Node != nullptr)
			{
				Output.EmplaceBack(Node->Value.Key);
				Node = Node->Next;
			}
		}

		return Output.GetElementCount();
	}

	SizeType GetValues(TDynamicArray<ValueType>& Output) const
	{
		Output.Reserve(ElementCount);

		for (SizeType BucketIndex = 0; BucketIndex < BucketCount; ++BucketIndex)
		{
			typename ElementListType::NodeType* Node = Buckets[BucketIndex].GetHead();

			while (Node != nullptr)
			{
				Output.EmplaceBack(Node->Value.Value);
				Node = Node->Next;
			}
		}

		return Output.GetElementCount();
	}

	SizeType GetElements(TDynamicArray<ElementType>& Output)
	{
		Output.Reserve(ElementCount);

		for (SizeType BucketIndex = 0; BucketIndex < BucketCount; ++BucketIndex)
		{
			typename ElementListType::NodeType* Node = Buckets[BucketIndex].GetHead();

			while (Node != nullptr)
			{
				Output.EmplaceBack(Node->Value.HashCode, Node->Value.Key, Node->Value.Value);
				Node = Node->Next;
			}
		}

		return Output.GetElementCount();
	}

public:
	template<typename PredicateType>
	void FilterKeys(PredicateType Predicate, TDynamicArray<KeyType>& Output) const
	{
		for (SizeType BucketIndex = 0; BucketIndex < BucketCount; ++BucketIndex)
		{
			typename ElementListType::NodeType* Node = Buckets[BucketIndex].GetHead();

			while (Node != nullptr)
			{
				if (Predicate(Node->Value.Key))
				{
					Output.EmplaceBack(Node->Value.Key);
				}
				Node = Node->Next;
			}
		}
	}

	template<typename PredicateType>
	void FilterValues(PredicateType Predicate, TDynamicArray<ValueType>& Output) const
	{
		for (SizeType BucketIndex = 0; BucketIndex < BucketCount; ++BucketIndex)
		{
			typename ElementListType::NodeType* Node = Buckets[BucketIndex].GetHead();

			while (Node != nullptr)
			{
				if (Predicate(Node->Value.Value))
				{
					Output.EmplaceBack(Node->Value.Value);
				}
				Node = Node->Next;
			}
		}
	}

	template<typename PredicateType>
	void FilterElements(PredicateType Predicate, TDynamicArray<ElementType>& Output)
	{
		for (SizeType BucketIndex = 0; BucketIndex < BucketCount; ++BucketIndex)
		{
			typename ElementListType::NodeType* Node = Buckets[BucketIndex].GetHead();

			while (Node != nullptr)
			{
				if (Predicate(Node->Value))
				{
					Output.EmplaceBack(Node->Value.HashCode, Node->Value.Key, Node->Value.Value);
				}
				Node = Node->Next;
			}
		}
	}

public:
	static TRNT_FORCE_INLINE HashCodeType GetHashCode(const KeyType& Key)
	{
		return KeyHasherType::GetHashCode(Key);
	}

private:
	SizeType ElementCount;
	SizeType BucketCount;

	ElementListType* Buckets;
};

#pragma warning(pop)