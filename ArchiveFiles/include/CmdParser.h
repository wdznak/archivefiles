#pragma once

#include <filesystem>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>

#include <lyra/lyra.hpp>

#include "Archiver.h"


namespace fs = std::filesystem;

namespace ArchiveFiles {

	class CmdParser
	{
	private:
		Archiver archiver;
		bool close_, compress_, list_, removeAll_, showHelp_;
		std::string fileName_, regex_, removeIds_, removeName_;

		lyra::cli_parser entryParser_ = lyra::help(showHelp_)
			| lyra::opt(fileName_, "Name")
			["-n"]["--name"]
			("Name of 7z archive.")
			| lyra::opt(regex_, "Regex")
			["-r"]["--regex"]
			("Specify a regex pattern.");

		lyra::cli_parser mainParser_ = lyra::opt(removeIds_, "Remove by ids")
			["-ri"]["--remove-by-ids"]
			("Remove entry/entries by id. Use comma as separator. E.g. <3,14,1>.")
			| lyra::opt(removeAll_, "Remove all")
			["-ra"]["--remove-all"]
			("Remove all entries.")
			| lyra::opt(removeName_, "Remove by name")
			["-rn"]["--remove-by-name"]
			("Remove entry by name")
			| lyra::opt(list_)
			["-l"]["--list"]
			("Print the list of staged files.")
			| lyra::opt(close_)
			["-c"]["--close"]
			("Close app.")
			| lyra::opt(compress_)
			["-a"]["--archive"]
			("Start compression.");

	public:
		CmdParser(int argc, char** argv);
		bool isDone() const;
		void parse(const std::string& input);

	private:

		void findFiles(const std::string& regex);
		void removeByIds();
	};

} // namespace ArchiveFiles
