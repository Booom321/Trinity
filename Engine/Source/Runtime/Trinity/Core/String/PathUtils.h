#pragma once

#include "String.h"

#include "Trinity/Core/TypeTraits/Conjunction.h"
#include "Trinity/Core/TypeTraits/Logical.h"
#include "Trinity/Core/TypeTraits/RemoveCVRef.h"
#include "Trinity/Core/TypeTraits/Decay.h"

class TRNT_API TPathUtils
{
	template<typename T>
	class TIsAnsiStringExceptStdString
	{
	public:
		static constexpr TBool Value = TOr<
			TAreTheSameType<typename TRemoveCVRef<T>::Type, TString>, TAreTheSameType<const TChar*, typename TDecay<T>::Type>, TAreTheSameType<TChar*, typename TDecay<T>::Type>
		>::Value;
	};

public:
	static TString GetFileName(const TChar* Path);

	static TString GetFileNameWithoutExtension(const TChar* Path);

	static TString GetFileExtension(const TChar* Path);

	static TString RemoveFileExtension(const TChar* Path);

	static void NormalizePath(TString& Path);

	static void ResolvePath(const TChar* OriginalPath, TString* Extension, TString* FileName, TString* FileNameWithoutExtension, TString* ParentPath);

	static TString ParentPathOf(const TChar* Path);

	template<typename ... PathArgs>
	static TString CombinePaths(PathArgs&&... Paths)
	{
		static_assert(TConjunction<TIsAnsiStringExceptStdString<PathArgs>...>::Value);

		const TString PathArray[] = { Forward<PathArgs>(Paths)... };
		const TInt32 PathCount = TRNT_GET_ARRAY_LENGTH(PathArray);

		if (PathCount <= 0)
		{
			return "";
		}
		
		TInt64 NewPathCapacity = 0;
		for (TInt32 Index = 0; Index < PathCount; ++Index)
		{
			NewPathCapacity += PathArray[Index].GetElementCount();
		}
		NewPathCapacity += PathCount;
		
		TString OutputPath;
		OutputPath.Reserve(NewPathCapacity);
		OutputPath += PathArray[0];

		for (TInt32 Index = 1; Index < PathCount; ++Index)
		{
			OutputPath /= PathArray[Index];
		}

		return OutputPath;
	}

public:
	static TRNT_INLINE constexpr const TChar* GetRootDirectory() { return TRNT_ROOT_DIRECTORY; }
};