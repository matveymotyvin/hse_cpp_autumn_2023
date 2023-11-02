#pragma once 

#include <iostream> 
#include <string>
#include <type_traits>
#include <cstdint>

// перечисление для возможных ошибки при сериализации и десериализации
enum class Error
{
	NoError,
	CorruptedArchive
};

// класс Serializer для сериализации данных
class Serializer
{
	static constexpr char Separator = ' ';
	std::ostream& out_; // ссылка на поток вывода, куда будут записываться данные
public:
	explicit Serializer(std::ostream& out);
	template <class T>
	Error save(T& object); // метод для сохранения объекта в поток
	template <class... ArgsT>
	Error operator()(ArgsT... args); // оператор для сериализации нескольких объектов
	Error process(bool&& arg); // метод для сериализации bool
	Error process(uint64_t&& arg); // метод для сериализации uint64_t
private:
	Error process();

	template<class T, class... Args>
	Error process(T&& val, Args&&... args);

	Error process_(bool&& arg);
	Error process_(uint64_t&& arg);
};

// класс Deserializer для десериализации данных
// методы аналогичны Serializer
class Deserializer
{
	std::istream& in_;
public:
	explicit Deserializer(std::istream& in);
	template <class T>
	Error load(T& object);
	template <class... ArgsT>
	Error operator()(ArgsT&... args);
	template<bool>
	Error process(bool& arg);
	template<uint64_t>
	Error process(uint64_t& arg);
private:
	const std::string mx = std::to_string(UINT64_MAX); // максимальное значение для uint64_t
	Error process();

	template<class T, class... Args>
	Error process(T&& val, Args&&... args);

	Error process_(bool& arg);
	inline bool is_number(const std::string& s); // проверка, является ли строка числом
	Error process_(uint64_t& arg);
};

// структура Data для сериализации и десериализации данных
struct Data
{
	uint64_t a;
	bool b;
	uint64_t c;

	template <class Serializer>
	Error serialize(Serializer& serializer);

	template <class Deserializer>
	Error deserialize(Deserializer& deserializer);
};
