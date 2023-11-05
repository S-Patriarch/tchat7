/*++

    (c) S-Patriarch, 2023

--*/

#include "../include/funct.h"

////////////////////////////////////////////////////////////////////////////////
namespace chat
{
//------------------------------------------------------------------------------
std::string
remove_last(const std::string& str,
            const char ch)
{
    std::string strResult {}; // возвращаемый результат

    // находим позицию последнего символа, отличного от заданного
    std::size_t found = str.find_last_not_of(ch);

    // если такой символ найден, удаляем символы после него
    if (found != std::string::npos) {
        strResult = str.substr(0, found+1);
    }

    return strResult;
}

//------------------------------------------------------------------------------
std::vector<std::string>
parsing_string(const std::string& str,
               const char ch)
{
    std::vector<std::string> subStrings {};
    std::istringstream iss(str);
    std::string subString {};

    while (std::getline(iss, subString, ch)) {
        subStrings.push_back(subString);
    }

    return subStrings;
}

//------------------------------------------------------------------------------
void
out_server_quit()
{
    std::cout << std::endl;
    std::cout << "Клиент покинул чат\n"
              << "Сервер завершает работу...\n"
              << "До новых встреч\n";
}

//------------------------------------------------------------------------------
void
out_client_quit(User& user)
{
    ptl::pColor color;

    std::cout << color.esc_tb(ptl::Color::CYAN)
              << "chat"
              << color.esc_c()
              << ": До новых встреч "
              << user.s_userName << " " << user.s_userFamaly
              << '\n';
}

//------------------------------------------------------------------------------
void
out_OK_NO(const std::string& s_)
{
    if (s_ == "OK") {
        std::cout << "OK\n";
    }
    else if (s_ == "NO") {
        std::cout << "NO\n";
    }
}

//------------------------------------------------------------------------------
std::string
get_date()
{
    std::chrono::system_clock::time_point today = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(today);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&t), "%Y-%m-%d");
    std::string currentDate = ss.str();

    return currentDate;
}

//------------------------------------------------------------------------------
std::string
get_time()
{
    // получаем текущее время
    std::time_t now = std::time(nullptr);

    // преобразуем текущее время в локальное время
    std::tm* localTime = std::localtime(&now);

    // форматируем локальное время в строку
    char buffer[9];
    std::strftime(buffer, sizeof(buffer), "%H:%M:%S", localTime);

    // преобразуем буфер в std::string
    std::string currentTime(buffer);

    return currentTime;
}

} // manespace chat
