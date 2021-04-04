#pragma once

#ifndef SILVERBINARYFILE_H
#define SILVERBINARYFILE_H

#include <string>
#include <fstream>
#include <vector>

#define __debug false
#if __debug == true
#	include <iostream>
using std::cout;
#endif

#ifndef SILVERBIN_EXPORTS
#define SILVERBIN_API __declspec(dllexport)
#else
#define SILVERBIN_API __declspec(dllimport)
#endif
#pragma warning(push)
#pragma warning(disable:4251)
class SILVERBIN_API SilverBinaryFile
{
private:
	std::string filePath;
	std::ifstream fileIn;
	std::ofstream fileOut;
	std::vector<unsigned char> currentBytes;
	bool openIn();
	bool openOut();
public:
	size_t position = 0;
	void jump(size_t amt = 1);
	void locate(size_t amt);
	SilverBinaryFile(std::string filePath = "data.bin");
	void clearPosition();
	bool fetchBytes();
	template <typename T>
	T next(bool* status = nullptr);
	std::string nextString(unsigned int max = 0xFF);
	template <typename T>
	void insert(T item);
};
#pragma warning(pop)

template<typename T>
inline T SilverBinaryFile::next(bool* status)
{
	if (!this->fetchBytes()) {
		if (status != nullptr) {
			*status = false;
		}
		return (T)0;
	}
	T x;
	try {
		x = parseUtils::getItemFromBytes<T>(&currentBytes[this->position]);
		this->jump(sizeof(T));
		return x;
	}
	catch (std::exception) {
		if (status != nullptr) *status = false;
		return (T)0;
	}
	if (status != nullptr) *status = true;
	return x;
}


template<typename T>
inline void SilverBinaryFile::insert(T item)
{
	this->fetchBytes();
	this->fileIn.close();
	this->openOut();
	std::vector<byte> bytes = parseUtils::getBytes(item);
	bool written = false;
	for (size_t pos = 0; pos < this->currentBytes.size(); pos++) {
		if (pos == position) {
#if __debug == true
			cout << "7 ";
#endif
			written = true;
			for (size_t i = 0; i < bytes.size(); i++) {
				this->fileOut << bytes[i];
			}
			pos += bytes.size();
		}
		else {
#if __debug == true
			cout << "\n(pre) pos= " << pos << "\n";
			cout << "set: #(" << pos << ") to: $(" << currentBytes[pos] << ") (" << (int)currentBytes[pos] << ")\n";
#endif
			this->fileOut << currentBytes[pos];
		}
	}
	if (!written) {
		for (size_t = 0; i < bytes.size(); i++) {
			this->fileOut << bytes[i];
		}
	}
	position += sizeof(item);
	this->fileOut.close();
}
#endif