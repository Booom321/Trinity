#pragma once

#include "String.h"
#include "Trinity/Core/TypeTraits/Conjunction.h"
#include "Trinity/Core/TypeTraits/Decay.h"
#include "Trinity/Core/TypeTraits/Logical.h"
#include "Trinity/Core/TypeTraits/RemoveCVRef.h"

template<typename T>
class TRNT_API TPathUtils
{
public:
	static_assert(TIsCharTypeSupported<T>::Value, "TPathUtils<T> is not implemented for this char type.");

	using CharType = typename TRemoveCVRef<T>::Type;

	template<typename U>
	class TIsString
	{
	public:
		static constexpr TBool Value = TOr<
			TAreTheSameType<typename TRemoveCVRef<U>::Type, TStringBase<CharType>>,
			TAreTheSameType<const CharType*, typename TDecay<U>::Type>,
			TAreTheSameType<CharType*, typename TDecay<U>::Type>>::Value;
	};

	static constexpr TBool IsCharType = TAreTheSameType<CharType, TChar>::Value;
	static constexpr TBool IsWCharType = TAreTheSameType<CharType, TWChar>::Value;

	template<typename U>
	class TPathComponents;

	template<>
	class TPathComponents<TChar>
	{
	public:
		static TRNT_CONSTEXPR const TChar* Empty = "";
		static TRNT_CONSTEXPR const TChar* Dot = ".";
		static TRNT_CONSTEXPR const TChar* ForwardSlash = "/";
		static TRNT_CONSTEXPR const TChar* BackSlash = "\\";
		static TRNT_CONSTEXPR const TChar* Separators = "/\\";
		static TRNT_CONSTEXPR const TChar* TwoDots = "..";
	};

	template<>
	class TPathComponents<TWChar>
	{
	public:
		static TRNT_CONSTEXPR const TWChar* Empty = L"";
		static TRNT_CONSTEXPR const TWChar* Dot = L".";
		static TRNT_CONSTEXPR const TWChar* ForwardSlash = L"/";
		static TRNT_CONSTEXPR const TWChar* BackSlash = L"\\";
		static TRNT_CONSTEXPR const TWChar* Separators = L"/\\";
		static TRNT_CONSTEXPR const TWChar* TwoDots = L"..";
	};

	using CString = TCString<CharType>;
	using PathComponents = TPathComponents<CharType>;

public:
	static TStringBase<CharType> GetFileName(const CharType* Path);

	static TStringBase<CharType> GetFileExtension(const CharType* Path);

	static TStringBase<CharType> GetFileNameWithoutExtension(const CharType* Path);

	static TStringBase<CharType> RemoveFileExtension(const CharType* Path);

	static TStringBase<CharType> ChangeFileExtension(const CharType* Path, const CharType* NewExtension);

	static TStringBase<CharType> ParentPathOf(const CharType* Path);

	static void NormalizePath(TStringBase<CharType>& Path);

	static TBool IsSeparator(CharType Chr);

	template<typename... PathArgs>
	static TStringBase<CharType> CombinePaths(PathArgs&&... Paths);

public:
	static TRNT_INLINE constexpr const TChar* GetRootDirectory()
	{
		if constexpr (IsCharType)
		{
			return TRNT_ROOT_DIRECTORY;
		}
		else if constexpr (IsWCharType)
		{
			return TRNT_ROOT_DIRECTORY_W;
		}
	}

private:
	static const CharType* FindLastSlashInPath(const CharType* Path, const CharType* PathEnd);
};

template<typename T>
const typename TPathUtils<T>::CharType* TPathUtils<T>::FindLastSlashInPath(const CharType* Path, const CharType* PathEnd)
{
	for (; Path <= PathEnd; --PathEnd)
	{
		if (*PathEnd == PathComponents::BackSlash[0] || *PathEnd == PathComponents::ForwardSlash[0])
		{
			return PathEnd;
		}
	}

	return nullptr;
}

template<typename T>
TStringBase<typename TPathUtils<T>::CharType> TPathUtils<T>::GetFileName(const CharType* Path)
{
	if (!Path || !*Path)
	{
		return PathComponents::Empty;
	}

	const CharType* PathEnd = Path + CString::Strlen(Path);
	const CharType* Pos = FindLastSlashInPath(Path, PathEnd);

	if (Pos)
	{
		++Pos;
		return TStringBase<CharType>(PathEnd - Pos, Pos);
	}

	return TStringBase<CharType>(PathEnd - Path, Path);
}

template<typename T>
TStringBase<typename TPathUtils<T>::CharType> TPathUtils<T>::GetFileExtension(const CharType* Path)
{
	if (!Path || !*Path)
	{
		return PathComponents::Empty;
	}

	const CharType* DotPos = CString::Strrchr(Path, PathComponents::Dot[0]);

	return (DotPos) ? TStringBase<CharType>(DotPos) : PathComponents::Empty;
}

template<typename T>
TStringBase<typename TPathUtils<T>::CharType> TPathUtils<T>::GetFileNameWithoutExtension(const CharType* Path)
{
	if (!Path || !*Path)
	{
		return PathComponents::Empty;
	}

	const CharType* PathEnd = Path + CString::Strlen(Path);
	const CharType* Pos = FindLastSlashInPath(Path, PathEnd);

	if (Pos)
	{
		const CharType* DotPos = CString::Strrchr(Pos, PathComponents::Dot[0]);

		if (DotPos)
		{
			return TStringBase<CharType>(DotPos - Pos++, Pos);
		}
	}

	return TStringBase<CharType>(PathEnd - Path, Path);
}

