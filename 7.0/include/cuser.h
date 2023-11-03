/*++

    (c) S-Patriarch, 2023

--*/

#ifndef __CHAT_CUSER_H__
#define __CHAT_CUSER_H__

#include "../include/cst.h"
#include "../include/sct.h"
#include "../include/funct.h"

#include "ptl/pcolor.h"
#include "ptl/pTCPClient.h"

#include <iostream>
#include <stdio.h>
#include <string>

////////////////////////////////////////////////////////////////////////////////
namespace chat
{
//------------------------------------------------------------------------------
// Авторизация клиента.
//
bool
authorization(ptl::pTCPClient&, User&, Exchange&);

//------------------------------------------------------------------------------
// Регистрация клиента.
//
bool
registration(ptl::pTCPClient&, User&, Exchange&);

//------------------------------------------------------------------------------
// Удаление клиента.
//
bool
delete_user(ptl::pTCPClient&, User&, Exchange&);

//------------------------------------------------------------------------------
// Вывод из БД непрочитанных сообщений для клиента.
//
bool
out_message(ptl::pTCPClient&, User&, Exchange&);

} // namespace chat

#endif // __CHAT_CUSER_H__
