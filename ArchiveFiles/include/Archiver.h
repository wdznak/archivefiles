#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <7zpp/7zpp.h>

namespace ArchiveFiles {

class ArchiverPC : public SevenZip::ProgressCallback
{
public:
	void OnStartWithTotal(const SevenZip::TString& archivePath, unsigned __int64 totalBytes) override {
		std::cout << "Cos sie dzieje\n"
			<< archivePath << "\n"
			<< totalBytes << "\n";
	}

	void OnProgress(const SevenZip::TString& archivePath, unsigned __int64 bytesCompleted) override {
		std::cout << "Robimy Progress!\n"
			<< archivePath << "\n"
			<< bytesCompleted << "\n";
	}

	void OnDone(const SevenZip::TString& archivePath) override {
		std::cout << "On Done!\n";
	};

	void OnFileDone(const SevenZip::TString& archivePath, const SevenZip::TString& filePath, unsigned __int64 bytesCompleted) override {
		std::cout << "On File Done!\n";
	};

	bool OnCheckBreak() override {
		std::cout << "On Check Break!\n";
		return false;
	};
};

class Archiver
{
private:
	SevenZip::SevenZipLibrary lib_;
	ArchiverPC callback_;
	std::map<size_t, std::string> pathList_;
	size_t idCounter_ = 0;
	
public:
	Archiver();
	void addFile(const std::string& path);
	void addFiles(const std::vector<std::string>& pathList);
	bool removeFile(const std::string& path);
	bool removeFile(size_t id);
	void removeAll();

	void compress(const std::string& fileName);
	void list() const;
};

} // namespace ArchiveFiles