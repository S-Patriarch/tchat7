/*++

    (c) S-Patriarch, 2023
    Определение структор данных.

--*/

#ifndef __CHAT_SCT_H__
#define __CHAT_SCT_H__

#include <string>

////////////////////////////////////////////////////////////////////////////////
namespace chat {

// структура, описывающая текущего пользователя
struct User {
    std::string  s_userID {};     // id пользователя
    std::string  s_userName {};   // имя пользователя
    std::string  s_userFamaly {}; // фамилия пользователя
    std::string  s_userEmail {};  // email пользователя
    unsigned int s_userColor {};  // цвет вывода пользователя
};

// структура, описывающая сообщение
struct Message {
    std::string s_msgFromWhom {}; // от кого сообщение
    std::string s_msgToWhom {};   // кому сообщение
    std::string s_msgText {};     // текст сообщения
};

// структура для обмена данными между сервером и клиентом
struct Exchange {
    std::string strReadRequest {}; // строка запроса
    std::string strSendAnswer {};  // строка ответа
};

} // namespace chat

#endif // __CHAT_SCT_H__
