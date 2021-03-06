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

#include "PCH.h"
#include "Core/Helpers/Strings.h"

#include <algorithm>
#include <cctype>

#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#undef __STDC_FORMAT_MACROS

namespace MicroBuild {
namespace Strings {

bool CaseInsensitiveEquals(const std::string& a, const std::string& b)
{
	if (a.size() != b.size())
	{
		return false;
	}
	for (unsigned int i = 0; i < a.size(); i++)
	{
		if (::tolower(a[i]) != ::tolower(b[i]))
		{
			return false;
		}
	}
	return true;
}

std::string ToLowercase(const std::string& input)
{
	std::string result = input;
	for (size_t i = 0; i < input.size(); i++)
	{
		result[i] = (char)::tolower(result[i]);
	}
	return result;
}

std::string Join(int argc, char* argv[], std::string glue)
{
	size_t glueSize = glue.length();
	size_t totalSize = 0;

	for (int i = 0; i < argc; i++)
	{
		totalSize += strlen(argv[i]);
		totalSize += glueSize;
	}

	std::string result;
	result.reserve(totalSize);

	for (int i = 0; i < argc; i++)
	{
		if (i != 0)
		{
			result += glue;
		}
		result += argv[i];
	}

	return result;
}

std::string Join(const std::vector<std::string>& needles, std::string glue)
{
	std::string result = "";
	for (const std::string& res : needles)
	{
		if (!result.empty())
		{
			result += glue;
		}
		result += res;
	}
	return result;
}

std::string StripQuotes(const std::string& input, bool bThroughout)
{
	if (bThroughout)
	{
		bool bInPair = false;
		bool bQuotesRequired = false;
		char lastChr = '\0';
		size_t startPosition = 0;
			
		std::string result = input;

		for (size_t i = 0; i < result.size(); i++)
		{		
			char chr = result[i];
			if (chr == '"' && lastChr != '\\')
			{
				if (bInPair)
				{
					if (!bQuotesRequired)
					{
						result.erase(startPosition, 1);
						i--;
						result.erase(i, 1);
						i--;
					}
				}
				else
				{
					bInPair = true;
					bQuotesRequired = false;
					startPosition = i;
				}
			}
			else if (chr == ' ')	
			{
				bQuotesRequired = true;
			}
			lastChr = chr;
		}

		return result;
	}
	else
	{
		if (input.size() >= 2 && input[0] == '\"' && input[input.size() - 1] == '\"')
		{
			return input.substr(1, input.size() - 2);
		}
		else
		{
			return input;
		}
	}
}

std::string StripSingleQuotes(const std::string& input, bool bThroughout)
{
	if (bThroughout)
	{
		bool bInPair = false;
		bool bQuotesRequired = false;
		char lastChr = '\0';
		size_t startPosition = 0;

		std::string result = input;

		for (size_t i = 0; i < result.size(); i++)
		{
			char chr = result[i];
			if (chr == '\'' && lastChr != '\\')
			{
				if (bInPair)
				{
					if (!bQuotesRequired)
					{
						result.erase(startPosition, 1);
						i--;
						result.erase(i, 1);
						i--;
					}
				}
				else
				{
					bInPair = true;
					bQuotesRequired = false;
					startPosition = i;
				}
			}
			else if (chr == ' ')
			{
				bQuotesRequired = true;
			}
			lastChr = chr;
		}

		return result;
	}
	else
	{
		if (input.size() >= 2 && input[0] == '\'' && input[input.size() - 1] == '\'')
		{
			return input.substr(1, input.size() - 2);
		}
		else
		{
			return input;
		}
	}
}

std::vector<std::string> Crack(std::string value, char seperator, bool bKeepQuotes)
{
	std::vector<std::string> result;
	std::string segment = "";
	bool inQuotes = false;

	for (size_t i = 0; i < value.length(); i++)
	{
		char chr = value[i];

		// End of value.
		if (chr == seperator && !inQuotes)
		{
			if (segment.length() > 0)
			{
				result.push_back(segment);
			}
			segment.clear();
			continue;
		}
		
		// Quote value start.
		else if (chr == '"')
		{
			inQuotes = !inQuotes;
			if (!bKeepQuotes)
			{
				continue;
			}
		}
		
		// Quote escape.
		else if (chr == '\\' && inQuotes)
		{
			if (i + 1 < value.length())
			{
				if (value[i + 1] == '"')
				{
					i++;
					continue;
				}
			}
		}

		segment += chr;
	}

	if (!segment.empty())
	{
		result.push_back(segment);
	}

	return result;
}

void SplitOnIndex(const std::string& input, size_t index,
	std::string& leftOut, std::string& rightOut)
{
	// These temporaries are intentional as its common to do things like this with this function:
	// Consumable.Split(10, Consumable, Consumed)

	std::string leftValue = input.substr(0, index);
	std::string rightValue = input.substr(index + 1);

	leftOut = leftValue;
	rightOut = rightValue;
}

bool ReadFile(const Platform::Path& path, std::string& output)
{
	FILE* file = fopen(path.ToString().c_str(), "r");
	if (file)
	{
		int result = fseek(file, 0, SEEK_END);
		if (result != 0)
		{
			fclose(file);
			return false;
		}

		long length = ftell(file);

		result = fseek(file, 0, SEEK_SET);
		if (result != 0)
		{
			fclose(file);
			return false;
		}

		std::vector<char> data;
		data.resize(length);

		size_t ret = fread(data.data(), 1, length, file);
		data.resize(ret);

		fclose(file);

		output = std::string(data.data(), data.size());

		return true;
	}

	return false;
}

bool WriteFile(const Platform::Path& path, const std::string& data)
{
	FILE* file = fopen(path.ToString().c_str(), "w");
	if (file)
	{
		size_t ret = fwrite(data.c_str(), 1, data.size(), file);

		fclose(file);

		return (ret == data.size());
	}

	return false;
}

std::string Format(std::string format, ...)
{

#if defined(MB_PLATFORM_WINDOWS)
#pragma warning( push )
// non-portable use of class 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>' as an argument to a variadic function
// Not sure this is valid, from what I can see this should be valid and portable? Need to investigate, currently works on all platforms though (undefined behaviour fun!)
#pragma warning( disable : 4840 ) 
#endif

	va_list args;
	va_start(args, format);
	std::string result = FormatVa(format, args);
	va_end(args);

#if defined(MB_PLATFORM_WINDOWS)
#pragma warning( pop )
#endif

	return result;
}

std::string FormatVa(std::string format, va_list args)
{
	const int STACK_BUFFER_SIZE = 512;

	char stackBuffer[STACK_BUFFER_SIZE];
	char* buffer = stackBuffer;

	va_list baseArgs;
	va_copy(baseArgs, args);

	int bytesRequired = vsnprintf(buffer, STACK_BUFFER_SIZE, format.c_str(), baseArgs);
	if (bytesRequired >= STACK_BUFFER_SIZE - 1)
	{
		va_copy(baseArgs, args);
		buffer = new char[bytesRequired + 1];
		vsnprintf(buffer, bytesRequired + 1, format.c_str(), baseArgs);
	}

	std::string result = buffer;

	if (buffer != stackBuffer)
	{
		delete[] buffer;
	}

	va_end(baseArgs);

	return result;
}

unsigned int Hash(const std::string& value, unsigned int start)
{
	unsigned int hash = start;
	const char* Value = value.c_str();

	for (; *Value; ++Value)
	{
		hash += *Value;
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}

	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);

