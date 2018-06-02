#ifndef VK_API_H
#define VK_API_H

#include <string>
#include <unordered_map>
#include <vector>

#include "json.hpp"

#include "attachment.h"

namespace VK {

using callback_func_cap = std::string(*)(const std::string &);
using callback_func_fa2 = std::string(*)();

using json = nlohmann::json;

/* http params */
using params_map = std::unordered_map<std::string, std::string>;

class Client {

private:

    static const std::string app_id;
    static const std::string app_secret;
    static const std::string scope;
    static const std::string auth_url;
    static const std::string api_url;

    std::string a_t;
    std::string captcha_sid;
    std::string captcha_key;
    std::string fa2_code;

    std::string l_error;

    callback_func_cap captcha_callback = nullptr;
    callback_func_fa2 fa2_callback = nullptr;

    inline std::string get_captcha_key(const std::string &captcha_sid);
    inline std::string get_fa2_code();

    std::string curl_buffer;

protected:
    std::string version;
    std::string lang;
    Attachment::User user;
    bool check_access();
    std::string request(const std::string &url, const std::string &data);

public:

    Client(const std::string _version = "5.65",
           const std::string _lang = "en",
           const callback_func_cap cap_callback = nullptr,
           const callback_func_fa2 _fa2_callback = nullptr);

    bool auth(const std::string &login, const std::string &pass,
              const std::string &access_token = "");

    bool oauth(const callback_func_cap handler);

    json call(const std::string &method, const params_map &params);

    json call(const std::string &method, const std::string &params = "");

    void clear();

    std::string first_name() const;
    std::string last_name() const;
    size_t user_id() const;

    std::string access_token() const;
    std::string last_error() const;

    void set_fa2_callback(const callback_func_fa2 _fa2_callback = nullptr);
    void set_cap_callback(const callback_func_cap cap_callback = nullptr);

    virtual ~Client() {}
};


/* Network utils
 */
class Utils {
public:
    static std::string data2str(const params_map &data);
    static std::string urlencode(const std::string &url);
    static std::string char2hex(const char dec);
    static int CURL_WRITER(char *data, size_t size, size_t nmemb, std::string *buffer);
};


} // namespace VK




#endif // VK_API_H
