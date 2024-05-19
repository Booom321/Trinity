#include "Trinity/Core/PlatformDetection.h"

#if defined(TRNT_PLATFORM_WIN64)

#include <Windows.h>
#include <shlwapi.h>

#include "Trinity/Platform/FileSystem.h"

#include "WindowsDeclarations.h"

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

	return (FileAttributes & FILE_ATTRIBUTE_DIRECTORY);
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

TBool TFileSystem::GetFilesInDirectory(const TString& Directory, TDynamicArray<TString>& Output, TBool Files, TBool Directories)
{
	if (Directory.GetElementCount() == 0)
	{
		return false;
	}

	TChar LastChar = Directory.Last();
	TString TempDirectory = (LastChar == 47 || LastChar == 92) ? Directory : (Directory + "/");

	WIN32_FIND_DATAA FindData;
	TWindowsHandle HFind = FindFirstFileA(TString(TempDirectory + "*").GetData(), &FindData);

	if (HFind == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	FindNextFileA(HFind, &FindData); // skip  "."
	FindNextFileA(HFind, &FindData); // skip  ".."

	do
	{
		if (Files)
		{
			if (!(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				Output.EmplaceBack(TempDirectory + FindData.cFileName);
			}
		}

		if (Directories)
		{
			if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				Output.EmplaceBack(TempDirectory + FindData.cFileName);
			}
		}
	} 
	while (FindNextFileA(HFind, &FindData));

	FindClose(HFind);

	return true;
}

#endif // PLATFORM_WINDOWS