	return hash;
}

uint64_t Hash64(const std::string& value, uint64_t start)
{
	uint64_t hash = start;
	const char* Value = value.c_str();

	for (; *Value; ++Value)
	{
		hash += *Value;
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}

	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);

	return hash;
}

std::string Trim(const std::string& input)
{
	if (input.size() == 0)
	{
		return "";
	}

	size_t startIndex = 0;
	while (startIndex < input.size())
	{
		if (std::isspace(input[startIndex]))//input[startIndex] == ' ' || input[startIndex] == '\t')
		{
			startIndex++;
		}
		else
		{
			break;
		}
	}

	size_t endIndex = input.size() - 1;
	while (endIndex >= startIndex)
	{
		if (std::isspace(input[endIndex]))// input[endIndex] == ' ' || input[endIndex] == '\t')
		{
			endIndex--;
		}
		else
		{
			break;
		}
	}

	return input.substr(startIndex, (endIndex - startIndex) + 1);
}

std::vector<std::string> Split(char seperator, const std::string& value, bool bAllowEscaping, bool bIgnoreEmpty)
{
	std::vector<std::string> result;

	size_t offset = 0;
	size_t shiftOffset = 0;
	while (offset < value.size())
	{
		size_t endOffset = value.find(seperator, offset + shiftOffset);
		if (endOffset != std::string::npos)
		{
			if (!bAllowEscaping || (endOffset < 1 || value[endOffset - 1] != '\\'))
			{
				std::string frag = Trim(value.substr(offset, endOffset - offset));
				if (frag.size() > 0 || !bIgnoreEmpty)
				{
					result.push_back(frag);
				}

				offset = endOffset + 1;
				shiftOffset = 0;
			}
			else
			{
				shiftOffset++;
			}
		}
		else
		{
			std::string frag = Trim(value.substr(offset));
			if (frag.size() > 0 || !bIgnoreEmpty)
			{
				result.push_back(frag);
			}
			shiftOffset = 0;
			break;
		}
	}

	return result;
}

