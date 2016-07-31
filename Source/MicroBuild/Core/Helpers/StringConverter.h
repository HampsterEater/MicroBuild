/*
MicroBuild
Copyright (C) 2016 TwinDrills

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "Core/Helpers/Strings.h"
#include "Core/Platform/Path.h"
#include "Core/Log.h"

namespace MicroBuild {

template <typename FromType, typename ToType>
struct StringConverter
{
	bool Cast(const FromType& value, ToType& type)
	{
		extern bool StringConvert(const FromType& value, ToType& type);
		return StringConvert(value, type);
	}
};

// String->String

template <>
struct StringConverter<std::string, std::string>
{
	bool Cast(const std::string& value, std::string& type)
	{
		type = value;
		return true;
	}
};

template <>
struct StringConverter<const char*, std::string>
{
	bool Cast(const char* value, std::string& type)
	{
		type = value;
		return true;
	}
};


// String->Float

template <>
struct StringConverter<std::string, float>
{
	bool Cast(const std::string& value, float& type)
	{
		type = (float)atof(value.c_str());
		return true;
	}
};

template <>
struct StringConverter<float, std::string>
{
	bool Cast(const float& value, std::string& type)
	{
		char buffer[128];
		sprintf(buffer, "%f", value);
		type = buffer;
		return true;
	}
};

// String->Int

template <>
struct StringConverter<std::string, int>
{
	bool Cast(const std::string& value, int& type)
	{
		type = (int)atoi(value.c_str());
		return true;
	}
};

template <>
struct StringConverter<int, std::string>
{
	bool Cast(const int& value, std::string& type)
	{
		char buffer[128];
		sprintf(buffer, "%i", value);
		type = buffer;
		return true;
	}
};

// String->Bool

template <>
struct StringConverter<std::string, bool>
{
	bool Cast(const std::string& value, bool& type)
	{
		type = 
			!(Strings::CaseInsensitiveEquals(value, "false") ||
			  Strings::CaseInsensitiveEquals(value, "0"));
		return true;
	}
};


template <>
struct StringConverter<bool, std::string>
{
	bool Cast(const bool& value, std::string& type)
	{
		type = value ? "true" : "false";
		return true;
	}
};

// String->Path

template <>
struct StringConverter<std::string, Platform::Path>
{
	bool Cast(const std::string& value, Platform::Path& type)
	{
		type = value;
		return true;
	}
};


template <>
struct StringConverter<Platform::Path, std::string>
{
	bool Cast(const Platform::Path& value, std::string& type)
	{
		type = value.ToString();
		return true;
	}
};

// Allows the casting from one string to another data type.
template <typename FromType, typename ToType>
bool StringCast(const FromType& value, ToType& output)
{
	StringConverter<FromType, ToType> Converter;
	return Converter.Cast(value, output);
}

// Same as above but dosen't report errors, just returns the value if successful
// or an empty string otherwise.
template <typename FromType>
std::string CastToString(const FromType& value)
{
	std::string result;
	StringCast<FromType, std::string>(value, result);
	return result;
}

// Same as above but dosen't report errors, just returns the value if successful
// or an empty string otherwise.
template <typename ToType>
ToType CastFromString(const std::string& value)
{
	ToType result;
	StringCast<std::string, ToType>(value, result);
	return result;
}

}; // namespace MicroBuild