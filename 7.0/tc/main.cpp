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

    std::cout << color.esc_tb(ptl::Color::GREEN)
              << "chat"
              << color.esc_c()
              << ": Добро пожаловать "
              << user.s_userName
              << " "
              << user.s_userFamaly
              << '\n';

    isOk_ = chat::out_message(tcp, user, exchange);
    if (!isOk_) {
        // действия : ...
        std::cout << color.esc_tb(ptl::Color::GREEN)
                  << "chat"
                  << color.esc_c()
                  << ": Для Вас нет сообщений...\n";
    }





    exchange.strSendAnswer = "QUIT|-q|";
    tcp.Send(exchange.strSendAnswer);

    tcp.exit();

    std::cout << std::endl;
    std::cout << color.esc_tb(ptl::Color::GREEN)
              << "chat"
              << color.esc_c()
              << ": До новых встреч "
              << user.s_userName
              << " "
              << user.s_userFamaly
              << '\n';

    ptl::scrs();
    return 0;
}
