#include "SilverBinaryFile.h"
#include "../Parsing/utils.hpp"

//#include <iostream>

typedef unsigned char byte;

using std::string;
using std::ofstream;
using std::ifstream;
using std::fstream;
using std::vector;

bool SilverBinaryFile::openIn()
{
	this->fileIn.open(filePath, std::ios::in | std::ios::binary);
	if (fileIn.fail()) {
		if (errno == ENOENT) {
			this->fileOut.open(filePath);
			fileOut.close();
			std::flush(fileOut);
			this->openIn();
		}
		else {
			return false;
		}
	}
	return true;
}

bool SilverBinaryFile::openOut()
{
	this->fileOut.open(filePath, std::ios::out | std::ios::binary);
	if (fileIn.fail()) {
		return false;
	}
	return true;
}

void SilverBinaryFile::jump(size_t amt)
{
	this->position += amt;
}

void SilverBinaryFile::locate(size_t amt)
{
	this->position = amt;
}

SilverBinaryFile::SilverBinaryFile(string filePath)
{
	this->filePath = filePath;
}

/*SilverBinaryFile::~SilverBinaryFile()
{
	if (this->fileIn.is_open()) this->fileIn.close();
	if (this->fileOut.is_open()) this->fileOut.close();
}*/

void SilverBinaryFile::clearPosition()
{
	this->position = 0;
}

bool SilverBinaryFile::fetchBytes()
{
	vector<byte> val = {};
	try {
		this->openIn();
		char byte;
		for (size_t i = 0; !(fileIn.read(&byte, 1).eof()); i++) {
			val.push_back((unsigned char)byte);
		};
		this->fileIn.close();
	}
	catch (std::exception) {
	}
	this->currentBytes = val;
	if (val.size() == 0) return false;
	return true;
}

string SilverBinaryFile::nextString(unsigned int max) 
{
	vector<char> chars;
	for (unsigned int strSize = 0; strSize < max; strSize++) {
		bool success = true;
		char nextChar = next<char>(&success);
		this->position++;
		if (!success) break;
//		std::cout << "[string builder] next char = " << nextChar << '\n';
		if (nextChar == '\0') {
			break;
		};
		chars.push_back(nextChar);
	}
	return string(chars.begin(), chars.end());
}