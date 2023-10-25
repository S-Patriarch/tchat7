/*++

    (c) S-Patriarch, 2023
    Определение констант и глобальных переменных.

--*/

#ifndef __CHAT_CST_H__
#define __CHAT_CST_H__

#include <cstdint>
#include <string>

////////////////////////////////////////////////////////////////////////////////
namespace chat
{

const std::uint16_t PORT {53333};
const std::uint16_t SERVER {1000};
const std::uint16_t CLIENT {1001};
const std::uint16_t MAX_PACKET_SIZE {4096};

const std::string IP_ADDRESS_SERVER {"127.0.0.1"};
const std::string GIVE_ME_MESSAGES_FROM_THE_DATABASE {"100"};

const std::string OK            {"OK"};
const std::string QUIT          {"QUIT"};
const std::string AUTHORIZATION {"AUTHORIZATION"};
const std::string REGISTRATION  {"REGISTRATION"};
const std::string DELETE        {"DELETE"};
const std::string EDIT          {"EDIT"};
const std::string MESSAGE       {"MESSAGE"};

} // namespace chat

#endif // __CHAT_CST_H__
