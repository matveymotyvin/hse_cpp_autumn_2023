#include <assert.h>
#include <sstream>
#include "serial.hpp"

// сериализация в поток
Serializer::Serializer(std::ostream& out)
    : out_(out) {}

// сохранение обьектов при вызове метода serialize
template <class T>
Error Serializer::save(T& object)
{
    return object.serialize(*this);
}

// оператор () для сериализации произвольного числа аргументов
template <class... ArgsT>
Error Serializer::operator()(ArgsT... args)
{
    return process(args...);
}

// сериализация bool
Error Serializer::process(bool&& arg)
{
    out_ << std::boolalpha << arg; // сериализуем bool с использованием boolalpha
    return Error::NoError;
}

// сериализация uint64_t
Error Serializer::process(uint64_t&& arg)
{
    out_ << arg;
    return Error::NoError;
}

// больше данных для сериализации нет
Error Serializer::process()
{
    return Error::NoError;
}

// рекурсивная функция для сериализации произвольного числа аргументов
template<class T, class... Args>
Error Serializer::process(T&& val, Args&&... args)
{
    out_ << std::boolalpha << val << Separator; // сериализуем текущий аргумент с разделителем Separator
    return process(std::forward<Args>(args)...); // рекурсивно вызываем для остальных аргументов
}

// десериализация данных из потока
Deserializer::Deserializer(std::istream& in)
    : in_(in) {}

// загрузика объект при вызове метода deserialize
template <class T>
Error Deserializer::load(T& object)
{
    return object.deserialize(*this);
}

// оператор () для десериализации произвольного числа аргументов
template <class... ArgsT>
Error Deserializer::operator()(ArgsT&... args)
{
    return process(args...);
}

// десериализация bool
template<bool>
Error Deserializer::process(bool& arg)
{
    return process_(arg);
}

// десериализация uint64_t
template<uint64_t>
Error Deserializer::process(uint64_t& arg)
{
    return process_(arg);
}

// максимальный uint64_t 
const std::string mx = std::to_string(UINT64_MAX);

// больше данных для десериализации нет
Error Deserializer::process()
{
    return Error::NoError;
}

// рекурсивная функция для десериализации произвольного числа аргументов
// аналогично сериализации
template<class T, class... Args>
Error Deserializer::process(T&& val, Args&&... args)
{
    process_(val);
    return process(std::forward<Args>(args)...);
}

Error Deserializer::process_(bool& arg)
{
    std::string s;
    in_ >> s; 
    if (s == "true") arg = true;
    else if (s == "false") arg = false;
    else return Error::CorruptedArchive;
    return Error::NoError;
}

// проверка является ли строка числом
inline bool Deserializer::is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}


Error Deserializer::process_(uint64_t& arg)
{
    std::string s;
    in_ >> s; 
    if (s.size() > mx.size() || (s.size() == mx.size() && s > mx) || !is_number(s))
        return Error::CorruptedArchive;
    std::istringstream iss(s);
    iss >> arg;
    return Error::NoError;
}

// инстанцируем шаблоны
template
Error Serializer::save(Data&);

template
Error Deserializer::load(Data&);

template <class Serializer>
Error Data::serialize(Serializer& serializer)
{
	return serializer(a, b, c);
}

template <class Deserializer>
Error Data::deserialize(Deserializer& deserializer)
{
	return deserializer(a, b, c);
}