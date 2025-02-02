#pragma once

#include "PlatformDeclarations.h"
#include "Trinity/Core/Defines.h"
#include "Trinity/Core/PlatformDetection.h"
#include "Trinity/Core/String/String.h"

enum class TFileOpenMode : TUInt32
{
	ECreateNew = 1,
	ECreateAlways = 2,
	EOpenExisting = 3,
	EOpenAlways = 4,
	ETruncateExisting = 5,
};

enum class TSeekDir : TUInt32
{
	EBegin = 0,
	ECurrent = 1,
	EEnd = 2,
};

class TRNT_API TFileReader
{
	TRNT_DISALLOW_COPY_AND_ASSIGN(TFileReader);

public:
	TFileReader();

	TFileReader(const TChar* FilePath, TFileOpenMode OpenMode = TFileOpenMode::EOpenExisting);

	TRNT_FORCE_INLINE ~TFileReader()
	{
		Close();
	}

public:
	void Close();

	TBool Open(const TChar* FilePath, TFileOpenMode OpenMode = TFileOpenMode::EOpenExisting);

	TByte* ReadFile();

	TBool ReadFile(TByte* Buffer, TInt64 TBytesToRead);

	TString ReadTextFile();

	TBool IsOpen() const;

	TUInt32 Tellg() const;

	void Seekg(TUInt32 Seek, TSeekDir SeekDir = TSeekDir::EBegin);

	TInt64 GetFileSize() const;

private:
	TFileHandle FileHandle;
};

class TRNT_API TFileWriter
{
	TRNT_DISALLOW_COPY_AND_ASSIGN(TFileWriter);

public:
	TFileWriter();

	TFileWriter(const TChar* FilePath, TFileOpenMode OpenMode = TFileOpenMode::ECreateNew, TBool EnableToAppend = true);

	TRNT_FORCE_INLINE ~TFileWriter()
	{
		Close();
	}

public:
	void Close();

	TBool Open(const TChar* FilePath, TFileOpenMode OpenMode = TFileOpenMode::ECreateNew, TBool EnableToAppend = true);

	TBool WriteFile(TByte* Buffer, TUInt32 TBytesToWrite);

	TBool WriteTextFile(const TString& Text);

	TBool IsOpen() const;

	TUInt32 Tellg() const;

	void Seekg(TUInt32 Seek, TSeekDir SeekDir = TSeekDir::EBegin);

	TInt64 GetFileSize() const;

private:
	TFileHandle FileHandle;
};