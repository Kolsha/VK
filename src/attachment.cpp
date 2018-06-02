#include "attachment.h"

using std::string;

const string VK::Attachment::Audio::type = "audio";
const string VK::Attachment::Photo::type = "photo";
const string VK::Attachment::Document::type = "doc";

bool VK::Attachment::BaseAttachment::parse_type(const json &data) {
    try {
        this->parsed_type = data.at("type").get<string>();
        return true;
    }
    catch(...) {
        this->parsed_type.clear();
    }
    return false;
}

bool VK::Attachment::BaseAttachment::parse_common(const json &data) {
    try {
        this->date = data.at("date").get<size_t>();
        this->id = data.at("id").get<int>();
        this->owner_id = data.at("owner_id").get<int>();
        return true;
    }
    catch(...) {

    }

    return false;
}

bool VK::Attachment::Audio::parse(const json &data) {
    if(data == nullptr) {
        return false;
    }

    try {

        if(!parse_type(data) || parsed_type != type) {
            return false;
        }

        json att = data.at(type).get<json>();
        if(att == nullptr) {
            return false;
        }
        parse_common(att);
        this->title = att.at("title").get<string>();
        this->artist = att.at("artist").get<string>();
        this->duration = att.at("duration").get<size_t>();
        this->direct_url = att.at("url").get<string>();
        return true;
    }
    catch(...) {

    }

    return false;
}

bool VK::Attachment::Photo::parse(const json &data) {
    std::vector<string> sizes = { "photo_2560", "photo_1280", "photo_807", "photo_604", "photo_130", "photo_75"};
    if(data == nullptr) {
        return false;
    }

    try {

        if(!parse_type(data) || parsed_type != type) {
            return false;
        }

        json att = data.at(type).get<json>();
        if(att == nullptr) {
            return false;
        }
        parse_common(att);
        this->text = att.at("text").get<string>();
        for(auto &size: sizes) {
            if(att.find(size) != att.end()) {
                this->direct_url = att.at(size).get<string>();
                break;
            }

        }
        return true;
    }
    catch(...) {

    }

    return false;
}

std::string VK::Attachment::Document::doc_type_str(const int tp) {
    switch (tp) {
    case 1:
        return "text";
    case 2:
        return "archive";
    case 3:
        return "gif";
    case 4:
        return "image";
    case 5:
        return "audio";
    case 6:
        return "video";
    case 7:
        return "book";
    default:
        return "unknown";
    }
}

bool VK::Attachment::Document::parse(const json &data) {
    if(data == nullptr) {
        return false;
    }

    try {

        if(!parse_type(data) || parsed_type != type) {
            return false;
        }

        json att = data.at(type).get<json>();
        if(att == nullptr) {
            return false;
        }
        parse_common(att);
        this->title = att.at("title").get<string>();
        this->ext = att.at("ext").get<string>();
        this->doc_type = att.at("type").get<int>();
        this->size = att.at("size").get<size_t>();
        this->direct_url = att.at("url").get<string>();
        return true;
    }
    catch(...) {

    }

    return false;
}

bool VK::Attachment::User::parse(const VK::json &data) {
    if(data == nullptr) {
        return false;
    }

    try {

        this->first_name = data.at("first_name").get<string>();
        this->last_name = data.at("last_name").get<string>();
        this->user_id = data.at("id").get<size_t>();
        return true;
    }
    catch(...) {

    }

    return false;

}
