#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include <set>
#include <algorithm>
#include <exception>
#include <cstring>
#include <map>

class Formater
{
    static constexpr char Separator = ' ';

public:
    // конструктор форматера, принимающий ссылку на выходной поток и строку для форматирования
    explicit Formater(std::stringstream& out, std::string& s) : out_(out), line_(s) {}

    // метод для проверки, является ли строка числом
    bool is_number(const std::string& s);

    // метод для получения параметров из входного потока
    void getParams();

    // метод для получения строки из форматера после замены плейсхолдеров
    std::string getString();

    // метод для замены всех вхождений подстроки from на to в строке str
    void replaceAll(std::string& str, const std::string& from, const std::string& to);

    // метод для проверки строки на наличие плейсхолдеров и получения их индексов
    std::set<int> checkString();

    // оператор () для добавления параметров в форматер
	// variadic function
    template <class T, class... ArgsT>
    void operator()(T&& val, ArgsT&&... args)
    {
        count++;
        out_ << std::boolalpha;
        out_ << Separator;
        out_ << val;
        (*this)(std::forward<ArgsT>(args)...);
    }

    // оператор () для добавления последнего параметра в форматер
    template <class T>
    void operator()(T&& val)
    {
        count++;
        out_ << std::boolalpha;
        out_ << Separator;
        out_ << val;
    }

private:
    std::stringstream& out_;     // ссылка на выходной поток
    std::string& line_;          // ссылка на строку для форматирования
    int count = 0;               // счетчик параметров
    std::vector<std::string> params_;  // вектор для хранения параметров
};

// класс исключения для ошибки в количестве параметров
class IncorrectParamsNumberException : public std::exception
{
    std::string _msg;

public:
    IncorrectParamsNumberException(const std::string& msg) : _msg(msg) {}

    // метод для получения сообщения об ошибке
    virtual const char* what() const noexcept override
    {
        return _msg.c_str();
    }
};

// исключение для несовпадения открывающих и закрывающих скобок
class BracketsMismatchException : public std::exception
{
    std::string _msg;

public:
    BracketsMismatchException(const std::string& msg) : _msg(msg) {}

    virtual const char* what() const noexcept override
    {
        return _msg.c_str();
    }
};

// исключение для отсутствия значения у плейсхолдера
class EmptyParamException : public std::exception
{
    std::string _msg;

public:
    EmptyParamException(const std::string& msg) : _msg(msg) {}

    virtual const char* what() const noexcept override
    {
        return _msg.c_str();
    }
};

// исключение для ошибки в значении плейсхолдера
class NotANumberException : public std::exception
{
    std::string _msg;

public:
    NotANumberException(const std::string& msg) : _msg(msg) {}

    virtual const char* what() const noexcept override
    {
        return _msg.c_str();
    }
};
