#include "Message.h"

namespace chapter12
{
Message::Message(const std::string &contents)
    :_contents(contents)
{

}

Message::Message(const Message &msg)
    : _contents(msg._contents), _folders(msg._folders)
{
    addToFolders(this);
}

Message& Message::operator = (const Message &msg)
{
    removeFromFolders();
    _contents = msg._contents;
    _folders = msg._folders;
    addToFolders(&msg);

    return *this;
}

Message::~Message()
{
    removeFromFolders();
}

void Message::save(Folder &folder)
{
    _folders.insert(&folder);
    folder.addMsg(this);
}

void Message::remove(Folder &folder)
{
    _folders.erase(&folder);
    folder.removeMsg(this);
}

void Message::addToFolders(const Message *msg)
{
    for (auto item : msg->_folders)
    {
        item->addMsg(this);
    }
}

void Message::removeFromFolders()
{
    for (auto item : _folders)
    {
        item->removeMsg(this);
    }
}

void swap(Message &lhs, Message &rhs)
{
    for (auto item : lhs._folders)
    {
        item->removeMsg(&lhs);
    }

    for (auto item : rhs._folders)
    {
        item->removeMsg(&rhs);
    }

    using std::swap;
    swap(lhs._contents, rhs._contents);
    swap(lhs._folders, rhs._folders);

    for (auto item : lhs._folders)
    {
        item->addMsg(&lhs);
    }

    for (auto item : rhs._folders)
    {
        item->addMsg(&rhs);
    }
}

}


