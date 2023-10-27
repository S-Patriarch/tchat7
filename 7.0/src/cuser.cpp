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

    std::cout << color.esc_tb(ptl::Color::GREEN)
              << "chat"
              << color.esc_c()
              << ": Авторизация пользователя...\n";

    // ввод логина пользователя
    std::cout << "Электронная почта: ";

    std::memset(&msgBuffer[0], 0, sizeof(msgBuffer));

    std::cout << color.esc_tb(ptl::Color::WHITE);
    fgets(msgBuffer, sizeof(msgBuffer), stdin);
    std::cout << color.esc_c();

    user.s_userEmail = chat::remove_last(msgBuffer, '\n');
    exchange.strSendAnswer =
        "AUTHORIZATION|" + user.s_userEmail + "|";

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

    if (exchange.strReadRequest != "") {
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
out_message(ptl::pTCPClient& tcp,
            User& user,
            Exchange& exchange)
{
    ptl::pColor color;

    bool isResultReturn_ {false};

    exchange.strSendAnswer = "MESSAGE|" + user.s_userEmail + "|";
    tcp.Send(exchange.strSendAnswer);
    exchange.strReadRequest = tcp.read();

    if (exchange.strReadRequest != "NO") {
        std::cout << color.esc_tb(ptl::Color::GREEN)
                  << "chat"
                  << color.esc_c()
                  << ": Для Вас есть сообщения...\n";

        // разбиваем полученную строку от сервера на подстроки
        std::vector<std::string> subStrings__ {};
        subStrings__ = chat::parsing_string(exchange.strReadRequest, '|');

        for (const auto& i : subStrings__) {
             std::cout << i << std::endl;
        }

        isResultReturn_ = true;
    }

    return isResultReturn_;
}

} // manespace chat
