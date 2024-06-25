#pragma once

#include "Trinity/Core/Defines.h"
#include "Trinity/Core/PlatformDetection.h"
#include "Trinity/Core/Types/DataTypes.h"

#include "Trinity/Core/String/String.h"

#ifdef TRNT_PLATFORM_WIN64
#	undef DeleteFile
#	undef CreateDirectory
#	undef SetCurrentDirectory
#	undef GetCurrentDirectory
#endif // TRNT_PLATFORM_WIN64

class TRNT_API TFileSystem
{
public:
	static TBool FileExists(const TChar* FilePath);

	static TBool DeleteFile(const TChar* FilePath);

	static TBool CopyFileOrDirectory(const TChar* From, const TChar* To);

	static TBool MoveFileOrDirectory(const TChar* From, const TChar* To);

	static TInt64 GetFileSize(const TChar* FilePath);
	
	static TBool GetFilesInDirectory(const TString& Directory, TDynamicArray<TString>& Output, TBool Files = true, TBool Directories = true);

	static TBool GetFilesInDirectoryRecursively(const TString& Directory, TDynamicArray<TString>& Output);

public:
	static TBool DirectoryExists(const TChar* DirectoryPath);

	static TBool CreateDirectory(const TChar* DirectoryPath);

	static TBool DeleteDirectory(const TChar* DirectoryPath);

	static TBool SetCurrentDirectory(const TChar* CurrentDir);

public:
	static TBool IsFile(const TChar* Path);

	static TBool IsDirectory(const TChar* Path);

public:
	static TString GetExecutablePath();

	static TString GetCurrentDirectory();

	static TBool IsAbsolute(const TChar* Path);

	static TBool IsRelative(const TChar* Path);
};