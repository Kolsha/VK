#ifndef VK_ATTACHMENT_H
#define VK_ATTACHMENT_H

#include "json.hpp"

namespace VK {

using json = nlohmann::json;

namespace Attachment {

class DataModel{
protected:
    bool parsed;
public:
    virtual bool parse(const json &data) = 0;
    virtual std::string dump() = 0;
    bool is_parsed(){
        return parsed;
    }
    virtual ~DataModel(){}
};

class User : public DataModel
{
public:

    std::string first_name;
    std::string last_name;
    size_t user_id;
    bool parse(const json &data);
    std::string dump(){
        return std::to_string(user_id) + " - " + first_name + " " + last_name;
    }

};


class BaseAttachment : public DataModel
{
protected:
    std::string parsed_type;
    bool parse_type(const json &data);
    bool parse_common(const json &data);
public:
    int id;
    int owner_id;
    size_t date;
    std::string direct_url;
};

class Audio : public BaseAttachment
{
public:
    static const std::string type;
    std::string artist;
    std::string title;
    size_t duration;
    bool parse(const json &data);
    std::string dump(){
        return artist + " - " + title + " : " + std::to_string(duration);
    }

};

class Photo : public BaseAttachment
{
public:
    static const std::string type;
    std::string text;
    bool parse(const json &data);
    std::string dump(){
        return text + " - " + direct_url + " : " + std::to_string(date);
    }

};

class Document : public BaseAttachment
{
public:
    static const std::string type;
    std::string title;
    std::string ext;
    int doc_type = -1; /*
        Тип документа. Возможные значения:
        1 — текстовые документы;
        2 — архивы;
        3 — gif;
        4 — изображения;
        5 — аудио;
        6 — видео;
        7 — электронные книги;
        8 — неизвестно.
    */
    static std::string doc_type_str(const int tp);
    size_t size = 0; /* in byte */

    bool parse(const json &data);
    std::string dump(){
        return title + " - " + ext + " : " + std::to_string(size);
    }

};

}

}
#endif // VK_ATTACHMENT_H
