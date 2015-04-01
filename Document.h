#ifndef INC_DOCUMENT_H
#define INC_DOCUMENT_H

#include <istream>
#include <sstream>

class Document
{
public:
	Document(std::string input) : text_(input), charPosition_(0)/*, currentLine_(0)*/
	{
		// Handle windows formate of endline(\r\n)
		if (text_[text_.size() - 1] == '\r') {
			text_[text_.size() - 1] = '\n';
		}
		// Add an extra endline at the end if there are none
		if (text_[text_.size() -1] != '\n') {
			text_.append("\n");
		}
	}

	const char nextChar();
	int getCharPosition();
	int getTextSize();
	// const int getLineNumber();
	// void setLineNumber(const int num);

private:
	// bool readline();

	std::string text_;
	int charPosition_;
	// int currentLine_;
};

// const int Document::getLineNumber() {
// 	return currentLine_;
// }

// void Document::setLineNumber(const int num) {
// 	currentLine_ = num;
// }

const char Document::nextChar() {
	if (charPosition_ >= text_.size()) {
		return 0;
	}
	return text_[charPosition_];
}

int Document::getCharPosition() {
	return charPosition_;
}

int Document::getTextSize() {
	return text_.size();
}

#endif /* INC_DOCUMENT_H */