#include "modelChator.h"

ModelChator::ModelChator() {}

ModelMessage::ModelMessage() : _idMessage(0), _idRoom(0), _idUser(0), _date(QDateTime::currentDateTime()), _content("") {}

ModelMessage::ModelMessage(const quint32 idMessage, const quint32 idRoom, const quint32 idUser,
                           const QDateTime& date, const QString& content) :
    _idMessage(idMessage), _idRoom(idRoom), _idUser(idUser), _date(date), _content(content) {}

ModelMessage::~ModelMessage(){}

void ModelMessage::modify(const QString& content)
{
    _content = content;
}

QString ModelMessage::getContent() const
{
    return _content;
}

quint32 ModelMessage::getIdMessage() const
{
    return _idMessage;
}

quint32 ModelMessage::getIdUser() const
{
    return _idUser;
}

quint32 ModelMessage::getIdRoom() const
{
    return _idRoom;
}

QDateTime ModelMessage::getDate() const
{
    return _date;
}

void ModelMessage::setIdMessage(const quint32 id)
{
    _idMessage = id;
}

void ModelMessage::setDate(const QDateTime& date)
{
    _date = date;
}

ModelUser::ModelUser() : _idUser(0), _userName(""), _firstName(""), _lastName(""), _isConnected(false), _lastConnection(QDateTime()), _image(QImage()) {}

ModelUser::ModelUser(const quint32 idUser, const QString& userName, const QString& firstName,
                     const QString& lastName, const bool isConnected, const QDateTime& lastConnection,
                     const QImage& image) :
    _idUser(idUser), _userName(userName), _firstName(firstName),_lastName(lastName), _isConnected(isConnected),
    _lastConnection(lastConnection), _image(image) {}
ModelUser::~ModelUser(){};

QDataStream& operator << (QDataStream& ds, const ModelRoom& r)
{
    return ds;
}

QDataStream& operator >> (QDataStream& ds, ModelRoom& r)
{
    return ds;
}

QDataStream& operator << (QDataStream& ds, const ModelMessage& m)
{
    return ds << m._idMessage << m._idUser << m._date << m._content;
}

QDataStream& operator >> (QDataStream& ds, ModelMessage& m)
{
    return ds >> m._idMessage >> m._idUser >> m._date >> m._content;
}

QDataStream& operator << (QDataStream& ds, const ModelUser& u)
{
    return ds << u._userName << u._firstName << u._lastName << u._isConnected << u._lastConnection << u._image;
}

QDataStream& operator >> (QDataStream& ds, ModelUser& u)
{
    return ds >> u._userName >> u._firstName >> u._lastName >> u._isConnected >> u._lastConnection >> u._image;
}

ModelRoom::ModelRoom() : _idRoom(0), _name(""), _private(false), _visible(false), _picture(QImage()), _limitOfStoredMessage(0), _admins(QSet<quint32>()), _members(QSet<quint32>()){}

ModelRoom::ModelRoom(const quint32 idRoom, const QString& name, const quint32 limitOfStoredMessage, const bool isPrivate,
          const bool isVisible, const QImage& picture, const QSet<quint32>& adminsIds, const QSet<quint32>& usersIds, const QMap<quint32, ModelMessage>& messages) :
    _idRoom(idRoom), _name(name), _private(isPrivate), _visible(isVisible),
    _picture(picture), _limitOfStoredMessage(limitOfStoredMessage), _admins(adminsIds), _members(usersIds), _messages(messages) {}

ModelRoom::~ModelRoom(){}

void ModelRoom::addUser(const quint32 idUser)
{
    _members.insert(idUser);
}

void ModelRoom::addAdmin(const quint32 idUser)
{
    _members.insert(idUser);
    _admins.insert(idUser);
}

QSet<quint32> ModelRoom::getUsers() const
{
    return _members;
}

QSet<quint32> ModelRoom::getAdmins() const
{
    return _admins;
}

quint32 ModelRoom::getIdRoom() const
{
    return _idRoom;
}

QMap<quint32, ModelMessage> ModelRoom::getMessages() const
{
    return _messages;
}

ModelRoom& ModelChator::getRoom(const quint32 idRoom)
{
    return _rooms[idRoom];
}

const ModelRoom& ModelChator::getRoom(const quint32 idRoom) const
{
    return _rooms.find(idRoom).value();
}

ModelUser& ModelChator::getUser(const quint32 idUser)
{
    return _users[idUser];
}

const ModelUser& ModelChator::getUser(const quint32 idUser) const
{
    return _users.find(idUser).value();
}

