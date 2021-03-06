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

#include "App/Builder/Tasks/CompilePchTask.h"
#include "Core/Platform/Process.h"
#include "Core/Helpers/Time.h"

namespace MicroBuild {

CompilePchTask::CompilePchTask(Toolchain* toolchain, ProjectFile& project, BuilderFileInfo file)
	: BuildTask(BuildStage::PchCompile, true, true, false)
	, m_toolchain(toolchain)
	, m_file(file)
{
	MB_UNUSED_PARAMETER(project);
}

BuildAction CompilePchTask::GetAction()
{
	BuildAction action;
	action.StatusMessage = Strings::Format("Generating PCH: %s\n", m_file.SourcePath.GetFilename().c_str());

	m_toolchain->GetCompilePchAction(action, m_file);

	return action;
}

}; // namespace MicroBuild