template<typename T>
TStringBase<typename TPathUtils<T>::CharType> TPathUtils<T>::RemoveFileExtension(const CharType* Path)
{
	if (!Path || !*Path)
	{
		return PathComponents::Empty;
	}

	const CharType* DotPos = CString::Strrchr(Path, PathComponents::Dot[0]);

	return (DotPos) ? TStringBase<CharType>(static_cast<TInt64>(DotPos - Path), Path) : Path;
}

template<typename T>
void TPathUtils<T>::NormalizePath(TStringBase<CharType>& Path)
{
	if (Path.IsEmpty())
	{
		return;
	}

	CharType* PathData = Path.GetData();

	CharType* Found = CString::Strchr(PathData, PathComponents::BackSlash[0]);

	while (Found)
	{
		*Found = PathComponents::ForwardSlash[0];
		Found = CString::Strchr(Found, PathComponents::BackSlash[0]);
	}

	TDynamicArray<TStringBase<CharType>> StringArr;
	TInt64 PathLen = Path.GetElementCount();

	TStringBase<CharType> Tmp;
	for (TInt64 Index = 0; Index < PathLen; ++Index)
	{
		Tmp = PathComponents::Empty;

		while (Index < PathLen && Path[Index] != PathComponents::ForwardSlash[0])
		{
			Tmp += Path[Index];
			++Index;
		}

		const CharType* TmpData = Tmp.GetData();

		if (CString::Strcmp(TmpData, PathComponents::TwoDots) == 0)
		{
			if (!StringArr.IsEmpty())
			{
				StringArr.Pop();
			}
		}
		else if (CString::Strcmp(TmpData, PathComponents::Dot) == 0 || !*TmpData)
		{
		}
		else
		{
			StringArr.EmplaceBack(Tmp);
		}
	}

	Path = PathComponents::Empty;
	TInt64 ElementCount = StringArr.GetElementCount();
	const TStringBase<CharType>* StringArrData = StringArr.GetData();

	for (TInt64 Index = 0; Index < ElementCount; ++Index)
	{
		Path += StringArrData[Index];
		Path += PathComponents::ForwardSlash;
	}

	if (Path.IsEmpty())
	{
		Path = PathComponents::ForwardSlash;
	}
}

template<typename T>
TStringBase<typename TPathUtils<T>::CharType> TPathUtils<T>::ParentPathOf(const CharType* Path)
{
	if (!Path || !*Path)
	{
		return PathComponents::Empty;
	}

	const CharType* PathEnd = Path + CString::Strlen(Path);
	const CharType* Pos = FindLastSlashInPath(Path, PathEnd);

	return (Pos) ? TStringBase<CharType>(++Pos - Path, Path) : PathComponents::Empty;
}

template<typename T>
TStringBase<typename TPathUtils<T>::CharType> TPathUtils<T>::ChangeFileExtension(const CharType* Path, const CharType* NewExtension)
{
	if (!Path || !*Path)
	{
		return PathComponents::Empty;
	}

	if (!NewExtension || !*NewExtension)
	{
		return Path;
	}

	const CharType* DotPos = CString::Strrchr(Path, PathComponents::Dot[0]);
	const TBool NewExtensionHasDot = NewExtension[0] == PathComponents::Dot[0];

	if (DotPos)
	{
		TStringBase<CharType> Result(DotPos - Path + 1, Path);
		Result.Append(NewExtension + ((NewExtensionHasDot) ? 1 : 0));

		return Result;
	}

	const TSize_T PathLen = CString::Strlen(Path);
	TStringBase<CharType> Result(static_cast<TInt64>(PathLen), Path);

	if (NewExtensionHasDot)
	{
		Result += NewExtension;
	}
	else
	{
		Result += '.';
		Result += NewExtension;
	}

	return Result;
}

template<typename T>
TBool TPathUtils<T>::IsSeparator(CharType Chr)
{
	return Chr == PathComponents::ForwardSlash[0] || Chr == PathComponents::BackSlash[0];
}

template<typename T>
template<typename... PathArgs>
TStringBase<typename TPathUtils<T>::CharType> TPathUtils<T>::CombinePaths(PathArgs&&... Paths)
{
	static_assert(TConjunction<TIsString<PathArgs>...>::Value);

	const TStringBase<CharType> PathArray[] = { Forward<PathArgs>(Paths)... };
	const TInt32 PathCount = TRNT_GET_ARRAY_LENGTH(PathArray);

	if (PathCount < 1)
	{
		return PathComponents::Empty;
	}

	TInt64 NewPathCapacity = 0;
	for (TInt32 Index = 0; Index < PathCount; ++Index)
	{
		NewPathCapacity += PathArray[Index].GetElementCount();
	}
	NewPathCapacity += PathCount;

	TStringBase<CharType> OutputPath;
	OutputPath.Reserve(NewPathCapacity);
	OutputPath += PathArray[0];

	for (TInt32 Index = 1; Index < PathCount; ++Index)
	{
		OutputPath /= PathArray[Index];
	}

	return OutputPath;
}