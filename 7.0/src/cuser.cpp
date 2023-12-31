/*++

    (c) S-Patriarch, 2023

--*/

#include "../include/cuser.h"

////////////////////////////////////////////////////////////////////////////////
namespace chat {

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

    std::cout 
        << color.esc_tb(ptl::Color::CYAN)
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
    std::cout << "\033[?25l";
    exchange.strSendAnswer += chat::get_hidden_input() + "|";
    std::cout << "\033[?25h";

    tcp.Send(exchange.strSendAnswer);
    exchange.strReadRequest = tcp.read();

    if (exchange.strReadRequest != "NO") {
        // strReadRequest {id|user_name|user_surname|}
        subStringRequest = chat::parsing_string(exchange.strReadRequest, '|');

        user.s_userID     = subStringRequest[0];
        user.s_userName   = subStringRequest[1];
        user.s_userFamaly = subStringRequest[2];
        user.s_userColor  = get_random_number(1, 7);

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

    std::cout 
        << color.esc_tb(ptl::Color::CYAN)
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
    std::cout << "\033[?25l";
    exchange.strSendAnswer += chat::get_hidden_input() + "|";
    std::cout << "\033[?25h";

    tcp.Send(exchange.strSendAnswer);
    exchange.strReadRequest = tcp.read();

    if (exchange.strReadRequest != "NO") {
        // strReadRequest {id|}
        subStringRequest = chat::parsing_string(exchange.strReadRequest, '|');
        user.s_userID    = subStringRequest[0];
        user.s_userColor = get_random_number(1, 7);
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
        // strReadRequest {OK}
        user.s_userID     = "";
        user.s_userName   = "";
        user.s_userFamaly = "";
        user.s_userEmail  = "";

        isResultReturn_ = true;
    }

    return isResultReturn_;
}

//------------------------------------------------------------------------------
bool
edit_user(ptl::pTCPClient& tcp,
          User& user,
          Exchange& exchange)
{
    ptl::pColor color;

    bool isResultReturn_ {false};
    char msgBuffer[chat::MAX_PACKET_SIZE];

    std::cout 
        << color.esc_tb(ptl::Color::CYAN)
        << "chat"
        << color.esc_c()
        << ": Редактирование данных пользователя...\n\n";

    exchange.strSendAnswer = "EDIT|" + user.s_userEmail + "|";

    // ввод нового логина пользователя
    std::cout << "Новая электронная почта: ";
    std::memset(&msgBuffer[0], 0, sizeof(msgBuffer));
    fgets(msgBuffer, sizeof(msgBuffer), stdin);
    std::string newUserEmail_ = chat::remove_last(msgBuffer, '\n');
    exchange.strSendAnswer += newUserEmail_ + "|";

    // ввод нового пароля
    std::cout << "Новый пароль: ";
    std::cout << "\033[?25l";
    exchange.strSendAnswer += chat::get_hidden_input() + "|";
    std::cout << "\033[?25h";

    tcp.Send(exchange.strSendAnswer);
    exchange.strReadRequest = tcp.read();

    if (exchange.strReadRequest != "NO") {
        // strReadRequest {OK}
        user.s_userEmail = newUserEmail_;
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
            std::cout 
                << color.esc_tb(ptl::Color::CYAN)
                << "M"
                << color.esc_c()
                << ": ";
            std::cout << i << '\n';
        }

        std::cout << std::endl;
        isResultReturn_ = true;
    }

    return isResultReturn_;
}

//------------------------------------------------------------------------------
bool
in_message(ptl::pTCPClient& tcp,
           Message& msg,
           Exchange& exchange)
{
    bool isResultReturn_ {false};

    exchange.strSendAnswer =
        "MESSAGE_TO_DATABASE|" + msg.s_msgFromWhom + "|"
                               + msg.s_msgToWhom   + "|"
                               + msg.s_msgText     + "|";

    tcp.Send(exchange.strSendAnswer);
    exchange.strReadRequest = tcp.read();

    if (exchange.strReadRequest != "NO") {
        // strReadRequest {OK}
        isResultReturn_ = true;
    }

    return isResultReturn_;
}

//------------------------------------------------------------------------------
bool
message_status(ptl::pTCPClient& tcp,
               User& user,
               Exchange& exchange,
               std::string& strStatus_)
{
    bool isResultReturn_ {false};

    exchange.strSendAnswer =
        "MESSAGE_STATUS|" + user.s_userID + "|"
                          + strStatus_    + "|";

    tcp.Send(exchange.strSendAnswer);
    exchange.strReadRequest = tcp.read();

    if (exchange.strReadRequest != "NO") {
        // strReadRequest {OK}
        isResultReturn_ = true;
    }

    return isResultReturn_;
}

} // manespace chat
