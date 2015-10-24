#include "Message.h"
#include "Folder.h"

namespace chapter12
{
Folder::Folder(const std::string &name)
    :_name(name)
{

}

Folder::Folder(const Folder &folder)
    : _name(folder._name), _msgSet(folder._msgSet)
{
    addToMessages();
}

Folder& Folder::operator = (const Folder &folder)
{
    removeFromMessages();
    _name = folder._name;
    _msgSet = folder._msgSet;
    addToMessages();

    return *this;
}

Folder::~Folder()
{
    removeFromMessages();
}

void Folder::addMsg(Message *msg)
{
    _msgSet.insert(msg);
}

void Folder::removeMsg(Message *msg)
{
    _msgSet.erase(msg);
}

void Folder::addToMessages()
{
    for (auto item : _msgSet)
    {
        item->_folders.insert(this);
    }
}

void Folder::removeFromMessages()
{
    for (auto item : _msgSet)
    {
        item->_folders.erase(this);
    }
}

}


