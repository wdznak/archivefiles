#include "Archiver.h"

namespace ArchiveFiles {

Archiver::Archiver() {
	if (!lib_.Load()) {
		std::cout << "Can't load 7z.dll\n";
		
	}
}

void Archiver::addFile(const std::string& path)
{
	pathList_.insert({ idCounter_++, path });
}

void Archiver::addFiles(const std::vector<std::string>& pathList)
{
	for (auto& e : pathList) {
		pathList_.insert({ idCounter_++, e });
	}
}

bool Archiver::removeFile(const std::string& path)
{
	for (auto it = pathList_.begin(); it != pathList_.end(); ++it) {
		if (it->second == path) {
			pathList_.erase(it);
			return true;
		}
	}
	return false;
}

bool Archiver::removeFile(size_t id)
{
	auto it = pathList_.find(id);
	if (it != pathList_.end()) {
		pathList_.erase(it);
		return true;
	}
	return false;
}

void Archiver::removeAll()
{
	pathList_.clear();
	idCounter_ = 0;
}

void Archiver::compress(const std::string& fileName)
{
	SevenZip::SevenZipCompressor comp(lib_, fileName);
	comp.UseAbsolutePaths(false);
	comp.SetCompressionFormat(SevenZip::CompressionFormat::Zip);

	for (auto& e : pathList_) {
		comp.AddFile(e.second);
	}

	comp.DoCompress(&callback_);
}

void Archiver::list() const
{
	std::cout << "List of files staged to compress:\n";
	for (auto& e : pathList_) {
		std::cout << e.first << ": " << e.second << "\n";
	}
}

} // namespace ArchiveFiles
	