#ifndef messages_H
#define messages_H

#include "api.h"

#include "attachment.h"

namespace VK {

static const int chat_offset = 2000000000;

class DialogInfo: public Attachment::DataModel
{
public:
    bool is_chat = false;
    int chat_id;
    std::string body;
    std::string title;

    bool parse(const json &data);


    std::string dump(){
        return "[" + title + " | " + std::to_string(chat_id) + "] " + body;
    }

    virtual ~DialogInfo(){}
};


using vector_dialogs = std::vector< DialogInfo >;

class Messages : public Client
{

public:
    Messages();
    vector_dialogs get_dialogs( const size_t count = 20,
                                const size_t offset = 0);
    vector_dialogs get_all_dialogs(const size_t max_count = 0);

    std::string get_chat_title(const int chat_id);
    std::string get_username(const int user_id);

    template<typename T> std::vector<T> get_attachments(const int chat_id, const size_t count = 0);

    virtual ~Messages(){}
};

}
#endif
