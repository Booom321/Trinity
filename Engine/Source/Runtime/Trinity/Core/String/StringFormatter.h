#pragma once

#include "String.h"

#include "Trinity/Core/TypeTraits/IsCharTypeSupported.h"
#include "Trinity/Core/TypeTraits/RemoveCVRef.h"

template<typename T>
class TStringFormatter
{
public:
	static_assert(TIsCharTypeSupported<T>::Value, "TStringFormatter<T> is not implemented for this char type.");

	using CharType = typename TRemoveCVRef<T>::Type;

public:
	TRNT_FORCE_INLINE TStringFormatter() : FmtString()
	{}

	TRNT_FORCE_INLINE TStringFormatter(const TStringBase<CharType>& FmtString) : FmtString(FmtString)
	{}

	TRNT_FORCE_INLINE TStringFormatter(TStringBase<CharType>&& FmtString) noexcept : FmtString(Move(FmtString))
	{}

	TRNT_FORCE_INLINE TStringFormatter(const CharType* FmtString) : FmtString(FmtString)
	{}

	TRNT_FORCE_INLINE TStringFormatter(const TStringFormatter& FmtString) : FmtString(FmtString.FmtString)
	{}

	TRNT_FORCE_INLINE TStringFormatter(TStringFormatter&& FmtString) noexcept : FmtString(Move(FmtString.FmtString))
	{}

	~TStringFormatter() = default;

public:
	TRNT_FORCE_INLINE TStringFormatter& operator=(const TStringFormatter& FmtString)
	{
		if (this != &FmtString)
		{
			this->FmtString = FmtString.FmtString;
		}

		return *this;
	}

	TRNT_FORCE_INLINE TStringFormatter& operator=(TStringFormatter&& FmtString) noexcept
	{
		if (this != &FmtString)
		{
			this->FmtString = Move(FmtString.FmtString);
		}

		return *this;
	}
	
public:

private:
	TStringBase<CharType> FmtString;
};