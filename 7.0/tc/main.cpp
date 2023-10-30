/*++

    (c) S-Patriarch, 2023
    Терминальный чат.
    Версия 7.0 CLIENT

--*/

#include "../include/sct.h"
#include "../include/cst.h"
#include "../include/info.h"
#include "../include/funct.h"
#include "../include/cuser.h"

#include "ptl/pconio.h"
#include "ptl/pTCPClient.h"

////////////////////////////////////////////////////////////////////////////////
int main()
{
    ptl::pTCPClient tcp;
    ptl::pColor     color;

    chat::User     user;
    chat::Exchange exchange;

    ptl::clrscr();
    ptl::hcrs();
    chat::get_logo(chat::CLIENT);

    try {
        // CLIENT
        // создание сокета,
        // установка соединения с сервером
        tcp.setup(chat::IP_ADDRESS_SERVER, chat::PORT);
    }
    catch (std::exception& ex) {
        std::cout << ex.what() << '\n';
        ptl::scrs();
        return 0;
    }

    std::cout << color.esc_tb(ptl::Color::GREEN)
              << "chat"
              << color.esc_c()
              << ": Соединение с сервером установлено\n\n";

    bool isOk_{false};

///---
//exchange.strSendAnswer = "REGISTRATION|chaos|chaos|apsk0529@mail.ru|chaos|";
//tcp.Send(exchange.strSendAnswer);
//exchange.strReadRequest = tcp.read();
//std::cout << exchange.strReadRequest << "\n";
///---
//strSend_ = "REGISTRATION|Tester|Testerov|test@test.ru|Tester|";
//tcp.Send(exchange.strSendAnswer);
//exchange.strReadRequest = tcp.read();
//std::cout << exchange.strReadRequest << "\n";
///---
//strSend_ = "EDIT|test@test.ru|test2@test2.ru|Test_Testerov|";
//tcp.Send(exchange.strSendAnswer);
//exchange.strReadRequest = tcp.read();
//std::cout << exchange.strReadRequest << "\n";
///---
//strSend_ = "DELETE|test2@test2.ru|";
//tcp.Send(exchange.strSendAnswer);
//exchange.strReadRequest = tcp.read();
//std::cout << exchange.strReadRequest << "\n";
///---

    ptl::scrs();
    isOk_ = chat::authorization(tcp, user, exchange);
    ptl::hcrs();

    if (!isOk_) {
        // действия : повторный вход | регистрация | выход из чата
    }

    ptl::clrscr();
    chat::get_logo(chat::CLIENT);
    chat::get_info();

    std::cout << color.esc_tb(ptl::Color::GREEN)
              << "chat"
              << color.esc_c()
              << ": Добро пожаловать "
              << user.s_userName << " " << user.s_userFamaly
              << '\n';

    isOk_ = chat::out_message(tcp, user, exchange);
    if (!isOk_) {
        // действия : ...
        std::cout << color.esc_tb(ptl::Color::GREEN)
                  << "chat"
                  << color.esc_c()
                  << ": Для Вас нет сообщений...\n";
    }

    char msgBuffer[chat::MAX_PACKET_SIZE];
    while (true) {
        // организация : ввод сообщения

        std::memset(&msgBuffer[0], 0, sizeof(msgBuffer));

        std::cout << color.esc_tb(ptl::Color::WHITE)
                  << user.s_userName << " " << user.s_userFamaly
                  << color.esc_c()
                  << "\nmsg: ";

        ptl::scrs();
        fgets(msgBuffer, sizeof(msgBuffer), stdin);
        ptl::hcrs();

        // организация : обработка введенного сообщения,
        //               отправка введенного сообщения

        std::string strTemp_ = chat::remove_last(msgBuffer, '\n');

        if (std::strncmp("-q", strTemp_.c_str(), 2) == 0 ||
            std::strncmp("-Q", strTemp_.c_str(), 2) == 0) {
            exchange.strSendAnswer = "QUIT|-q|";
            tcp.Send(exchange.strSendAnswer);
            chat::out_client_quit(user);
            tcp.exit();
            ptl::scrs();
            return 0;
        }

    } // while (true) {}
}
