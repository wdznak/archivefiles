#pragma once

#include <atomic>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <7zpp/7zpp.h>

namespace ArchiveFiles {

class ArchiverPC : public SevenZip::ProgressCallback
{
private:
	std::atomic<bool> cancelComp_ = false;

public:
	void OnStartWithTotal(const SevenZip::TString& archivePath, unsigned __int64 totalBytes) override {
		std::cout << "Start compressing: " << archivePath << " of total size: "	<< totalBytes << "\n";
	}

	void OnProgress(const SevenZip::TString& archivePath, unsigned __int64 bytesCompleted) override {
		std::cout <<  archivePath << " complited: " << bytesCompleted << "\n";
	}

	void OnDone(const SevenZip::TString& archivePath) override {
		std::cout << "Data has been compressed!\n";
	};

	void OnFileDone(const SevenZip::TString& archivePath, const SevenZip::TString& filePath, unsigned __int64 bytesCompleted) override {
		std::cout << "File has been compressed!\n";
	};

	bool OnCheckBreak() override {
		return cancelComp_;
	};

	void cancelCompressing() {
		cancelComp_ = true;
	}
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