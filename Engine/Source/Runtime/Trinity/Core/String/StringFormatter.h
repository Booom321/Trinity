#pragma once

#include "String.h"
#include "StringConversion.h"

template<typename T>
class TStringFormatter
{
public:
	using SizeType = typename TStringBase<T>::SizeType;
	using ElementType = typename TStringBase<T>::ElementType;
	
	TRNT_FORCE_INLINE TStringFormatter(const ElementType* FmtCStr)
		: FmtString(FmtCStr)
	{}

	TRNT_FORCE_INLINE TStringFormatter(const ElementType* FmtCStr, SizeType FmtLen)
		: FmtString(FmtLen, FmtCStr)
	{}

	TRNT_FORCE_INLINE TStringFormatter(const TStringBase<ElementType>& FmtStr)
		: FmtString(FmtStr)
	{}

	TRNT_FORCE_INLINE TStringFormatter(TStringBase<ElementType>&& FmtStr) noexcept
		: FmtString(Move(FmtStr))
	{}

	TRNT_FORCE_INLINE TStringFormatter(const TStringFormatter& FmtStr)
		: FmtString(FmtStr.FmtString)
	{}

	TRNT_FORCE_INLINE TStringFormatter(TStringFormatter&& FmtStr) noexcept
		: FmtString(Move(FmtStr.FmtString))
	{}

	~TStringFormatter() = default;

public:
	TRNT_NODISCARD TRNT_FORCE_INLINE TStringFormatter& operator=(const ElementType* FmtCStr)
	{
		FmtString = FmtCStr;
		return *this;
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE TStringFormatter& operator=(const TStringBase<ElementType>& FmtStr)
	{
		FmtString = FmtStr;
		return *this;
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE TStringFormatter& operator=(TStringBase<ElementType>&& FmtStr) noexcept
	{
		FmtString = Move(FmtStr);
		return *this;
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE TStringFormatter& operator=(const TStringFormatter& FmtStr)
	{
		if (this != &FmtStr)
		{
			FmtString = FmtStr.FmtString;
		}
		return *this;
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE TStringFormatter& operator=(TStringFormatter&& FmtStr) noexcept
	{
		if (this != &FmtStr)
		{
			FmtString = Move(FmtStr.FmtString);
		}
		return *this;
	}

public:
	TRNT_NODISCARD TRNT_FORCE_INLINE SizeType GetElementCount() const { return FmtString.GetElementCount(); }
	TRNT_NODISCARD TRNT_FORCE_INLINE TBool IsEmpty() const { return FmtString.IsEmpty(); }

	TRNT_NODISCARD TRNT_FORCE_INLINE ElementType* GetData() { return FmtString.GetData(); }
	TRNT_NODISCARD TRNT_FORCE_INLINE const ElementType* GetData() const { return FmtString.GetData(); }

private:
	static TRNT_CONSTEXPR ElementType GetBracketChar()
	{
		if constexpr (TAreTheSameType<ElementType, TChar>::Value)
		{
			return '{';
		}
		else if constexpr (TAreTheSameType<ElementType, TChar>::Value)
		{
			return L'{';
		}
	}

	static TRNT_CONSTEXPR ElementType GetCloseBracketChar()
	{
		if constexpr (TAreTheSameType<ElementType, TChar>::Value)
		{
			return '}';
		}
		else if constexpr (TAreTheSameType<ElementType, TChar>::Value)
		{
			return L'}';
		}
	}

public:
	template<typename... FormatArgs>
	TRNT_NODISCARD TStringBase<ElementType> Format(FormatArgs&&... Args)
	{
		static constexpr TInt32 ArgCount = sizeof...(FormatArgs);
		static constexpr ElementType BracketChar = GetBracketChar();
		static constexpr ElementType CloseBracketChar = GetCloseBracketChar();

		TStringBase<ElementType> Strings[ArgCount] = { TNsStringConversion::ToString<ElementType>(Args)... };

		const ElementType* FmtCStr = FmtString.GetData();
		SizeType Index = FmtString.FindChar(BracketChar),
			     LastIndex = 0,
			     ArgIndex = 0;
		
		TStringBase<ElementType> Result{};
		while (Index != TStringBase<ElementType>::Npos)
		{
			if (ArgIndex == ArgCount)
			{
				TRNT_ASSERT_MESSAGE(false, "Too many placeholders(\"{}\") in format string");
			}
			TRNT_ASSERT_MESSAGE(FmtCStr[Index + 1] == '}', "Unmatched '}' in format string.");
			Result.Append(FmtCStr + LastIndex, Index - LastIndex);
			Result.Append(Strings[ArgIndex]);
			
			LastIndex = Index + 2;
			Index = FmtString.FindChar(BracketChar, LastIndex); // ignore '}' character
 			++ArgIndex;
		}

		Result.Append(FmtString, LastIndex, FmtString.GetElementCount() - LastIndex);

		return Result;
	}

private:
	TStringBase<ElementType> FmtString;
};