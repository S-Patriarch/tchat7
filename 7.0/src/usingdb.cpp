/*++

    (c) S-Patriarch, 2023

--*/

#include "../include/usingdb.h"

////////////////////////////////////////////////////////////////////////////////
namespace chat
{
//------------------------------------------------------------------------------
bool
db_existence_check()
{
    MYSQL      mysql;
    MYSQL_RES* res;
    MYSQL_ROW  row;

    bool isResultReturn_ {false};

    mysql_init(&mysql);

    if (mysql_real_connect(
        &mysql,
        "127.0.0.1", "root", "ZZzz1122+", NULL, 0, NULL, 0)) {

        mysql_query(
            &mysql,
            "SHOW DATABASES LIKE 'tchatdb'");

        res = mysql_store_result(&mysql);
        row = mysql_fetch_row(res);

        if (row != 0) {
            isResultReturn_ = true;
        }

        mysql_free_result(res);
    }
    else {
        std::cout << "E: " << mysql_error(&mysql) << '\n';
    }

    mysql_close(&mysql);
    return isResultReturn_;
}

//------------------------------------------------------------------------------
void
db_create()
{
    MYSQL mysql;

    mysql_init(&mysql);

    if (mysql_real_connect(
        &mysql,
        "127.0.0.1", "root", "ZZzz1122+", NULL, 0, NULL, 0)) {

        mysql_query(
            &mysql,
            "CREATE DATABASE IF NOT EXISTS tchatdb");

        mysql_query(&mysql, "USE tchatdb");

        mysql_set_character_set(&mysql, "utf8");

        mysql_query(
            &mysql,
            "CREATE TABLE user ("
            "    id INT AUTO_INCREMENT PRIMARY KEY,"
            "    user_name CHAR(255),"
            "    user_surname CHAR(255),"
            "    user_email CHAR(255)"
            ") DEFAULT CHARACTER SET utf8");

        mysql_query(
            &mysql,
            "CREATE TABLE shadow ("
            "    id_user INT,"
            "    password_hash CHAR(40),"
            "    FOREIGN KEY (id_user) REFERENCES user(id) ON DELETE CASCADE"
            ") DEFAULT CHARACTER SET utf8");

        mysql_query(
            &mysql,
            "CREATE TABLE message ("
            "    id INT AUTO_INCREMENT PRIMARY KEY,"
            "    id_sender INT,"
            "    id_recipient INT,"
            "    mess_text TEXT,"
            "    mess_date DATE,"
            "    mess_read INT,"
            "    mess_delivered INT"
            ") DEFAULT CHARACTER SET utf8");
    }
    else {
        std::cout << "E: " << mysql_error(&mysql) << '\n';
    }

    mysql_close(&mysql);
}

//------------------------------------------------------------------------------
// v {AUTHORIZATION, user_email, password}
//
bool
db_authorization(std::vector<std::string>& v,
                 Exchange& exchange)
{
    MYSQL      mysql;
    MYSQL_RES* res;
    MYSQL_ROW  row;

    ptl::pColor color;
    ptl::pSha   sha;

    bool isResultReturn_ {false};
    std::string queryString_ {};
    std::string idUser_ {};
    std::string userName_ {};
    std::string userFamaly_ {};

    std::cout << color.esc_tb(ptl::Color::GREEN)
              << "chat"
              << color.esc_c()
              << ": Вход пользователя "
              << v[1]
              << "...\n";

    std::string iHash_ = sha.sha1(v[2].c_str(), sizeof(v[2].c_str()) - 1);

    mysql_init(&mysql);

    if (mysql_real_connect(
        &mysql,
        "127.0.0.1", "root", "ZZzz1122+", "tchatdb", 0, NULL, 0)) {

        mysql_set_character_set(&mysql, "utf8");

        queryString_ =
            "SELECT * FROM user "
            "WHERE user_email = \'" + v[1] + "\'";

        mysql_query(&mysql, queryString_.c_str());

        res = mysql_store_result(&mysql);
        row = mysql_fetch_row(res);

        if (row != NULL) {
            if (v[1] == row[3]) {
                idUser_   = row[0];
                userName_ = row[1];

                if (row[2] != NULL) {
                    userFamaly_ = row[2];
                }

                mysql_free_result(res);

                queryString_ =
                    "SELECT * FROM shadow "
                    "WHERE id_user = " + idUser_;

                mysql_query(&mysql, queryString_.c_str());

                res = mysql_store_result(&mysql);
                row = mysql_fetch_row(res);

                if (row[1] == iHash_) {
                    exchange.strSendAnswer =
                        (idUser_ + "|" + userName_ + "|" + userFamaly_ + "|");
                    isResultReturn_ = true;
                }

                mysql_free_result(res);
            }
        }
    }
    else {
        std::cout << "E: " << mysql_error(&mysql) << '\n';
    }

    mysql_close(&mysql);
    return isResultReturn_;
}

//------------------------------------------------------------------------------
// v {REGISTRATION, user_name, user_surname, user_email, password}
//
bool
db_registration(std::vector<std::string>& v,
                Exchange& exchange)
{
    MYSQL      mysql;
    MYSQL_RES* res;
    MYSQL_ROW  row;

    ptl::pColor color;
    ptl::pSha   sha;

    bool isResultReturn_ {false};
    std::string queryString_ {};

    std::cout << color.esc_tb(ptl::Color::GREEN)
              << "chat"
              << color.esc_c()
              << ": Регистрация пользователя "
              << v[3]
              << "...\n";

    std::string iHash_ = sha.sha1(v[4].c_str(), sizeof(v[4].c_str()) - 1);

    mysql_init(&mysql);

    if (mysql_real_connect(
        &mysql,
        "127.0.0.1", "root", "ZZzz1122+", "tchatdb", 0, NULL, 0)) {

        mysql_set_character_set(&mysql, "utf8");

        queryString_ =
            "INSERT INTO user ("
            "    id,"
            "    user_name,"
            "    user_surname,"
            "    user_email)"
            "VALUES"
            "    (default, \'" + v[1] + "\', \'" + v[2] + "\', \'" + v[3] + "\')";

        mysql_query(&mysql, queryString_.c_str());

        mysql_query(
            &mysql,
            "SET @new_id = LAST_INSERT_ID()");

        queryString_ =
            "INSERT INTO shadow ("
            "    id_user,"
            "    password_hash)"
            "VALUES"
            "    (@new_id, \'" + iHash_ + "\')";

        mysql_query(&mysql, queryString_.c_str());

        queryString_ =
            "SELECT * FROM user "
            "WHERE user_email = \'" + v[3] + "\'";

        mysql_query(&mysql, queryString_.c_str());

        res = mysql_store_result(&mysql);
        row = mysql_fetch_row(res);
        mysql_free_result(res);

        if (row != NULL) {
            if (v[3] == row[3]) {
                std::string idUser_ = row[0];
                exchange.strSendAnswer =
                   (idUser_ + "|" + v[1] + "|" + v[2] + "|");
                isResultReturn_ = true;
            }
        }
    }
    else {
        std::cout << "E: " << mysql_error(&mysql) << '\n';
    }

    mysql_close(&mysql);
    return isResultReturn_;
}

//------------------------------------------------------------------------------
// v {DELETE, user_email}
//
bool
db_delete(std::vector<std::string>& v)
{
    MYSQL mysql;

    ptl::pColor color;

    bool isResultReturn_ {false};

    std::cout << color.esc_tb(ptl::Color::GREEN)
              << "chat"
              << color.esc_c()
              << ": Удаление пользователя "
              << v[1]
              << "...\n";

    mysql_init(&mysql);

    if (mysql_real_connect(
        &mysql,
        "127.0.0.1", "root", "ZZzz1122+", "tchatdb", 0, NULL, 0)) {

        mysql_set_character_set(&mysql, "utf8");

        std::string queryString_ =
            "DELETE FROM user "
            "WHERE user_email = \'" + v[1] + "\'";

        std::int32_t queryState_ = mysql_query(&mysql, queryString_.c_str());
        if (queryState_) {
            std::cout << "E: " << mysql_error(&mysql) << '\n';
            mysql_close(&mysql);
            return isResultReturn_;
        }

        isResultReturn_ = true;
    }
    else {
        std::cout << "E: " << mysql_error(&mysql) << '\n';
    }

    mysql_close(&mysql);
    return isResultReturn_;
}

//------------------------------------------------------------------------------
// v {EDIT, (OLD)user_email, (NEW)user_email, (NEW)password}
//
bool
db_edit(std::vector<std::string>& v)
{
    MYSQL      mysql;
    MYSQL_RES* res;
    MYSQL_ROW  row;

    ptl::pColor color;
    ptl::pSha   sha;

    bool isResultReturn_ {false};

    std::string queryString_ {};
    std::int32_t queryState_ {};
    std::string idUser_ {};

    std::cout << color.esc_tb(ptl::Color::GREEN)
              << "chat"
              << color.esc_c()
              << ": Редактирование данных пользователя "
              << v[1]
              << "...\n";

    std::string iHash_ = sha.sha1(v[2].c_str(), sizeof(v[2].c_str()) - 1);

    mysql_init(&mysql);

    if (mysql_real_connect(
        &mysql,
        "127.0.0.1", "root", "ZZzz1122+", "tchatdb", 0, NULL, 0)) {

        mysql_set_character_set(&mysql, "utf8");

        queryString_ =
            "SELECT * FROM user "
            "WHERE user_email = \'" + v[1] + "\'";

        queryState_ = mysql_query(&mysql, queryString_.c_str());
        if (queryState_) {
            std::cout << "E: " << mysql_error(&mysql) << '\n';
            mysql_close(&mysql);
            return isResultReturn_;
        }

        res = mysql_store_result(&mysql);
        row = mysql_fetch_row(res);
        mysql_free_result(res);

        if (row != NULL) {
            if (v[1] == row[3]) {
                idUser_ = row[0];
            }
        }

        queryString_ =
            "UPDATE user "
            "SET user_email = \'" + v[2] + "\' " +
            "WHERE user_email = \'" + v[1] + "\'";

        queryState_ = mysql_query(&mysql, queryString_.c_str());
        if (queryState_) {
            std::cout << "E: " << mysql_error(&mysql) << '\n';
            mysql_close(&mysql);
            return isResultReturn_;
        }

        queryString_ =
            "UPDATE shadow "
            "SET password_hash = \'" + iHash_ + "\' " +
            "WHERE id_user = " + idUser_;

        queryState_ = mysql_query(&mysql, queryString_.c_str());
        if (queryState_) {
            std::cout << "E: " << mysql_error(&mysql) << '\n';
            mysql_close(&mysql);
            return isResultReturn_;
        }

        isResultReturn_ = true;
    }
    else {
        std::cout << "E: " << mysql_error(&mysql) << '\n';
    }

    mysql_close(&mysql);
    return isResultReturn_;
}

//------------------------------------------------------------------------------
// v {MESSAGE, user_email}
//
bool
db_message(std::vector<std::string>& v,
           Exchange& exchange)
{
    MYSQL      mysql;
    MYSQL_RES* res;
    MYSQL_ROW  row;

    ptl::pColor color;

    bool isResultReturn_ {false};

    std::string queryString_ {};
    std::int32_t queryState_ {};
    std::string idUser_ {};

    std::cout << color.esc_tb(ptl::Color::GREEN)
              << "chat"
              << color.esc_c()
              << ": Предоставление не прочитанных сообщений пользователю "
              << v[1]
              << "...\n";

    mysql_init(&mysql);

    if (mysql_real_connect(
        &mysql,
        "127.0.0.1", "root", "ZZzz1122+", "tchatdb", 0, NULL, 0)) {

        mysql_set_character_set(&mysql, "utf8");

        queryString_ =
            "SELECT * FROM user "
            "WHERE user_email = \'" + v[1] + "\'";

        queryState_ = mysql_query(&mysql, queryString_.c_str());
        if (queryState_) {
            std::cout << "E: " << mysql_error(&mysql) << '\n';
            mysql_close(&mysql);
            return isResultReturn_;
        }

        res = mysql_store_result(&mysql);
        row = mysql_fetch_row(res);
        mysql_free_result(res);

        if (row != NULL) {
            if (v[1] == row[3]) {
                idUser_ = row[0];
            }
        }

        queryString_ =
                "SELECT * FROM message "
                "WHERE id_recipient = " + idUser_;

        queryState_ = mysql_query(&mysql, queryString_.c_str());
        if (queryState_) {
            std::cout << "E: " << mysql_error(&mysql) << '\n';
            mysql_close(&mysql);
            return isResultReturn_;
        }

        std::string str1_ {};
        std::string str2_ {};

        if (res = mysql_store_result(&mysql)) {
            while (row = mysql_fetch_row(res)) {
                str1_ = row[4];
                str2_ = row[3];
                exchange.strSendAnswer = (str1_ + " : " + str2_ + "|");
                isResultReturn_ = true;
            }

            mysql_free_result(res);
        }
    }
    else {
        std::cout << "E: " << mysql_error(&mysql) << '\n';
    }

    mysql_close(&mysql);
    return isResultReturn_;
}

} // namespace chat
