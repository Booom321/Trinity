#pragma once

class TNonMovable
{
protected:
	TNonMovable() = default;
	~TNonMovable() = default;

	TNonMovable(TNonMovable&&) = delete;
	TNonMovable& operator=(TNonMovable&&) = delete;
};