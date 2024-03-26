#pragma once

#include "Trinity/Core/Defines.h"
#include "Trinity/Core/PlatformDetection.h"
#include "Trinity/Core/Types/DataTypes.h"

#include "Trinity/Core/String/String.h"

class TRNT_API TFileSystem
{
public:
	static TBool FileExists(const TChar* FilePath);

	static TBool DeleteFile(const TChar* FilePath);

	static TBool CopyFileOrDirectory(const TChar* From, const TChar* To);

	static TBool MoveFileOrDirectory(const TChar* From, const TChar* To);

	static TInt64 GetFileSize(const TChar* FilePath);

public:
	static TBool DirectoryExists(const TChar* DirectoryPath);

	static TBool CreateDirectory(const TChar* DirectoryPath);

	static TBool DeleteDirectory(const TChar* DirectoryPath);

	static TBool SetCurrentDirectory(const TChar* CurrentDir);

public:
	static TBool IsFile(const TChar* Path);

	static TBool IsDirectory(const TChar* Path);

public:
	static TString GetFileName(const TChar* Path);

	static TString GetFileNameWithoutExtension(const TChar* Path);

	static TString GetFileExtension(const TChar* Path);

	static TString RemoveFileExtension(const TChar* Path);

	static void NormalizePath(TChar* Path);

	static TString GetExecutablePath();

	static void ResolvePath(const TChar* OriginalPath, TString* Extension, TString* FileName, TString* FileNameWithoutExtension, TString* ParentPath);

	static TString ParentPathOf(const TChar* Path);

	static TString GetCurrentDirectory();

	static TBool IsAbsolute(const TChar* Path);

	static TBool IsRelative(const TChar* Path);
};