/*++

    (c) S-Patriarch, 2023

--*/

#include "../include/cuser.h"

////////////////////////////////////////////////////////////////////////////////
namespace chat
{
//------------------------------------------------------------------------------
bool
authorization(ptl::pTCPClient& tcp,
              User& user,
              Exchange& exchange)
{
    ptl::pColor color;

    std::vector<std::string> subStringRequest {};

    bool isResultReturn_ {false};
    char msgBuffer[chat::MAX_PACKET_SIZE];

    std::cout << color.esc_tb(ptl::Color::CYAN)
              << "chat"
              << color.esc_c()
              << ": Авторизация пользователя...\n\n";

    exchange.strSendAnswer = "AUTHORIZATION|";

    // ввод логина пользователя
    std::cout << "Электронная почта: ";
    std::memset(&msgBuffer[0], 0, sizeof(msgBuffer));
    fgets(msgBuffer, sizeof(msgBuffer), stdin);
    user.s_userEmail = chat::remove_last(msgBuffer, '\n');
    exchange.strSendAnswer += user.s_userEmail + "|";

    // ввод пароля
    std::cout << "Пароль: ";
    std::memset(&msgBuffer[0], 0, sizeof(msgBuffer));
    std::cout << "\033[?25l";
    std::cout << color.esc_tr(color.getbkgcolor());
    fgets(msgBuffer, sizeof(msgBuffer), stdin);
    std::cout << color.esc_c();
    std::cout << "\033[?25h";
    exchange.strSendAnswer += chat::remove_last(msgBuffer, '\n') + "|";

    tcp.Send(exchange.strSendAnswer);
    exchange.strReadRequest = tcp.read();

    if (exchange.strReadRequest != "NO") {
        // strReadRequest {id|user_name|user_surname|}
        subStringRequest = chat::parsing_string(exchange.strReadRequest, '|');

        user.s_userID     = subStringRequest[0];
        user.s_userName   = subStringRequest[1];
        user.s_userFamaly = subStringRequest[2];

        isResultReturn_ = true;
    }

    return isResultReturn_;
}

//------------------------------------------------------------------------------
bool
registration(ptl::pTCPClient& tcp,
             User& user,
             Exchange& exchange)
{
    ptl::pColor color;

    std::vector<std::string> subStringRequest {};

    bool isResultReturn_ {false};
    char msgBuffer[chat::MAX_PACKET_SIZE];

    std::cout << color.esc_tb(ptl::Color::CYAN)
              << "chat"
              << color.esc_c()
              << ": Регистрация пользователя...\n\n";

    exchange.strSendAnswer = "REGISTRATION|";

    // ввод имени пользователя
    std::cout << "Имя: ";
    std::memset(&msgBuffer[0], 0, sizeof(msgBuffer));
    fgets(msgBuffer, sizeof(msgBuffer), stdin);
    user.s_userName = chat::remove_last(msgBuffer, '\n');
    exchange.strSendAnswer += user.s_userName + "|";

    // ввод фамилии пользователя
    std::cout << "Фамилия: ";
    std::memset(&msgBuffer[0], 0, sizeof(msgBuffer));
    fgets(msgBuffer, sizeof(msgBuffer), stdin);
    user.s_userFamaly = chat::remove_last(msgBuffer, '\n');
    exchange.strSendAnswer += user.s_userFamaly + "|";

    // ввод логина пользователя
    std::cout << "Электронная почта: ";
    std::memset(&msgBuffer[0], 0, sizeof(msgBuffer));
    fgets(msgBuffer, sizeof(msgBuffer), stdin);
    user.s_userEmail = chat::remove_last(msgBuffer, '\n');
    exchange.strSendAnswer += user.s_userEmail + "|";

    // ввод пароля
    std::cout << "Пароль: ";
    std::memset(&msgBuffer[0], 0, sizeof(msgBuffer));
    std::cout << "\033[?25l";
    std::cout << color.esc_tr(color.getbkgcolor());
    fgets(msgBuffer, sizeof(msgBuffer), stdin);
    std::cout << color.esc_c();
    std::cout << "\033[?25h";
    exchange.strSendAnswer += chat::remove_last(msgBuffer, '\n') + "|";

    tcp.Send(exchange.strSendAnswer);
    exchange.strReadRequest = tcp.read();

    if (exchange.strReadRequest != "NO") {
        // strReadRequest {id|}
        subStringRequest = chat::parsing_string(exchange.strReadRequest, '|');
        user.s_userID    = subStringRequest[0];
        isResultReturn_  = true;
    }

    return isResultReturn_;
}

//------------------------------------------------------------------------------
bool
delete_user(ptl::pTCPClient& tcp,
            User& user,
            Exchange& exchange)
{
    bool isResultReturn_ {false};

    exchange.strSendAnswer = "DELETE|" + user.s_userID + "|";

    tcp.Send(exchange.strSendAnswer);
    exchange.strReadRequest = tcp.read();

    if (exchange.strReadRequest != "NO") {
        // strReadRequest {OK|}
        user.s_userID     = "";
        user.s_userName   = "";
        user.s_userFamaly = "";
        user.s_userEmail  = "";

        isResultReturn_  = true;
    }

    return isResultReturn_;
}

//------------------------------------------------------------------------------
bool
out_message(ptl::pTCPClient& tcp,
            User& user,
            Exchange& exchange)
{
    ptl::pColor color;

    bool isResultReturn_ {false};

    exchange.strSendAnswer = "MESSAGE_FROM_DATABASE|" + user.s_userID + "|";
    tcp.Send(exchange.strSendAnswer);
    exchange.strReadRequest = tcp.read();

    if (exchange.strReadRequest != "NO") {
        // разбиваем полученную строку от сервера на подстроки
        std::vector<std::string> subStrings__ {};
        subStrings__ = chat::parsing_string(exchange.strReadRequest, '|');

        for (const auto& i : subStrings__) {
            std::cout << color.esc_tb(ptl::Color::CYAN)
                      << "M"
                      << color.esc_c()
                      << ": ";
            std::cout << i << std::endl;
        }

        std::cout << std::endl;
        isResultReturn_ = true;
    }

    return isResultReturn_;
}

} // manespace chat
