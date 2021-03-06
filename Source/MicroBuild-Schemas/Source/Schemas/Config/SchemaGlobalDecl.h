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

#ifndef SCHEMA_FILE
#error No schema file defined
#endif
#ifndef SCHEMA_CLASS
#error No schema class defined
#endif

// ----------------------------------------------------------------------------

#define START_OPTION(ValueType, Group, Key, Description)
#define START_ARRAY_OPTION(ValueType, Group, Key, Description)
#define START_KEY_VALUE_ARRAY_OPTION(Group, Description)
#define OPTION_RULE_REQUIRED()
#define OPTION_RULE_ORDER_IMPORTANT()
#define OPTION_RULE_DEFAULT(Value)
#define OPTION_RULE_VALIDATOR(ValidatorFunction)
#define OPTION_RULE_EXPAND_PATH_WILDCARDS(bCanCache)
#define OPTION_RULE_ABSOLUTE_PATH()
#define OPTION_RULE_OPTION(Option)
#define OPTION_RULE_NO_INHERIT()
#define END_OPTION()
#define END_ARRAY_OPTION()
#define END_KEY_VALUE_ARRAY_OPTION()
#define START_ENUM(Name) \
		enum class Name : unsigned int {
#define ENUM_KEY(Name) \
			Name,
#define END_ENUM() \
			COUNT \
		};

#include SCHEMA_FILE

#undef START_OPTION
#undef START_ARRAY_OPTION
#undef START_KEY_VALUE_ARRAY_OPTION
#undef OPTION_RULE_REQUIRED
#undef OPTION_RULE_ORDER_IMPORTANT
#undef OPTION_RULE_DEFAULT
#undef OPTION_RULE_VALIDATOR
#undef OPTION_RULE_EXPAND_PATH_WILDCARDS
#undef OPTION_RULE_ABSOLUTE_PATH
#undef OPTION_RULE_OPTION
#undef OPTION_RULE_NO_INHERIT
#undef END_OPTION
#undef END_ARRAY_OPTION
#undef END_KEY_VALUE_ARRAY_OPTION
#undef START_ENUM
#undef ENUM_KEY
#undef END_ENUM

// ----------------------------------------------------------------------------
