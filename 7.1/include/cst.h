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
const std::string FILE_LOGGER {"slog"};

const std::string OK {"OK"};
const std::string NO {"NO"};

const std::string QUIT                  {"QUIT"};
const std::string AUTHORIZATION         {"AUTHORIZATION"};
const std::string REGISTRATION          {"REGISTRATION"};
const std::string DELETE                {"DELETE"};
const std::string EDIT                  {"EDIT"};
const std::string MESSAGE_FROM_DATABASE {"MESSAGE_FROM_DATABASE"};
const std::string MESSAGE_TO_DATABASE   {"MESSAGE_TO_DATABASE"};
const std::string MESSAGE_STATUS        {"MESSAGE_STATUS"};

extern bool t_shouldStop; // Флаг работы/остановки потока.
                          // false - работа поток в штатном режиме,
                          // true  - необходимость остановки работы потока.

} // namespace chat

#endif // __CHAT_CST_H__
