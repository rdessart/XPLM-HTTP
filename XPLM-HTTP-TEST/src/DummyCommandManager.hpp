#pragma once

#include <Commands/ICommandExecutor.hpp>
#include <iostream>

class DummyCommandManager : public ICommandExecutor
{
	public:
	DummyCommandManager() = default;
	void execute(const std::string& command, CommandMode mode) override
	{
		std::cout  << "Executing command: " << command <<  "with mode : " << static_cast<int>(mode) << "\n";
	}
};

