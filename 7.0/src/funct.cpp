/*++

    (c) S-Patriarch, 2023

--*/

#include "../include/funct.h"

////////////////////////////////////////////////////////////////////////////////
namespace chat {

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
    std::cout << '\n';
    std::cout 
        << "Клиент покинул чат\n"
        << "Сервер завершает работу...\n"
        << "До новых встреч\n";
}

//------------------------------------------------------------------------------
void
out_client_quit(User& user)
{
    ptl::pColor color;

    std::cout 
        << color.esc_tb(ptl::Color::CYAN)
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
    } else if (s_ == "NO") {
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

//------------------------------------------------------------------------------
std::string
sha256(const std::string& str_)
{
    unsigned char hash_[SHA256_DIGEST_LENGTH];

    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str_.c_str(), str_.size());
    SHA256_Final(hash_, &sha256);

    std::stringstream ss_;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        ss_ << std::hex << std::setw(2) << std::setfill('0') << (int)hash_[i];
    }

    return ss_.str();
}

//------------------------------------------------------------------------------
std::string 
get_hidden_input() 
{
    struct termios oldt;
    struct termios newt;
    
    std::string strResult{};

    // Сохраняем текущие настройки терминала
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~ECHO; // Отключаем отображение ввода

    // Применяем новые настройки терминала
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Читаем ввод пользователя
    std::getline(std::cin, strResult);

    // Востанавливаем старые настройки терминала
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return strResult;
}

//------------------------------------------------------------------------------
unsigned 
int get_random_number(int disBegin, int disEnd)
{
    std::random_device rd;  // Получение случайного устройства
    std::mt19937 gen(rd()); // Генератор случайных чисел
    // Получение числа
    std::uniform_int_distribution<unsigned int> dis(disBegin, disEnd);
    return dis(gen);
}

//------------------------------------------------------------------------------
void 
out_color_string(const std::string& strOutput, unsigned int nColor)
{
    ptl::pColor color;
    
    switch (nColor) {
        case 1 : std::cout << color.esc_tr(ptl::Color::RED)     << strOutput; break;
        case 2 : std::cout << color.esc_tr(ptl::Color::GREEN)   << strOutput; break;
        case 3 : std::cout << color.esc_tr(ptl::Color::YELLOW)  << strOutput; break;
        case 4 : std::cout << color.esc_tr(ptl::Color::BLUE)    << strOutput; break;
        case 5 : std::cout << color.esc_tr(ptl::Color::MAGENTA) << strOutput; break;
        case 6 : std::cout << color.esc_tr(ptl::Color::CYAN)    << strOutput; break;
        case 7 : std::cout << color.esc_tr(ptl::Color::WHITE)   << strOutput; break;
    }
    std::cout << color.esc_c();
}

} // manespace chat
