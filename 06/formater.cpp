#include "formater.hpp"

// метод получения строки из форматера после замены плейсхолдеров
std::string Formater::getString()
{
    try {
        getParams();  // получаем параметры из входного потока
        std::set<int> paramsToChange = checkString(); // проверяем строку на наличие плейсхолдеров и получаем их индексы
        int i = 0;
        for (auto it = paramsToChange.begin(); it != paramsToChange.end(); it++, i++)
        {
            if ((unsigned)*it > params_.size() - 1)
            {
                throw IncorrectParamsNumberException("params error");
            }
            std::string to = params_[*it];
            std::string substr = "{" + std::to_string(*it) + "}";
            replaceAll(line_, substr, to); // заменяем все вхождения плейсхолдера на соответствующее значение
        }
        std::stringstream().swap(out_); // очищаем входной поток
        out_ << line_; // записываем строку в выходной поток
    } catch (const IncorrectParamsNumberException& e) {
        throw;
    }
    return line_; // возвращаем измененную строку
}

// метод замены всех вхождений подстроки from на to в строке str
void Formater::replaceAll(std::string& str, const std::string& from, const std::string& to)
{
    if (from.empty())
        return;
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

// метод для проверки строки на наличие плейсхолдеров и получения их индексов
std::set<int> Formater::checkString()
{
    std::set<int> params;
    try {
        int counter = 0;
        std::vector<int> openPositions;
        std::vector<int> closePositions;
        for (std::string::iterator it = line_.begin(); it != line_.end(); ++it)
        {
            if (*it == '{')
            {
                openPositions.push_back(counter);
            }
            if (*it == '}')
            {
                closePositions.push_back(counter);
            }
            counter++;
        }
        if (openPositions.size() != closePositions.size())
        {
            throw BracketsMismatchException("miss {}");
        }

        int i = 0;

        for (auto it = openPositions.begin(); it != openPositions.end(); it++, i++)
        {

            int closePos = closePositions[i];
            if (closePos < *it || closePos == *it + 1)
            {
                throw EmptyParamException("miss format's index");
            }
            int length = closePos - *it + 1;
            std::string param = line_.substr(*it, length);
            std::string between = param.substr(1, param.length() - 2);
            if (is_number(between))
            {
                params.insert(atoi(between.c_str()));
            }
            else
            {
                throw NotANumberException("format's index error value");
            }
        }
        if (params_.size() != params.size())
        {
            throw IncorrectParamsNumberException("order number params error");
        }
    } catch (const IncorrectParamsNumberException& e) {
        throw;
    } catch (const BracketsMismatchException& e) {
        throw;
    } catch (const EmptyParamException& e) {
        throw;
    } catch (const NotANumberException& e) {
        throw;
    }
    return params; // возвращаем индексы найденных плейсхолдеров
}

// проверка, является ли строка s числом
bool Formater::is_number(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(), s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

// метод для получения параметров из входного потока
void Formater::getParams()
{
    std::string text;
    for (int i = 0; i < count; i++)
    {
        out_ >> text;
        params_.push_back(text);
    }
}
