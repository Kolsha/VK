#ifndef VK_API_H
#define VK_API_H

#include <string>
#include <unordered_map>
#include <vector>

#include "json.hpp"

#include "attachment.h"

namespace VK{

using callback_func = std::string(*)(const std::string &);

using json = nlohmann::json;

/* http params */
using params_map = std::unordered_map<std::string, std::string>;

class Client
{

private:

    static const std::string app_id;
    static const std::string app_secret;
    static const std::string scope;

    static const std::string auth_url;

    std::string a_t;

    std::string captcha_sid;
    std::string captcha_key;
    callback_func captcha_callback;


    std::string get_captcha_key(const std::string captcha_sid){
        if(captcha_callback != nullptr){
            return captcha_callback(captcha_sid);
        }
        return "";
    }

    std::string curl_buffer;
    static const std::string api_url;
protected:
    std::string lang;
    std::string version;
    Attachment::User user;
    bool check_access();
    std::string request(const std::string &url, const std::string &data);
public:



    Client(const std::string version = "5.65", const std::string lang = "en",
           const callback_func callback = nullptr);

    std::string access_token(){
        return a_t;
    }

    bool auth(const std::string &login, const std::string &pass,
              const std::string &access_token = "");

    bool oauth(const callback_func handler);

    json call(const std::string &method, const params_map &params);

    json call(const std::string &method, const std::string &params = "");

    void clear(){
        a_t.clear();
        user.first_name.clear();
        user.last_name.clear();
        user.user_id = 0;
    }

    std::string first_name(){
        return user.first_name;
    }
    std::string last_name(){
        return user.last_name;
    }
    size_t user_id(){
        return user.user_id;
    }

    virtual ~Client(){}

};


/* Network utils
 */
class Utils{
public:
    static std::string data2str(const params_map &data);
    static std::string urlencode(const std::string &url);
    static std::string char2hex(const char dec);
    static int CURL_WRITER(char *data, size_t size, size_t nmemb, std::string *buffer);
};


} // namespace VK




#endif // VK_API_H
