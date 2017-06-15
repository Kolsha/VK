#include <curl/curl.h>
#include "api.h"

using ::std::string;
using ::std::cout;
using ::std::endl;
using json = ::nlohmann::json;



const string VK::Client::api_url = "https://api.vk.com/method/";
const string VK::Client::app_id = "3140623";// android=2274003
const string VK::Client::app_secret = "VeWdmVclDCtn6ihuP1nt";// android=hHbZxrka2uZ6jB1inYsH
const string VK::Client::scope = "offline,groups,messages,friends,audio";

const string VK::Client::auth_url = "https://oauth.vk.com/token?";



bool VK::Client::oauth(const callback_func handler){
    if(handler == nullptr){
        return false;
    }
    this->clear();
    string oauth_url = "https://oauth.vk.com/authorize?";
    params_map params = {
        {"client_id", app_id},
        {"display", "page"},
        {"redirect_uri", "https://oauth.vk.com/blank.html"},
        {"scope", scope},
        {"response_type", "token"},
        {"v", version},
    };
    oauth_url += Utils::data2str(params);
    string blank = handler(oauth_url);
    if(blank.empty()){
        return false;
    }

    auto it = blank.find("=");
    if(it == string::npos){
        return false;
    }
    it++;
    this->a_t = blank.substr(it);

    it = this->a_t.find("&expires_in");
    if(it == string::npos){
        this->clear();
        return false;
    }
    this->a_t = a_t.substr(0, it);

    return !this->a_t.empty();

}


VK::Client::Client(const string version, const string lang,
                   const callback_func callback){

    this->captcha_callback = callback;
    this->lang = lang;
    this->version = version;
}


bool VK::Client::check_access(){
    json jres = call("users.get", "");
    if(jres.find("error") != jres.end()){
        this->clear();
        return false;
    }
    try{
        json info = jres.at("response").get<json>();
        info = info.begin().value();
        user.parse(info);
    }
    catch(...){
        this->clear();
        return false;
    }

    return true;
}

bool VK::Client::auth(const string &login, const string &pass,
                      const string &access_token){

    if(!access_token.empty()){
        this->a_t = access_token;
        if(check_access()){
            return true;
        }
    }

    this->a_t.clear();

    if(login.empty() || pass.empty()){
        return false;
    }

    string url = auth_url;
    params_map params = {
        {"client_id", app_id},
        {"grant_type", "password"},
        {"client_secret", app_secret},
        {"scope", scope},
        {"username", login},
        {"password", pass},
        {"captcha_sid", captcha_sid},
        {"captcha_key", captcha_key}
    };

    string data = VK::Utils::data2str(params);

    string res = request(url, data);

    if(res.empty()){
        return false;
    }

    captcha_sid.clear();
    captcha_key.clear();

    try{
        cout << res << endl;
        json jres = json::parse(res);
        if(jres.find("error") == jres.end() ||
                jres.find("access_token") != jres.end()){

            this->a_t = jres.at("access_token").get<string>();
            this->user.user_id = jres.at("user_id").get<size_t>();


            return check_access();
        }
        else{
            string error = jres.at("error").get<string>();
            if(error == "invalid_client"){
                return false;
            }
            if(error == "invalid_request"){
                return false;
            }
            if(error == "need_captcha"){
                captcha_sid = jres.at("captcha_sid").get<string>();
                captcha_key = get_captcha_key(captcha_sid);
                if(!captcha_key.empty()){
                    return this->auth(login, pass);
                }
                captcha_sid.clear();
                captcha_key.clear();
            }

        }

        return false;
    }
    catch(...){
        cout << "Catch in auth" << endl;
        return false;
    }

    return false;
}

json VK::Client::call(const string &method, const std::string &params){
    if(method.empty()){
        return nullptr;
    }
    string url = api_url + method;
    string data = params + ( (params.empty()) ? "" : "&");

    params_map tmp_params;

    tmp_params.insert({"captcha_sid", captcha_sid});
    tmp_params.insert({"captcha_key", captcha_key});
    tmp_params.insert({"v", version});
    tmp_params.insert({"lang", lang});
    if(!a_t.empty()){
        tmp_params.insert({"access_token", a_t});
    }

    data += VK::Utils::data2str(tmp_params);
    string res = request(url, data);

    captcha_sid.clear();
    captcha_key.clear();

    try{
        json jres = json::parse(res);

        if(jres.find("error") == jres.end()){
            return jres;
        }
        else{
            json item = jres.at("error").get<json>();
            string error = item.at("error_msg").get<string>();
            if(error == "need_captcha"){
                captcha_sid = item.at("captcha_sid").get<string>();
                captcha_key = get_captcha_key(captcha_sid);
                if(!captcha_key.empty()){
                    return this->call(method, params);
                }
                captcha_sid.clear();
                captcha_key.clear();
            }else{
                return jres;
            }

        }
    }
    catch(...){

    }

    return nullptr;
}

json VK::Client::call(const string &method, const params_map &params){

    if(method.empty()){
        return nullptr;
    }

    string data;
    if(params.size()){
        data = VK::Utils::data2str(params);
    }

    return this->call(method, data);
}









string VK::Utils::char2hex(const char dec){
    char dig1 = (dec & 0xF0) >> 4;
    char dig2 = (dec & 0x0F);

    if (0  <= dig1 && dig1 <=  9) dig1 += 48;
    if (10 <= dig1 && dig1 <= 15) dig1 += 87;
    if (0  <= dig2 && dig2 <=  9) dig2 += 48;
    if (10 <= dig2 && dig2 <= 15) dig2 += 87;

    string r;
    r.append(&dig1, 1);
    r.append(&dig2, 1);
    return r;
}

string VK::Utils::urlencode(const string &url){

    string escaped;
    for(const char& c : url) {
        if ( (48 <= c && c <= 57)  ||
             (65 <= c && c <= 90)  ||
             (97 <= c && c <= 122) ||
             (c =='~' || c =='!' || c =='*' || c =='(' || c ==')' || c =='\'')
             )
            escaped.append( &c, 1);
        else{
            escaped.append("%");
            escaped.append( char2hex(c) );
        }
    }

    return escaped;
}

string VK::Utils::data2str(const params_map &data){
    string result;

    for(auto &kv:data){
        result += kv.first + "=" + urlencode(kv.second)+ "&";
    }

    return result;
}


int VK::Utils::CURL_WRITER(char *data, size_t size, size_t nmemb, string *buffer){
    int result = 0;
    if (buffer != NULL){
        buffer->append(data, size * nmemb);
        result = size * nmemb;
    }

    return result;
}

string VK::Client::request(const string &url, const string &data){
    static char errorBuffer[CURL_ERROR_SIZE];


    curl_buffer.clear();

    CURL *curl;
    CURLcode result;
    curl = curl_easy_init();
    if (curl){
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "VK API Client");
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, VK::Utils::CURL_WRITER);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curl_buffer);
        result = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        if (result == CURLE_OK)
        {
            return curl_buffer;
        }
        else
        {
            return errorBuffer;
        }
    }
    curl_easy_cleanup(curl);

    return "";
}



