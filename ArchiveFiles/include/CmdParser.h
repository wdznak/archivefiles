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
		CmdParser(int argc, char** argv) {
			auto result = entryParser_.parse({ argc, argv });
			if (!result) {
				std::cerr << entryParser_ << "\n";
				exit(1);
			}

			if (showHelp_) std::cout << entryParser_ << "\n";

			if (fileName_ != "") {
				std::cout << "Archive name is set to: " << fileName_ << "\n";
			}

			if (regex_ != "") {
				findFiles(regex_);
				regex_.clear();
				archiver.list();
			}

			mainParser_ |= entryParser_;
		}

		bool isDone() const {
			return close_;
		}

		// I know it looks gorgeous
		void parse(const std::string& input) {
			auto result = mainParser_.parse({input, input});
			if (!result) {
				std::cout << mainParser_ << "\n";
				return;
			}

			// Todo: if compressing is in a process stop it first
			if (close_) return;
			
			if (removeAll_) {
				archiver.removeAll();
				removeAll_ = false;
			}
			else {
				if (removeIds_ != "") {
					removeByIds();
					removeIds_.clear();
				}
				if (removeName_ != "") {
					archiver.removeFile(removeName_);
					removeName_.clear();
				}
			}

			if (regex_ != "") {
				findFiles(regex_);
				regex_.clear();
				archiver.list();
			}
			else if (list_) {
				archiver.list();
				list_ = false;
			}

			if (compress_) {
				if (fileName_ != "") {
					archiver.compress(fileName_);
				}
				else {
					std::cout << "You have to specify the name of a file. Type -h or --help form more information.\n";
				}
				compress_ = false;
			}
			
		}

	private:

		void findFiles(const std::string& regex) {
			std::regex exp(regex, std::regex_constants::ECMAScript | std::regex_constants::icase);

			std::string str;
			for (auto& p : fs::directory_iterator("./")) {
					std::cout << p.path().string();
				if (std::regex_search(p.path().string(), exp)) {
					auto s = p.path();
					s.make_preferred();
					archiver.addFile(s.string());
				}
			}
		}

		void removeByIds() {
			std::stringstream ss(removeIds_);
			std::string token;
			size_t id;
			while (std::getline(ss, token, ',')) {
				try {
					id = stoul(token);
					archiver.removeFile(id);
				}
				catch (...) {};
			}
		}
	};

} // namespace ArchiveFiles
