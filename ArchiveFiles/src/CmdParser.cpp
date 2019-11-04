#include "CmdParser.h"


ArchiveFiles::CmdParser::CmdParser(int argc, char** argv) {
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

bool ArchiveFiles::CmdParser::isDone() const {
	return close_;
}

void ArchiveFiles::CmdParser::parse(const std::string& input) {
	auto result = mainParser_.parse({ input, input });
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

void ArchiveFiles::CmdParser::findFiles(const std::string& regex) {
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

void ArchiveFiles::CmdParser::removeByIds() {
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
