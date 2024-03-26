#include "FileSystem.h"

TString TFileSystem::GetFileName(const TChar* Path)
{
	if (!Path || !*Path)
	{
		return "";
	}

	const TSize_T PathLen = strlen(Path);
	const TChar* Found = TCString<TChar>::Strrpbrk(Path, Path + PathLen, "/\\");
	++Found;
	return (Found) ? TString(static_cast<TInt64>(PathLen - (Found - Path)), Found) : "";
}

TString TFileSystem::GetFileNameWithoutExtension(const TChar* Path)
{
	if (!Path || !*Path)
	{
		return "";
	}

	const TSize_T PathLen = strlen(Path);

	const TChar* FoundSlash = TCString<TChar>::Strrpbrk(Path, Path + PathLen, "/\\");
	const TChar* FoundExtension = TCString<TChar>::Strrchr(FoundSlash, '.');

	if (!FoundSlash)
	{
		return (FoundExtension) ? TString(static_cast<TInt64>(FoundExtension - Path), Path) : "";
	}

	if (!FoundExtension)
	{
		return (FoundSlash++) ? TString(static_cast<TInt64>(PathLen - (FoundSlash - Path)), FoundSlash) : "";
	}

	return TString(static_cast<TInt64>(FoundExtension - FoundSlash), ++FoundSlash);
}

TString TFileSystem::GetFileExtension(const TChar* Path)
{
	if (!Path || !*Path)
	{
		return "";
	}

	const TChar* Found = TCString<TChar>::Strrchr(Path, '.');

	return (Found) ? TString(Found) : "";
}


TString TFileSystem::RemoveFileExtension(const TChar* Path)
{
	if (!Path || !*Path)
	{
		return "";
	}

	const TChar* Found = TCString<TChar>::Strrchr(Path, '.');

	return (Found) ? TString(static_cast<TInt64>(Found - Path), Path) : Path;
}

void TFileSystem::NormalizePath(TChar* Path)
{
	if (!Path || !*Path)
	{
		return;
	}

#if defined(TRNT_PLATFORM_WIN64)
	TChar* Found = strchr(Path, '\\');

	while (Found)
	{
		*Found = '/';
		Found = strchr(Found, '\\');
	}
#endif
}

TString TFileSystem::ParentPathOf(const TChar* Path)
{
	if (!Path || !*Path)
	{
		return "";
	}

	const TSize_T PathLen = strlen(Path);

	const TChar* FoundSlash = TCString<TChar>::Strrpbrk(Path, Path + PathLen, "/\\");

	return (FoundSlash) ? TString(static_cast<TInt64>(FoundSlash - Path) + 1, Path) : "";
}

void TFileSystem::ResolvePath(const TChar* OriginalPath, TString* Extension, TString* FileName, TString* FileNameWithoutExtension, TString* ParentPath)
{
	if (!OriginalPath || !*OriginalPath)
	{
		return;
	}

	if (Extension)
	{
		*Extension = GetFileExtension(OriginalPath);
	}

	if (FileName)
	{
		*FileName = GetFileName(OriginalPath);
	}

	if (FileNameWithoutExtension)
	{
		*FileNameWithoutExtension = GetFileNameWithoutExtension(OriginalPath);
	}

	if (ParentPath)
	{
		*ParentPath = ParentPathOf(OriginalPath);
	}
}
