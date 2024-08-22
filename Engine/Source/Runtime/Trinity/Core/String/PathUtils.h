#pragma once

#include "String.h"

#include "Trinity/Core/TypeTraits/Conjunction.h"
#include "Trinity/Core/TypeTraits/Logical.h"
#include "Trinity/Core/TypeTraits/RemoveCVRef.h"
#include "Trinity/Core/TypeTraits/Decay.h"

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
			TAreTheSameType<CharType*, typename TDecay<U>::Type>
		>::Value;
	};

	static constexpr TBool IsCharType = TAreTheSameType<CharType, TChar>::Value;
	static constexpr TBool IsWCharType = TAreTheSameType<CharType, TWChar>::Value;

	template<typename U>
	class TPathComponents;

	template<>
	class TPathComponents<TChar>
	{
	public:
		static TRNT_CONSTEXPR const TChar* Empty		= "";
		static TRNT_CONSTEXPR const TChar* Dot			= ".";
		static TRNT_CONSTEXPR const TChar* ForwardSlash = "/";
		static TRNT_CONSTEXPR const TChar* BackSlash	= "\\";
		static TRNT_CONSTEXPR const TChar* Separators	= "/\\";
		static TRNT_CONSTEXPR const TChar* TwoDots		= "..";
	};

	template<>
	class TPathComponents<TWChar>
	{
	public:
		static TRNT_CONSTEXPR const TWChar* Empty			= L"";
		static TRNT_CONSTEXPR const TWChar* Dot				= L".";
		static TRNT_CONSTEXPR const TWChar* ForwardSlash	= L"/";
		static TRNT_CONSTEXPR const TWChar* BackSlash		= L"\\";
		static TRNT_CONSTEXPR const TWChar* Separators		= L"/\\";
		static TRNT_CONSTEXPR const TWChar* TwoDots			= L"..";
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
	
	static void ResolvePath(
		const CharType* OriginalPath, 
		TStringBase<CharType>* Extension, 
		TStringBase<CharType>* FileName,
		TStringBase<CharType>* FileNameWithoutExtension, 
		TStringBase<CharType>* ParentPath);
	
	static TBool IsSeparator(CharType Chr);

	template<typename ... PathArgs>
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
	static TInt64 FindLastSlashInPath(const CharType* Path, TSize_T PathLen = (TSize_T)-1);
};

#include "PathUtils_inl.h"