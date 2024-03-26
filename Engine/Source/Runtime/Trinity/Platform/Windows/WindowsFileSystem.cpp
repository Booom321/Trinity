#include "Trinity/Platform/FileSystem.h"

#if defined(TRNT_PLATFORM_WIN64)

#include "WindowsDeclarations.h"

#include <Windows.h>
#include <shlwapi.h>

#undef DeleteFile
#undef CreateDirectory

TBool TFileSystem::FileExists(const TChar* FilePath)
{
	DWORD FileAttributes = GetFileAttributesA(FilePath);
	return (FileAttributes != INVALID_FILE_ATTRIBUTES && !(FileAttributes & FILE_ATTRIBUTE_DIRECTORY));
}

TBool TFileSystem::DeleteFile(const TChar* FilePath)
{
	return DeleteFileA(FilePath);
}

TInt64 TFileSystem::GetFileSize(const TChar* FilePath)
{
	TWindowsHandle FileHandle = CreateFileA(FilePath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (FileHandle == INVALID_HANDLE_VALUE)
	{
		return -1;
	}

	LARGE_INTEGER FileSize;
	if (!GetFileSizeEx(FileHandle, &FileSize))
	{
		CloseHandle(FileHandle);
		return -1;
	}

	CloseHandle(FileHandle);
	return FileSize.QuadPart;
}

TBool TFileSystem::DirectoryExists(const TChar* DirectoryPath)
{
	DWORD FileAttributes = GetFileAttributesA(DirectoryPath);
	return (FileAttributes != INVALID_FILE_ATTRIBUTES && (FileAttributes & FILE_ATTRIBUTE_DIRECTORY));
}

TBool TFileSystem::CreateDirectory(const TChar* DirectoryPath)
{
	return CreateDirectoryA(DirectoryPath, nullptr);
}

TBool TFileSystem::DeleteDirectory(const TChar* DirectoryPath)
{
	return RemoveDirectoryA(DirectoryPath);
}

TBool TFileSystem::IsFile(const TChar* Path)
{
	DWORD FileAttributes = GetFileAttributesA(Path);

	return !(FileAttributes & FILE_ATTRIBUTE_DIRECTORY);
}

TBool TFileSystem::IsDirectory(const TChar* Path)
{
	DWORD FileAttributes = GetFileAttributesA(Path);

	return ((FileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) ? true : false;
}

TBool TFileSystem::CopyFileOrDirectory(const TChar* From, const TChar* To)
{
	return CopyFileA(From, To, false);
}

TBool TFileSystem::MoveFileOrDirectory(const TChar* From, const TChar* To)
{
	return MoveFileA(From, To);
}

TString TFileSystem::GetExecutablePath()
{
	TChar Result[TRNT_WINDOWS_MAX_PATH];
	GetModuleFileNameA(nullptr, Result, TRNT_WINDOWS_MAX_PATH);
	return Result;
}

#undef GetCurrentDirectory
#undef SetCurrentDirectory

TString TFileSystem::GetCurrentDirectory()
{
	TChar Result[TRNT_WINDOWS_MAX_PATH];
	GetCurrentDirectoryA(TRNT_WINDOWS_MAX_PATH, Result);
	return Result;
}

TBool TFileSystem::SetCurrentDirectory(const TChar* CurrentDir)
{
	return SetCurrentDirectoryA(CurrentDir);
}

TBool TFileSystem::IsAbsolute(const TChar* Path)
{
	return !PathIsRelativeA(Path);
}

TBool TFileSystem::IsRelative(const TChar* Path)
{
	return PathIsRelativeA(Path);
}

#endif // PLATFORM_WINDOWS