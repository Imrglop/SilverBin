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

using std::string;
using std::ofstream;
using std::ifstream;
using std::fstream;
using std::vector;

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
	string filePath;
	ifstream fileIn;
	ofstream fileOut;
	vector<unsigned char> currentBytes;
	bool openIn();
	bool openOut();
public:
	size_t position = 0;
	void jump(size_t amt = 1);
	void locate(size_t amt);
	SilverBinaryFile(string filePath = "data.bin");
	void clearPosition();
	bool fetchBytes();
	template <typename T>
	T next(bool* status = nullptr);
	string nextString(unsigned int max = 0xFF);
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
		return x;
	}
	catch (std::exception) {
		if (status != nullptr) *status = false;
		return (T)0;
	}
	if (status != nullptr) *status = true;
	this->position += sizeof(T);
	return x;
}
template<typename T>
inline void SilverBinaryFile::insert(T item)
{
	this->fetchBytes();
	this->fileIn.close();
	this->openOut();
	vector<byte> bytes = parseUtils::getBytes(item);
	int ignore = -1;
	for (size_t pos = 0; pos < this->currentBytes.size(); pos++) {
		if (pos == position) {
#if __debug == true
			cout << "7 ";
#endif
			for (unsigned int i = 0; i < bytes.size(); i++) {
				this->fileOut << bytes[i];
			}
			pos += sizeof(item) - 1;
		}
		else {
#if __debug == true
			cout << "\n(pre) pos= " << pos << "\n";
			cout << "set: #(" << pos << ") to: $(" << currentBytes[pos] << ") (" << (int)currentBytes[pos] << ")\n";
#endif
			this->fileOut << currentBytes[pos];
		}
	}
	position += sizeof(item);
	this->fileOut.close();
}
#endif