#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <set>
#include "Folder.h"

namespace chapter12
{
class Message
{
    friend class Folder;
    friend void swap(Message &lhs, Message &rhs);

public:
    Message(const std::string &contents = std::string(""));
    Message(const Message &msg);
    Message& operator = (const Message &msg);
    ~Message();

    void save(Folder &folder);
    void remove(Folder &folder);

private:
    void addToFolders(const Message *msg);
    void removeFromFolders();
private:
    std::string _contents;
    std::set<Folder*> _folders;
};

void swap(Message &lhs, Message &rhs);

}


#endif // MESSAGE_H
