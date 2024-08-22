template<typename T>
TInt64 TPathUtils<T>::FindLastSlashInPath(const CharType* Path, TSize_T PathLen)
{
	if (PathLen == (TSize_T)-1)
	{
		PathLen = CString::Strlen(Path);
	}

	const CharType* First = Path;
	const CharType* Last = Path + PathLen;

	for (; First <= Last; --Last)
	{
		if (*Last == PathComponents::BackSlash[0] || *Last == PathComponents::ForwardSlash[0])
		{
			return static_cast<TInt64>(Last - First);
		}
	}

	return -1;
}

template<typename T>
TStringBase<typename TPathUtils<T>::CharType> TPathUtils<T>::GetFileName(const CharType* Path)
{
	if (!Path || !*Path)
	{
		return PathComponents::Empty;
	}
	
	const TSize_T PathLen = CString::Strlen(Path);
	const TInt64 SlashIndex = FindLastSlashInPath(Path, PathLen) + 1;

	return TStringBase<CharType>(PathLen - SlashIndex, Path + SlashIndex);
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

	const TSize_T PathLen = CString::Strlen(Path);

	const TInt64 SlashIndex = FindLastSlashInPath(Path, PathLen) + 1;
	const CharType* DotPos = CString::Strrchr(Path + SlashIndex, PathComponents::Dot[0]);

	if (DotPos)
	{
		const TInt64 DotIndex = static_cast<TInt64>(DotPos - Path);
		return TStringBase<CharType>(DotIndex - SlashIndex, Path + SlashIndex);
	}

	return TStringBase<CharType>(PathLen - SlashIndex, Path + SlashIndex);
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

	const TSize_T PathLen = CString::Strlen(Path);
	const TInt64 SlashPos = FindLastSlashInPath(Path, PathLen);

	return (SlashPos == -1) ? PathComponents::Empty : TStringBase<CharType>(SlashPos + 1, Path);
}

template<typename T>
void TPathUtils<T>::ResolvePath(
	const CharType* OriginalPath,
	TStringBase<CharType>* Extension,
	TStringBase<CharType>* FileName,
	TStringBase<CharType>* FileNameWithoutExtension,
	TStringBase<CharType>* ParentPath)
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
template<typename ... PathArgs>
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