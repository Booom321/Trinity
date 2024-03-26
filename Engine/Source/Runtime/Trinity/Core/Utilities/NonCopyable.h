#pragma once

class TNonCopyable
{
protected:
	TNonCopyable() = default;
	~TNonCopyable() = default;

	TNonCopyable(const TNonCopyable&) = delete;
	TNonCopyable& operator=(const TNonCopyable&) = delete;
};
