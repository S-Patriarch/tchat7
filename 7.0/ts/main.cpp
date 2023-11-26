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
#include "ptl/plogger.h"

#include <pthread.h>

////////////////////////////////////////////////////////////////////////////////
int main()
{
    ptl::pTCPServer tcp;
    ptl::pColor     color;

    ptl::pLogger logger(chat::FILE_LOGGER);

    chat::Exchange exchange;

    std::string strLogger_ {};

    ptl::clrscr();
    ptl::hcrs();
    chat::get_logo(chat::SERVER);

    //--------------------------------------------------------------------------
    std::cout << "Проверка базы данных... " << std::flush;
    strLogger_ =
        chat::get_date() + " " + chat::get_time() +
        " : Проверка базы данных... ";
    try {
        bool isCheckDB_ = chat::db_existence_check();
        if (!isCheckDB_) {
            chat::db_create();
        }
    }
    catch (std::exception& ex) {
        std::cout << ex.what() << '\n';
        ptl::scrs();
        return 0;
    }
    chat::out_OK_NO(chat::OK);
    strLogger_ += chat::OK;
    logger.write(strLogger_);

    //--------------------------------------------------------------------------
    std::cout << "Очистка базы данных... " << std::flush;
    strLogger_ =
        chat::get_date() + " " + chat::get_time() +
        " : Очистка базы данных... ";
    bool isCheckDB_ = chat::db_message_dalete();
    if (isCheckDB_) {
        chat::out_OK_NO(chat::OK);
        strLogger_ += chat::OK;
    }
    else {
        chat::out_OK_NO(chat::NO);
        strLogger_ += chat::NO;
    }
    logger.write(strLogger_);

    //--------------------------------------------------------------------------
    std::cout << "Подключение клиента... " << std::flush;
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
    chat::out_OK_NO(chat::OK);

    strLogger_ =
        chat::get_date() + " " + chat::get_time() +
        " : Подключение клиента... " + chat::OK;
    logger.write(strLogger_);

    //--------------------------------------------------------------------------
    std::vector<std::string> subStringRequest {};
    bool isOk_ {false};

    while (true) {
        exchange.strReadRequest = tcp.read();
        subStringRequest = chat::parsing_string(exchange.strReadRequest, '|');

        // subStringRequest {QUIT,user_email}
        if (subStringRequest[0] == chat::QUIT) {
            std::cout << "Клиент " + subStringRequest[1] + " покидает чат... ";
            chat::out_OK_NO(chat::OK);

            strLogger_ =
                chat::get_date() + " " + chat::get_time() +
                " : Клиент " + subStringRequest[1] + " покидает чат... " + chat::OK;
            logger.write(strLogger_);

            chat::out_server_quit();
            tcp.exit();
            ptl::scrs();
            return 0;
        }
        // subStringRequest {AUTHORIZATION,user_email,password}
        else if (subStringRequest[0] == chat::AUTHORIZATION) {
            isOk_ = chat::db_authorization(subStringRequest, exchange);
            strLogger_ =
                chat::get_date() + " " + chat::get_time() +
                " : Вход пользователя " + subStringRequest[1] + "... ";

            if (isOk_) {
                // вход клиента выполнен успешно
                // strSendAnswer {id|user_name|user_surname|}
                chat::out_OK_NO(chat::OK);
                strLogger_ += chat::OK;
                logger.write(strLogger_);
                tcp.Send(exchange.strSendAnswer);
            }
            else {
                chat::out_OK_NO(chat::NO);
                strLogger_ += chat::NO;
                logger.write(strLogger_);
                exchange.strSendAnswer = chat::NO;
                tcp.Send(exchange.strSendAnswer);
            }
        }
        // subStringRequest {REGISTRATION,user_name,user_surname,user_email,password}
        else if (subStringRequest[0] == chat::REGISTRATION) {
            isOk_ = chat::db_registration(subStringRequest, exchange);
            strLogger_ =
                chat::get_date() + " " + chat::get_time() +
                " : Регистрация пользователя " + subStringRequest[3] + "... ";

            if (isOk_) {
                // регистрация клиента выполнена успешно
                // strSendAnswer {id|}
                chat::out_OK_NO(chat::OK);
                strLogger_ += chat::OK;
                logger.write(strLogger_);
                tcp.Send(exchange.strSendAnswer);
            }
            else {
                chat::out_OK_NO(chat::NO);
                strLogger_ += chat::NO;
                logger.write(strLogger_);
                exchange.strSendAnswer = chat::NO;
                tcp.Send(exchange.strSendAnswer);
            }
        }
        // subStringRequest {DELETE,id}
        else if (subStringRequest[0] == chat::DELETE) {
            isOk_ = chat::db_delete(subStringRequest);
            strLogger_ =
                chat::get_date() + " " + chat::get_time() +
                " : Удаление пользователя... ";

            if (isOk_) {
                // удаление клиента выполнено успешно
                // strSendAnswer {OK}
                chat::out_OK_NO(chat::OK);
                exchange.strSendAnswer = chat::OK;
                strLogger_ += chat::OK;
                logger.write(strLogger_);
                tcp.Send(exchange.strSendAnswer);
            }
            else {
                chat::out_OK_NO(chat::NO);
                strLogger_ += chat::NO;
                logger.write(strLogger_);
                exchange.strSendAnswer = chat::NO;
                tcp.Send(exchange.strSendAnswer);
            }
        }
        // subStringRequest {EDIT,(OLD)user_email,(NEW)user_email,(NEW)password}
        else if (subStringRequest[0] == chat::EDIT) {
            isOk_ = chat::db_edit(subStringRequest);
            strLogger_ =
                chat::get_date() + " " + chat::get_time() +
                " : Редактирование данных пользователя " +
                subStringRequest[1] + "... ";

            if (isOk_) {
                // редактирование данных клиента выполнено успешно
                // strSendAnswer {OK}
                chat::out_OK_NO(chat::OK);
                exchange.strSendAnswer = chat::OK;
                strLogger_ += chat::OK;
                logger.write(strLogger_);
                tcp.Send(exchange.strSendAnswer);
            }
            else {
                chat::out_OK_NO(chat::NO);
                strLogger_ += chat::NO;
                logger.write(strLogger_);
                exchange.strSendAnswer = chat::NO;
                tcp.Send(exchange.strSendAnswer);
            }
        }
        // subStringRequest {MESSAGE_FROM_DATABASE,id}
        else if (subStringRequest[0] == chat::MESSAGE_FROM_DATABASE) {
            isOk_ = chat::db_message_from_database(subStringRequest, exchange);
            strLogger_ =
                chat::get_date() + " " + chat::get_time() +
                " : Предоставление не прочитанных сообщений... ";

            if (isOk_) {
                // выборка не прочитанных сообщений клиента выполнена успешно
                // strSendAnswer {id_sender : mess_date : mess_text|}
                chat::out_OK_NO(chat::OK);
                strLogger_ += chat::OK;
                logger.write(strLogger_);

                std::vector<std::string> subStrings__ {};
                subStrings__ = chat::parsing_string(exchange.strSendAnswer, '|');
                for (const auto& i : subStrings__) {
                    strLogger_ = "M: " + i;
                    logger.write(strLogger_);
                }

                tcp.Send(exchange.strSendAnswer);
            }
            else {
                chat::out_OK_NO(chat::NO);
                strLogger_ += chat::NO;
                logger.write(strLogger_);
                exchange.strSendAnswer = chat::NO;
                tcp.Send(exchange.strSendAnswer);
            }
        }
        // subStringRequest {MESSAGE_TO_DATABASE,id_sender,id_recipient,msg_text}
        else if (subStringRequest[0] == chat::MESSAGE_TO_DATABASE) {
            isOk_ = chat::db_message_to_database(subStringRequest);
            strLogger_ =
                chat::get_date() + " " + chat::get_time() +
                " : Запись направленного сообщения... ";

            if (isOk_) {
                // запись направленного сообщения клиента выполнена успешно
                // strSendAnswer {OK}
                chat::out_OK_NO(chat::OK);
                strLogger_ += chat::OK;
                logger.write(strLogger_);
                exchange.strSendAnswer = chat::OK;
                tcp.Send(exchange.strSendAnswer);
            }
            else {
                chat::out_OK_NO(chat::NO);
                strLogger_ += chat::NO;
                logger.write(strLogger_);
                exchange.strSendAnswer = chat::NO;
                tcp.Send(exchange.strSendAnswer);
            }
        }
        // subStringRequest {MESSAGE_STATUS,id,msg_read,msg_delivered}
        else if (subStringRequest[0] == chat::MESSAGE_STATUS) {
            isOk_ = chat::db_message_status(subStringRequest);
            strLogger_ =
                chat::get_date() + " " + chat::get_time() +
                " : Изменение статуса сообщений... ";

            if (isOk_) {
                // изменение статуса сообщений клиента выполнена успешно
                // strSendAnswer {OK}
                chat::out_OK_NO(chat::OK);
                strLogger_ += chat::OK;
                logger.write(strLogger_);
                exchange.strSendAnswer = chat::OK;
                tcp.Send(exchange.strSendAnswer);
            }
            else {
                chat::out_OK_NO(chat::NO);
                strLogger_ += chat::NO;
                logger.write(strLogger_);
                exchange.strSendAnswer = chat::NO;
                tcp.Send(exchange.strSendAnswer);
            }
        }
    } // while (true) {}
}