std::string Guid(const std::vector<std::string>& values)
{
	uint64_t hash = 0;

	for (const std::string& res : values)
	{
		hash = Hash64(res, hash);
	}

	char buffer[64];
	sprintf(buffer, "%012" PRIu64, hash);

	std::string result = buffer;
	if (result.size() > 12)
	{
		result.resize(12);
	}

	return Strings::Format("{00000000-0000-0000-0000-%s}", result.c_str());
}

std::string Uuid(int length, const std::vector<std::string>& values)
{
	uint64_t hash = 0;

	for (const std::string& res : values)
	{
		hash = Hash64(res, hash);
	}

	char tag[64];
	sprintf(tag, "%%0%i" PRIX64, length);

	char buffer[64];
	sprintf(buffer, tag, hash);

	std::string result = buffer;
	if ((int)result.size() > length)
	{
		result.resize(length);
	}
    while ((int)result.size() < length)
    {
        result = "0" + result;
    }

	return result;
}

std::string Escaped(const std::string& input, bool bEscapeSequences)
{
	std::string result;
	result.reserve(input.size());
	for (char chr : input)
	{
		if (chr == '"' || (chr == '\\' && bEscapeSequences))
		{
			result.push_back('\\');
		}
		result.push_back(chr);
	}
	return result;
}

std::string EscapeSlashes(const std::string& input)
{
	std::string result;
	result.reserve(input.size());
	for (char chr : input)
	{
		if (chr == '\\')
		{
			result.push_back('\\');
		}
		result.push_back(chr);
	}
	return result;
}

std::string Quoted(const std::string& input, bool bEscapeSequences)
{
	return Strings::Format("\"%s\"", Escaped(input, bEscapeSequences).c_str());
}

std::string SpacesEscaped(const std::string& input)
{
	std::string result;
	result.reserve(input.size());
	for (char chr : input)
	{
		if (chr == ' ')
		{
			result.push_back('\\');
		}
		result.push_back(chr);
	}
	return result;
}

// Matches a string similar to fnmatch, based on code from;
// http://xoomer.virgilio.it/acantato/dev/wildcard/wildmatch.html
bool IsMatch(const std::string& input_in, const std::string& pattern_in)
{
	const char* value = input_in.c_str();
	const char* pattern = pattern_in.c_str();

	const char* s = NULL;
	const char* p = NULL;
	bool star = false;

loopStart:
	for (s = value, p = pattern; *s; ++s, ++p)
	{
		switch (*p)
		{
		case '?':
		{
			if (*s == '.')
				goto starCheck;

			break;
		}
		case '*':
		{
			star = true;
			value = s;
			pattern = p;

			do
			{
				++pattern;
			} while (*pattern == '*');

			if (!*pattern)
				return true;

			goto loopStart;
		}
		default:
		{
			if (::toupper(*s) != ::toupper(*p))
				goto starCheck;

			break;
		}
		}
	}

	while (*p == '*')
		++p;

	return (!*p);

starCheck:
	if (!star)
		return false;

	value++;
	goto loopStart;
}

std::string Replace(const std::string& context, const std::string& from, const std::string& to)
{
	std::string value = context;
	size_t from_size = from.size();
	while (true)
	{
		size_t offset = value.find(from);
		if (offset != std::string::npos)
		{
			value = value.replace(offset, from_size, to.c_str(), to.size());
		}
		else
		{
			break;
		}
	}
	return value;
}

bool IsNumeric(const std::string& input)
{
	for (size_t i = 0; i < input.size(); i++)
	{
		if (!::isdigit(input[i]))
		{
			return false;
		}
	}
	return true;
}

}; // namespace Strings
}; // namespace MicroBuild
