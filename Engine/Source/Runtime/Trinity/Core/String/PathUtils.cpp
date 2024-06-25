#include "TrinityPCH.h"

#include "PathUtils.h"

TString TPathUtils::GetFileName(const TChar* Path)
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

TString TPathUtils::GetFileNameWithoutExtension(const TChar* Path)
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

TString TPathUtils::GetFileExtension(const TChar* Path)
{
	if (!Path || !*Path)
	{
		return "";
	}

	const TChar* Found = TCString<TChar>::Strrchr(Path, '.');

	return (Found) ? TString(Found) : "";
}


TString TPathUtils::RemoveFileExtension(const TChar* Path)
{
	if (!Path || !*Path)
	{
		return "";
	}

	const TChar* Found = TCString<TChar>::Strrchr(Path, '.');

	return (Found) ? TString(static_cast<TInt64>(Found - Path), Path) : Path;
}

void TPathUtils::NormalizePath(TString& Path)
{
	if (Path.IsEmpty())
	{
		return;
	}

	TChar* PathData = Path.GetData();

	TChar* Found = ::strchr(PathData, '\\');

	while (Found)
	{
		*Found = '/';
		Found = strchr(Found, '\\');
	}

	TDynamicArray<TString> StringArr;
	TInt64 PathLen = Path.GetElementCount();

	TString Tmp;
	for (TInt64 Index = 0; Index < PathLen; ++Index)
	{
		Tmp = "";

		while (Index < PathLen && Path[Index] != '/')
		{
			Tmp += Path[Index];
			++Index;
		}

		const TChar* TmpData = Tmp.GetData();

		if (strcmp(TmpData, "..") == 0)
		{
			if (!StringArr.IsEmpty())
			{
				StringArr.Pop();
			}
		}
		else if (strcmp(TmpData, ".") == 0 || !*TmpData)
		{

		}
		else
		{
			StringArr.EmplaceBack(Tmp);
		}
	}

	Path = "";
	TInt64 ElementCount = StringArr.GetElementCount();
	const TString* StringArrData = StringArr.GetData();
	for (TInt64 Index = 0; Index < ElementCount; ++Index)
	{
		Path += StringArrData[Index];
		Path += "/";
	}

	if (Path.IsEmpty())
	{
		Path = "/";
	}
}

TString TPathUtils::ParentPathOf(const TChar* Path)
{
	if (!Path || !*Path)
	{
		return "";
	}

	const TSize_T PathLen = strlen(Path);

	const TChar* FoundSlash = TCString<TChar>::Strrpbrk(Path, Path + PathLen, "/\\");

	return (FoundSlash) ? TString(static_cast<TInt64>(FoundSlash - Path) + 1, Path) : "";
}

void TPathUtils::ResolvePath(const TChar* OriginalPath, TString* Extension, TString* FileName, TString* FileNameWithoutExtension, TString* ParentPath)
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