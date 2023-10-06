#include "parser.hpp"
#include <iostream>



void TokenParser::SetStartCallback(letter_func func) {
	StartCallback = func;
}

void TokenParser::SetEndCallback(letter_func func) {
    EndCallback = func;
}

void TokenParser::SetDigitTokenCallback(digit_func func) {
    DigitTokenCallback = func;
}

void TokenParser::TokenParser::SetLetterTokenCallback(letter_func func) {
    LetterTokenCallback = func;
}

uint64_t TokenParser::convert(std::string str) {
    uint64_t i = std::stoi(str);
    return i;
}

bool TokenParser::isDigit(std::string str) {
    for (auto ch : str) {
        if (std::isdigit(ch) == 0) 
            return false;
    }
    return true;
}

std::string TokenParser::Parse(const std::string & str)
    {
        std::string new_str = "";
        std::string work_str = str;

        if (StartCallback != nullptr)
             work_str = StartCallback(work_str); //callback to string before parsing

        std::string word = "";
        work_str += " ";

        for (auto ch : work_str) {
            if ((ch == ' ' || ch == '\t' || ch == '\n') && word.length() > 0) {
                if (isDigit(word) and DigitTokenCallback != nullptr) { // only for digits token

                    try{ // try to fetch into uint64_t size
                        uint64_t word_num = std::stoul(word);
                        word_num = DigitTokenCallback(word_num);
                        word = std::to_string(word_num);
                    }

                    catch (...) {
                        if (LetterTokenCallback != nullptr)
                            word = LetterTokenCallback(word);
                    }
                }

                else if (LetterTokenCallback != nullptr) // only for letters token
                    word = LetterTokenCallback(word);

                new_str = new_str + " " + word;
                word = "";

            }  else if (ch != ' '){
                word = word + ch;
            }
        }
        
    new_str = new_str + " " + word;
    if (EndCallback != nullptr)
        new_str = EndCallback(new_str); // callback to result string after parsing

    if (new_str == " ")
            return str;
    return new_str;
    }