void ModelChator::addUser(const quint32 idUser, const QString& userName, const QString& firstName, const QString& lastName, const bool isConnected, const QDateTime& lastConnection, const QImage& image)
{
    _users.insert(idUser, ModelUser(idUser, userName, firstName, lastName, isConnected, lastConnection, image));
}

void ModelChator::addUser(const ModelUser& user)
{
    _users.insert(user.getIdUser(), ModelUser(user));
}

void ModelChator::addRoom(const quint32 idRoom, const QString &name, const quint32 limitOfStoredMessage, const bool isPrivate, const bool isVisible, const QImage &picture, const QSet<quint32>& admins, const QSet<quint32>& users)
{
    _rooms.insert(idRoom, ModelRoom(idRoom, name, limitOfStoredMessage, isPrivate, isVisible, picture, admins, users));
}

void ModelChator::addRoom(const ModelRoom& room)
{
    _rooms.insert(room.getIdRoom(), ModelRoom(room));
}

void ModelChator::addAdmin(const quint32 idRoom, const quint32 idUser)
{
    _rooms[idRoom].addAdmin(idUser);
}

void ModelChator::modifyRoom(const quint32 idRoom, const QString& name, const quint32 limitOfStoredMessage, const bool isPrivate, const bool isVisible, const QImage& picture)
{
    _rooms[idRoom].modifyRoom(name, limitOfStoredMessage, isPrivate, isVisible, picture);
}

void ModelChator::deleteRoom(const quint32 idRoom)
{
    _rooms.remove(idRoom);
}

ModelMessage& ModelChator::getMessage(const quint32 idRoom, const quint32 idMessage)
{
    return _rooms[idRoom].getMessage(idMessage);
}

void ModelChator::modifyMessage(const quint32 idRoom, const quint32 idMessage, const QString& contents)
{
    _rooms[idRoom].modifyMessage(idMessage, contents);
}

void ModelChator::deleteMessage(const quint32 idRoom, const quint32 idMessage)
{
    _rooms[idRoom].deleteMessage(idMessage);
}

ModelMessage& ModelRoom::getMessage(const quint32 idMessage)
{
    return _messages[idMessage];
}

void ModelRoom::addMessage(const quint32 idMessage, const quint32 idRoom, const quint32 idUser, const QDateTime &date, const QString &content)
{
    _messages.insert(idMessage, ModelMessage(idMessage, idRoom, idUser, date, content));
}

void ModelRoom::addMessage(const ModelMessage& message)
{
    _messages.insert(message.getIdMessage(), ModelMessage(message));
}

void ModelRoom::modifyMessage(const quint32 idMessage, const QString& contents)
{
    _messages[idMessage].modify(contents);
}

void ModelRoom::deleteMessage(const quint32 idMessage)
{
    _messages.remove(idMessage);
}

void ModelRoom::modifyRoom(const QString& name, const quint32 limitOfStoredMessage, const bool isPrivate, const bool isVisible, const QImage& picture)
{
    _name = name;
    _limitOfStoredMessage = limitOfStoredMessage;
    _private = isPrivate;
    _visible = isVisible;
    _picture = picture;
}

QList<quint32> ModelChator::getUserRooms(const quint32 idUser) const
{
    QList<quint32> userRooms;
    
    for (quint32 room : _users[idUser].getRooms())
    {
        userRooms.append(room);
    }

    return userRooms;
}

void ModelUser::modify(const QString& firstName, const QString& lastName, const QImage& image)
{
    _firstName = firstName;
    _lastName = lastName;
    _image = image;
}

quint32 ModelUser::getIdUser() const
{
    return _idUser;
}

QString ModelUser::getUserName() const
{
    return _userName;
}

bool ModelUser::isConnected() const
{
    return _isConnected;
}

QSet<quint32>& ModelUser::getRooms()
{
    return _roomsIds;
}

const QSet<quint32>& ModelUser::getRooms() const
{
    return _roomsIds;
}

void ModelUser::setIdUser(const quint32 id)
{
    _idUser = id;
}

void ModelUser::setConnected(const bool connected)
{
    _isConnected = connected;
}

QString ModelRoom::getName() const 
{ 
    return _name;
}

QImage ModelRoom::getPicture() const
{
    return _picture;
}

quint32 ModelRoom::getLimit() const
{
    return _limitOfStoredMessage;
}

bool ModelRoom::isPrivate() const
{
    return _private;
}
bool ModelRoom::isVisible() const
{
    return _visible;
}
