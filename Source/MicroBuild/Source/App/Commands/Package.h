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

#include "Core/Commands/Command.h"
#include "Core/Platform/Path.h"

namespace MicroBuild {

class App;

// Packages s project up using the steps defined in the config file.
class PackageCommand : public Command
{
public:
	PackageCommand(App* app);

protected:
	virtual bool Invoke(CommandLineParser* parser) override;

	bool ExecuteCommand(const std::string& command, ProjectFile* projectFile);

private:
	WorkspaceFile m_workspaceFile;
	Platform::Path m_workspaceFilePath;

	Platform::Path m_packageDirectoryPath;

	std::string m_configuration;
	std::string m_platform;

	std::string m_targetPackager;

	std::string m_projectName;

	std::map<std::string, std::string> m_setArguments;

	bool m_rebuild;

	App* m_app;

};

}; // namespace MicroBuild
