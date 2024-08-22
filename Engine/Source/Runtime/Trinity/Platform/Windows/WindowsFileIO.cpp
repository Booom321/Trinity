#include "TrinityPCH.h"
#include "Trinity/Platform/FileIO.h"

#if defined (TRNT_PLATFORM_WIN64)

#pragma warning(push)
#pragma warning(disable: 6387)

TFileReader::TFileReader()
	: FileHandle(nullptr)
{}

TFileReader::TFileReader(const TChar* FilePath, TFileOpenMode OpenMode)
	: FileHandle(CreateFileA(FilePath, GENERIC_READ, FILE_SHARE_READ, NULL, static_cast<DWORD>(OpenMode), FILE_ATTRIBUTE_NORMAL, NULL))
{
	TRNT_ASSERT(FileHandle != INVALID_HANDLE_VALUE);
}

void TFileReader::Close()
{
	if (FileHandle)
	{
		CloseHandle(FileHandle);
		FileHandle = nullptr;
	}
}

TBool TFileReader::Open(const TChar* FilePath, TFileOpenMode OpenMode)
{
	TRNT_ASSERT(FileHandle == nullptr);
	FileHandle = CreateFileA(FilePath, GENERIC_READ, FILE_SHARE_READ, NULL, static_cast<DWORD>(OpenMode), FILE_ATTRIBUTE_NORMAL, NULL);
	return FileHandle != INVALID_HANDLE_VALUE;
}

TByte* TFileReader::ReadFile()
{
	const TInt64 FileSize = GetFileSize();
	TByte* Buffer = new TByte[static_cast<TUInt32>(FileSize)];
	OVERLAPPED Ol = { 0 };
	TBool Result = ReadFileEx(FileHandle, Buffer, static_cast<TUInt32>(FileSize), &Ol, nullptr) != 0;
	if (!Result)
	{
		delete[] Buffer;
	}
	return Result ? Buffer : nullptr;
}

TBool TFileReader::ReadFile(TByte* Buffer, TInt64 TBytesToRead)
{
	if (TBytesToRead < 0)
	{
		TBytesToRead = GetFileSize();
	}
	OVERLAPPED Ol = { 0 };

	return ReadFileEx(FileHandle, Buffer, static_cast<TUInt32>(TBytesToRead), &Ol, nullptr) != 0;
}

TString TFileReader::ReadTextFile()
{
	const TInt64 FileSize = GetFileSize();
	TString Text(static_cast<TUInt32>(FileSize), '\0');
	OVERLAPPED Ol = { 0 };
	TBool Result = ReadFileEx(FileHandle, Text.GetData(), static_cast<TUInt32>(FileSize), &Ol, nullptr) != 0;
	return Result ? Text : TString();
}

TBool TFileReader::IsOpen() const
{
	return FileHandle != nullptr;
}

TUInt32 TFileReader::Tellg() const
{
	return SetFilePointer(FileHandle, 0, nullptr, FILE_CURRENT);
}

void TFileReader::Seekg(TUInt32 Seek, TSeekDir SeekDir)
{
	SetFilePointer(FileHandle, Seek, nullptr, static_cast<DWORD>(SeekDir));
}

TInt64 TFileReader::GetFileSize() const
{
	LARGE_INTEGER FileSize;
	if (!GetFileSizeEx(FileHandle, &FileSize))
	{
		return -1;
	}
	return FileSize.QuadPart;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TFileWriter::TFileWriter()
	: FileHandle(nullptr)
{}

TFileWriter::TFileWriter(const TChar* FilePath, TFileOpenMode OpenMode, TBool EnableToAppend)
	: FileHandle(CreateFileA(FilePath, GENERIC_WRITE, 0, NULL, static_cast<DWORD>(OpenMode), FILE_ATTRIBUTE_NORMAL, NULL))
{
	TRNT_ASSERT(FileHandle != INVALID_HANDLE_VALUE);

	EnableToAppend ? SetFilePointer(FileHandle, 0, nullptr, FILE_END) : SetFilePointer(FileHandle, 0, nullptr, FILE_BEGIN);
}

void TFileWriter::Close()
{
	if (FileHandle)
	{
		CloseHandle(FileHandle);
		FileHandle = nullptr;
	}
}

TBool TFileWriter::Open(const TChar* FilePath, TFileOpenMode OpenMode, TBool EnableToAppend)
{
	TRNT_ASSERT(FileHandle == nullptr);
	FileHandle = CreateFileA(FilePath, GENERIC_WRITE, 0, nullptr, static_cast<DWORD>(OpenMode), FILE_ATTRIBUTE_NORMAL, nullptr);
	if (FileHandle == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	EnableToAppend ? SetFilePointer(FileHandle, 0, nullptr, FILE_END) : SetFilePointer(FileHandle, 0, nullptr, FILE_BEGIN);
	return true;
}

TBool TFileWriter::WriteFile(TByte* Buffer, TUInt32 TBytesToWrite)
{
	DWORD Written;
	return ::WriteFile(FileHandle, Buffer, TBytesToWrite, &Written, nullptr);
}

TBool TFileWriter::WriteTextFile(const TString& Text)
{
	DWORD Written;
	return ::WriteFile(FileHandle, Text.GetData(), static_cast<TUInt32>(Text.GetElementCount()), &Written, nullptr);
}

TBool TFileWriter::IsOpen() const
{
	return FileHandle != nullptr;
}

TUInt32 TFileWriter::Tellg() const
{
	return SetFilePointer(FileHandle, 0, nullptr, FILE_CURRENT);
}

void TFileWriter::Seekg(TUInt32 Seek, TSeekDir SeekDir)
{
	SetFilePointer(FileHandle, Seek, nullptr, static_cast<DWORD>(SeekDir));
}

TInt64 TFileWriter::GetFileSize() const
{
	LARGE_INTEGER FileSize;
	if (!GetFileSizeEx(FileHandle, &FileSize))
	{
		return -1;
	}
	return FileSize.QuadPart;
}

#pragma warning(pop)

#endif