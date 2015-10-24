#ifndef FOLDER_H
#define FOLDER_H

#include <set>
#include <string>

namespace chapter12
{
class Message;

class Folder
{
    friend class Message;

public:
    Folder(const std::string &name = std::string(""));
    Folder(const Folder &folder);
    Folder& operator = (const Folder &folder);
    ~Folder();
    void addMsg(Message *msg);
    void removeMsg(Message *msg);

private:
    void addToMessages();
    void removeFromMessages();

private:
    std::string _name;
    std::set<Message*> _msgSet;
};

}



#endif // FOLDER_H
