/*++

    (c) S-Patriarch, 2023

--*/

#ifndef __CHAT_ADDFUNCT_H__
#define __CHAT_ADDFUNCT_H__

#include "../include/sct.h"

#include "ptl/pcolor.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <sstream>

namespace chat
{
//------------------------------------------------------------------------------
// Функция удаляет последний указанный символ из переданной строки.
//
std::string
remove_last(const std::string&, const char);

//------------------------------------------------------------------------------
// Функция разбивает переданную строку на построки.
//
std::vector<std::string>
parsing_string(const std::string&, const char);

//------------------------------------------------------------------------------
// Вывод сервером информации о завершении работы клиента.
//
void
out_server_quit();

//------------------------------------------------------------------------------
// Вывод информации о завершении работы клиента.
//
void
out_client_quit(User&);

//------------------------------------------------------------------------------
// Вывод короткого сообщения chat: OK или chat: NO.
//
void
out_OK_NO(const std::string&);

} // namespace chat

#endif // __CHAT_ADDFUNCT_H__
