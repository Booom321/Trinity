#pragma once

template<bool Result, typename TypeIfTrue, typename TypeIfFalse>
class TTypeChooser;

template<typename TypeIfTrue, typename TypeIfFalse>
class TTypeChooser<true, TypeIfTrue, TypeIfFalse>
{
public:
	using Type = TypeIfTrue;
};

template<typename TypeIfTrue, typename TypeIfFalse>
class TTypeChooser<false, TypeIfTrue, TypeIfFalse>
{
public:
	using Type = TypeIfFalse;
};