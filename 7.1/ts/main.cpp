/*++

    (c) S-Patriarch, 2023

    Терминальный чат.
    Версия 7.1 SERVER
    
--*/

#include "../include/sct.h"
#include "../include/cst.h"
#include "../include/info.h"
#include "../include/funct.h"
#include "../include/usingdb.h"

#include "ptl/pconio.h"
#include "ptl/pTCPServer.h"

#include <thread>
#include <functional>

//------------------------------------------------------------------------------
// Инициализируем флаг остановки потока в default-ое состояние.
bool chat::t_shouldStop=false;

////////////////////////////////////////////////////////////////////////////////
int main()
{
    ptl::pTCPServer tcp;
    ptl::pColor color;

    chat::Exchange exchange;
    chat::Logger logger;

    ptl::clrscr();
    ptl::hcrs();
    chat::get_logo(chat::SERVER);

    //--------------------------------------------------------------------------
    // Создаем поток, который асинхронно основному потоку будет отписывать
    // информацию в log-файл (chat::FILE_LOGGER). 
    // При этом, захватываем экземпляр структуры chat::Logger.
    //
    std::thread t_log([&logger]() 
    {
        ptl::pLogger log(chat::FILE_LOGGER);

        while(true)
        {
            // Проверяем на необходимость остановки потока.
            if(chat::t_shouldStop)
            {
                break;
            }
            else
            {
                // Проверяем на необходимость записи информации в log-файл.
                if(logger.s_isWriteLogger)
                { 
                    // Отписываем информацию в log-файл.
                    log.write(logger.s_strLogger);
                    // Сбрасываем в default-ое состояние компоненты структуры 
                    // chat::Logger.
                    logger.s_isWriteLogger=false;
                    logger.s_strLogger="";
                }
            }
        }
    });

    //--------------------------------------------------------------------------
    // Проверка базы данных на наличие.
    // При отсутствие, база данных создается.
    //
    std::cout << "Проверка базы данных... " << std::flush;
    logger.s_strLogger=
        chat::get_date()+" "+chat::get_time()+
        " : Проверка базы данных... ";
    try 
    {
        bool isCheckDB_ = chat::db_existence_check();
        if (!isCheckDB_) 
        {
            chat::db_create();
        }
    }
    catch (std::exception& ex) 
    {
        std::cout << ex.what() << '\n';
        ptl::scrs();
        chat::t_shouldStop=true; // остановить поток
        t_log.join();
        return 0;
    }
    chat::out_OK_NO(chat::OK);
    logger.s_strLogger+=chat::OK;
    logger.s_isWriteLogger=true;

    //--------------------------------------------------------------------------
    // Очистка базы данных от сообщений со статусами доставлен/прочитано.
    //
    std::cout 
        << "Очистка базы данных... " 
        << std::flush;
    logger.s_strLogger=
        chat::get_date()+" "+chat::get_time()+
        " : Очистка базы данных... ";
    bool isCheckDB_ = chat::db_message_dalete();
    if(isCheckDB_) 
    {
        chat::out_OK_NO(chat::OK);
        logger.s_strLogger+=chat::OK;
    }
    else 
    {
        chat::out_OK_NO(chat::NO);
        logger.s_strLogger+=chat::NO;
    }
    logger.s_isWriteLogger=true;

    //--------------------------------------------------------------------------
    // Подключение клиента.
    //
    std::cout 
        << "Подключение клиента... " 
        << std::flush;
    try 
    {
        // SERVER
        // создание сокета, привязка сокета,
        // постановка сервера на прием данных
        tcp.setup(chat::PORT);
    }
    catch (std::exception& ex) 
    {
        std::cout << ex.what() << '\n';
        ptl::scrs();
        chat::t_shouldStop=true; // остановить поток
        t_log.join();
        return 0;
    }
    chat::out_OK_NO(chat::OK);

    logger.s_strLogger=
        chat::get_date()+" "+chat::get_time()+
        " : Подключение клиента... "+chat::OK;
    logger.s_isWriteLogger=true;

    //--------------------------------------------------------------------------
    // Цикл обработки клиентских запросов.
    //
    std::vector<std::string> subStringRequest {};
    bool isOk_ {false};

    while (true) {
        exchange.strReadRequest = tcp.read();
        subStringRequest = chat::parsing_string(exchange.strReadRequest, '|');

        // subStringRequest {QUIT,user_email}
        if (subStringRequest[0] == chat::QUIT) {
            std::cout << "Клиент " + subStringRequest[1] + " покидает чат... ";
            chat::out_OK_NO(chat::OK);

            logger.s_strLogger=
                chat::get_date()+" "+chat::get_time()+
                " : Клиент "+subStringRequest[1]+" покидает чат... "+ chat::OK;
            logger.s_isWriteLogger=true;

            chat::out_server_quit();
            tcp.exit();
            ptl::scrs();
            chat::t_shouldStop=true; // остановить поток
            t_log.join();
            return 0;
        }
        // subStringRequest {AUTHORIZATION,user_email,password}
        else if (subStringRequest[0] == chat::AUTHORIZATION) {
            isOk_ = chat::db_authorization(subStringRequest, exchange);
            logger.s_strLogger=
                chat::get_date()+" "+chat::get_time()+
                " : Вход пользователя "+subStringRequest[1]+"... ";

            if (isOk_) {
                // вход клиента выполнен успешно
                // strSendAnswer {id|user_name|user_surname|}
                chat::out_OK_NO(chat::OK);
                logger.s_strLogger+=chat::OK;
                logger.s_isWriteLogger=true;
                tcp.Send(exchange.strSendAnswer);
            }
            else {
                chat::out_OK_NO(chat::NO);
                logger.s_strLogger+=chat::NO;
                logger.s_isWriteLogger=true;
                exchange.strSendAnswer = chat::NO;
                tcp.Send(exchange.strSendAnswer);
            }
        }
        // subStringRequest {REGISTRATION,user_name,user_surname,user_email,password}
        else if (subStringRequest[0] == chat::REGISTRATION) {
            isOk_ = chat::db_registration(subStringRequest, exchange);
            logger.s_strLogger=
                chat::get_date()+" "+chat::get_time()+
                " : Регистрация пользователя "+subStringRequest[3]+"... ";

            if (isOk_) {
                // регистрация клиента выполнена успешно
                // strSendAnswer {id|}
                chat::out_OK_NO(chat::OK);
                logger.s_strLogger+=chat::OK;
                logger.s_isWriteLogger=true;
                tcp.Send(exchange.strSendAnswer);
            }
            else {
                chat::out_OK_NO(chat::NO);
                logger.s_strLogger+=chat::NO;
                logger.s_isWriteLogger=true;
                exchange.strSendAnswer = chat::NO;
                tcp.Send(exchange.strSendAnswer);
            }
        }
        // subStringRequest {DELETE,id}
        else if (subStringRequest[0] == chat::DELETE) {
            isOk_ = chat::db_delete(subStringRequest);
            logger.s_strLogger=
                chat::get_date()+" "+chat::get_time()+
                " : Удаление пользователя... ";

            if (isOk_) {
                // удаление клиента выполнено успешно
                // strSendAnswer {OK}
                chat::out_OK_NO(chat::OK);
                exchange.strSendAnswer = chat::OK;
                logger.s_strLogger+=chat::OK;
                logger.s_isWriteLogger=true;
                tcp.Send(exchange.strSendAnswer);
            }
            else {
                chat::out_OK_NO(chat::NO);
                logger.s_strLogger+=chat::NO;
                logger.s_isWriteLogger=true;
                exchange.strSendAnswer = chat::NO;
                tcp.Send(exchange.strSendAnswer);
            }
        }
        // subStringRequest {EDIT,(OLD)user_email,(NEW)user_email,(NEW)password}
        else if (subStringRequest[0] == chat::EDIT) {
            isOk_ = chat::db_edit(subStringRequest);
            logger.s_strLogger=
                chat::get_date()+" "+chat::get_time()+
                " : Редактирование данных пользователя "+
                subStringRequest[1]+"... ";

            if (isOk_) {
                // редактирование данных клиента выполнено успешно
                // strSendAnswer {OK}
                chat::out_OK_NO(chat::OK);
                exchange.strSendAnswer = chat::OK;
                logger.s_strLogger+=chat::OK;
                logger.s_isWriteLogger=true;
                tcp.Send(exchange.strSendAnswer);
            }
            else {
                chat::out_OK_NO(chat::NO);
                logger.s_strLogger+=chat::NO;
                logger.s_isWriteLogger=true;
                exchange.strSendAnswer = chat::NO;
                tcp.Send(exchange.strSendAnswer);
            }
        }
        // subStringRequest {MESSAGE_FROM_DATABASE,id}
        else if (subStringRequest[0] == chat::MESSAGE_FROM_DATABASE) {
            isOk_ = chat::db_message_from_database(subStringRequest, exchange);
            if (isOk_) {
                tcp.Send(exchange.strSendAnswer);
            }
            else {
                exchange.strSendAnswer = chat::NO;
                tcp.Send(exchange.strSendAnswer);
            }
        }
        // subStringRequest {MESSAGE_TO_DATABASE,id_sender,id_recipient,msg_text}
        else if (subStringRequest[0] == chat::MESSAGE_TO_DATABASE) {
            isOk_ = chat::db_message_to_database(subStringRequest);
            logger.s_strLogger=
                chat::get_date()+" "+chat::get_time()+
                " : Запись направленного сообщения... ";

            if (isOk_) {
                // запись направленного сообщения клиента выполнена успешно
                // strSendAnswer {OK}
                chat::out_OK_NO(chat::OK);
                logger.s_strLogger+=chat::OK;
                logger.s_isWriteLogger=true;
                exchange.strSendAnswer = chat::OK;
                tcp.Send(exchange.strSendAnswer);
            }
            else {
                chat::out_OK_NO(chat::NO);
                logger.s_strLogger+=chat::NO;
                logger.s_isWriteLogger=true;
                exchange.strSendAnswer = chat::NO;
                tcp.Send(exchange.strSendAnswer);
            }
        }
        // subStringRequest {MESSAGE_STATUS,id,msg_read,msg_delivered}
        else if (subStringRequest[0] == chat::MESSAGE_STATUS) {
            isOk_ = chat::db_message_status(subStringRequest);
            if (isOk_) {
                exchange.strSendAnswer = chat::OK;
                tcp.Send(exchange.strSendAnswer);
            }
            else {
                exchange.strSendAnswer = chat::NO;
                tcp.Send(exchange.strSendAnswer);
            }
        }
    } // while (true)
}
