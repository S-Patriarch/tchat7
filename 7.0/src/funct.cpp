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
    std::cout << "M: Клиент покинул чат\n"
              << "M: Сервер завершает работу...\n";

    std::cout << std::endl;
    std::cout << "M: До новых встреч\n";
}

//------------------------------------------------------------------------------
void
out_client_quit(User& user)
{
    ptl::pColor color;

    std::cout << std::endl;
    std::cout << color.esc_tb(ptl::Color::GREEN)
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

} // manespace chat
