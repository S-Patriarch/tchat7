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

#include <thread>

////////////////////////////////////////////////////////////////////////////////
int main()
{
    ptl::pTCPClient tcp;
    ptl::pColor     color;

    chat::User     user;
    chat::Message  msg;
    chat::Exchange exchange;

    ptl::clrscr();
    ptl::hcrs();
    chat::get_logo(chat::CLIENT);

    //--------------------------------------------------------------------------
    // Подключение к серверу.
    //
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

    std::cout 
        << color.esc_tb(ptl::Color::CYAN)
        << "chat"
        << color.esc_c()
        << ": Соединение с сервером установлено\n";

    //--------------------------------------------------------------------------
    // Авторизация/регистрация клиента.
    //
    bool isOk_{false};

    ptl::scrs();
    isOk_ = chat::authorization(tcp, user, exchange);
    ptl::hcrs();

    if (!isOk_) {
        // действия : регистрация | выход из чата
        std::cout << '\n';
        std::cout 
            << color.esc_tb(ptl::Color::CYAN)
            << "chat"
            << color.esc_c()
            << ": Такого пользователя нет, пройдите регистрацию\n";
        ptl::scrs();
        isOk_ = chat::registration(tcp, user, exchange);
        ptl::hcrs();
    }

    //--------------------------------------------------------------------------
    // Приглашение клиента к работе.
    //
    ptl::clrscr();
    chat::get_logo(chat::CLIENT);
    chat::get_info();
    chat::get_help();

    std::cout 
        << color.esc_tb(ptl::Color::CYAN)
        << "chat"
        << color.esc_c()
        << ": Добро пожаловать "
        << user.s_userName 
        << ' '
        << user.s_userFamaly
        << "\n\n";

    //--------------------------------------------------------------------------
    // Цикл обмена сообщениями.
    //
    char msgBuffer[chat::MAX_PACKET_SIZE];
    while (true) {
        //----------------------------------------------------------------------
        // проверка на наличие в базе не прочитанных сообщений
        isOk_ = chat::out_message(tcp, user, exchange);
        if (isOk_) {
            std::string strStatus_ = "1|1";
            isOk_ = chat::message_status(tcp, user, exchange, strStatus_);
        }

        ptl::scrs();

        std::string sColor {};
        sColor += user.s_userName + ' ' + user.s_userFamaly + '\n';
        chat::out_color_string(sColor, user.s_userColor);

        // кому сообщение
        std::cout << "к: ";
        std::memset(&msgBuffer[0], 0, sizeof(msgBuffer));
        fgets(msgBuffer, sizeof(msgBuffer), stdin);
        msg.s_msgToWhom = chat::remove_last(msgBuffer, '\n');

        //----------------------------------------------------------------------
        // выход клиента из чата
        if (std::strncmp("-q", msg.s_msgToWhom.c_str(), 2) == 0 ||
            std::strncmp("-Q", msg.s_msgToWhom.c_str(), 2) == 0) {

            exchange.strSendAnswer = "QUIT|" + user.s_userEmail + "|";
            tcp.Send(exchange.strSendAnswer);
            chat::out_client_quit(user);
            tcp.exit();
            ptl::scrs();
            return 0;
        }
        //----------------------------------------------------------------------
        // предоставление информации по работе с чатом
        else if (std::strncmp("-h", msg.s_msgToWhom.c_str(), 2) == 0 ||
                 std::strncmp("-H", msg.s_msgToWhom.c_str(), 2) == 0 ||
                 std::strncmp("-?", msg.s_msgToWhom.c_str(), 2) == 0) {

            ptl::clrscr();
            chat::get_info();
            chat::get_help();
        }
        //----------------------------------------------------------------------
        // редактирование данных клиента
        else if (std::strncmp("-e", msg.s_msgToWhom.c_str(), 2) == 0 ||
                 std::strncmp("-E", msg.s_msgToWhom.c_str(), 2) == 0) {

            isOk_ = chat::edit_user(tcp, user, exchange);

            if (isOk_) {
                ptl::clrscr();
                chat::get_logo(chat::CLIENT);
                chat::get_info();
                chat::get_help();

                std::cout 
                    << color.esc_tb(ptl::Color::CYAN)
                    << "chat"
                    << color.esc_c()
                    << ": Добро пожаловать "
                    << user.s_userName 
                    << ' '
                    << user.s_userFamaly
                    << "\n\n";
            }
            else if (!isOk_) {
                // действия :
            }
        }
        //----------------------------------------------------------------------
        // удаление клиента
        else if (std::strncmp("-d", msg.s_msgToWhom.c_str(), 2) == 0 ||
                 std::strncmp("-D", msg.s_msgToWhom.c_str(), 2) == 0) {

            isOk_ = chat::delete_user(tcp, user, exchange);

            if (isOk_) {
                ptl::clrscr();

                ptl::scrs();
                isOk_ = chat::authorization(tcp, user, exchange);
                ptl::hcrs();

                if (!isOk_) {
                    // действия : регистрация | выход из чата
                    std::cout << '\n';
                    std::cout 
                        << color.esc_tb(ptl::Color::CYAN)
                        << "chat"
                        << color.esc_c()
                        << ": Такого пользователя нет, пройдите регистрацию\n";
                    ptl::scrs();
                    isOk_ = chat::registration(tcp, user, exchange);
                    ptl::hcrs();
                }

                ptl::clrscr();
                chat::get_logo(chat::CLIENT);
                chat::get_info();
                chat::get_help();

                std::cout 
                    << color.esc_tb(ptl::Color::CYAN)
                    << "chat"
                    << color.esc_c()
                    << ": Добро пожаловать "
                    << user.s_userName 
                    << ' '
                    << user.s_userFamaly
                    << "\n\n";
            }
            else if (!isOk_) {
                // действия :
            }
        }
        //----------------------------------------------------------------------
        // ввод сообщения и его отправка на сервер
        else { 
            std::cout << "ч: ";
            std::memset(&msgBuffer[0], 0, sizeof(msgBuffer));
            fgets(msgBuffer, sizeof(msgBuffer), stdin);
            msg.s_msgText = chat::remove_last(msgBuffer, '\n');

            // от кого сообщение
            msg.s_msgFromWhom = user.s_userID;

            isOk_ = chat::in_message(tcp, msg, exchange);

            if (!isOk_) {
                std::cout 
                    << color.esc_tb(ptl::Color::CYAN)
                    << "\nchat"
                    << color.esc_c()
                    << ": Такой пользователь не зарегистрирован в чате\n"
                    << '\n';
            }
            else {
                std::cout << '\n';
            }

            ptl::hcrs();
        }

    } // while (true) 
}
