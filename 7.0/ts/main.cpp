/*++

    (c) S-Patriarch, 2023
    Терминальный чат.
    Версия 7.0 SERVER
    
--*/
#include "../include/sct.h"
#include "../include/cst.h"
#include "../include/info.h"
#include "../include/funct.h"
#include "../include/usingdb.h"

#include "ptl/pconio.h"
#include "ptl/pTCPServer.h"
//#include "ptl/plogger.h"

#include <pthread.h>

////////////////////////////////////////////////////////////////////////////////
int main()
{
    ptl::pTCPServer tcp;
    ptl::pColor     color;

    chat::Exchange exchange;

    bool isCheckDB_ = chat::db_existence_check();
    if (!isCheckDB_) {
        chat::db_create();
    }

    ptl::clrscr();
    ptl::hcrs();
    chat::get_logo(chat::SERVER);

    std::cout << color.esc_tb(ptl::Color::GREEN)
              << "chat"
              << color.esc_c()
              << ": Жду подключения клиента...\n";

    try {
        // SERVER
        // создание сокета, привязка сокета,
        // постановка сервера на прием данных
        tcp.setup(chat::PORT);
    }
    catch (std::exception& ex) {
        std::cout << ex.what() << '\n';
        ptl::scrs();
        return 0;
    }

    std::cout << color.esc_tb(ptl::Color::GREEN)
              << "chat"
              << color.esc_c()
              << ": OK\n";

    std::vector<std::string> subStringRequest {};
    bool isOk_ {false};

    while (true) {
        exchange.strReadRequest = tcp.read();
        subStringRequest = chat::parsing_string(exchange.strReadRequest, '|');

        // subStringRequest {QUIT, -q[-Q]}
        if (subStringRequest[0] == chat::QUIT) {
            if (std::strncmp("-q", subStringRequest[1].c_str(), 2) == 0 ||
                std::strncmp("-Q", subStringRequest[1].c_str(), 2) == 0) {
                chat::out_server_quit();
                tcp.exit();
                ptl::scrs();
                return 0;
            }
        }
        // subStringRequest {AUTHORIZATION, user_email, password}
        else if (subStringRequest[0] == chat::AUTHORIZATION) {
            isOk_ = chat::db_authorization(subStringRequest, exchange);
            if (isOk_) {
                // вход клиента выполнен успешно
                // strSendAnswer {id|user_name|user_surname|}
                chat::out_OK_NO(chat::OK);
                tcp.Send(exchange.strSendAnswer);
            }
            else {
                chat::out_OK_NO(chat::NO);
                exchange.strSendAnswer = chat::NO;
                tcp.Send(exchange.strSendAnswer);
            }
        }
        // subStringRequest {REGISTRATION,
        //                   user_name, user_surname, user_email, password}
        else if (subStringRequest[0] == chat::REGISTRATION) {
            isOk_ = chat::db_registration(subStringRequest, exchange);
            if (isOk_) {
                // регистрация клиента выполнена успешно
                // strSendAnswer {id|user_name|user_surname|}
                chat::out_OK_NO(chat::OK);
                tcp.Send(exchange.strSendAnswer);
            }
            else {
                chat::out_OK_NO(chat::NO);
                exchange.strSendAnswer = chat::NO;
                tcp.Send(exchange.strSendAnswer);
            }
        }
        // subStringRequest {DELETE, user_email}
        else if (subStringRequest[0] == chat::DELETE) {
            isOk_ = chat::db_delete(subStringRequest);
            if (isOk_) {
                // удаление клиента выполнено успешно
                // strSendAnswer {OK|}
                chat::out_OK_NO(chat::OK);
                exchange.strSendAnswer = chat::OK + "|";
                tcp.Send(exchange.strSendAnswer);
            }
            else {
                chat::out_OK_NO(chat::NO);
                exchange.strSendAnswer = chat::NO;
                tcp.Send(exchange.strSendAnswer);
            }
        }
        // subStringRequest {EDIT, (OLD)user_email, (NEW)user_email, (NEW)password}
        else if (subStringRequest[0] == chat::EDIT) {
            isOk_ = chat::db_edit(subStringRequest);
            if (isOk_) {
                // редактирование данных клиента выполнено успешно
                // strSendAnswer {OK|}
                chat::out_OK_NO(chat::OK);
                exchange.strSendAnswer = chat::OK + "|";
                tcp.Send(exchange.strSendAnswer);
            }
            else {
                chat::out_OK_NO(chat::NO);
                exchange.strSendAnswer = chat::NO;
                tcp.Send(exchange.strSendAnswer);
            }
        }
        // subStringRequest {MESSAGE, user_email}
        else if (subStringRequest[0] == chat::MESSAGE) {
            isOk_ = chat::db_message(subStringRequest, exchange);
            if (isOk_) {
                // редактирование данных клиента выполнено успешно
                // strSendAnswer {mess_date : mess_text|}
                chat::out_OK_NO(chat::OK);
                tcp.Send(exchange.strSendAnswer);
            }
            else {
                chat::out_OK_NO(chat::NO);
                exchange.strSendAnswer = chat::NO;
                tcp.Send(exchange.strSendAnswer);
            }
        }
    } // while (true) {}

    tcp.exit();

    std::cout << std::endl;
    std::cout << color.esc_tb(ptl::Color::GREEN)
              << "chat"
              << color.esc_c()
              << ": Сервер завершает работу...\n"
              << color.esc_tb(ptl::Color::GREEN)
              << "chat"
              << color.esc_c()
              << ": До новых встреч\n";

    ptl::scrs();
    return 0;
}
