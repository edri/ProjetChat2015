/*
     * Created by Miguel Santamaria
     *
     * Implements modelChator.h
*/

#include "modelChator.h"
#include <iostream>

ModelChator::ModelChator() : _nbRequests(0) {}

void ModelChator::setRsaKeyPair(const RSAPair& rsaKeyPair)
{
    _rsaKeyPair = rsaKeyPair;
}

RSAPair ModelChator::getRsaKeyPair() const
{
    return _rsaKeyPair;
}

ModelMessage::ModelMessage() : _idMessage(0), _idRoom(0), _idUser(0), _date(QDateTime::currentDateTime()), _lastEditionDate(QDateTime::currentDateTime()), _content("") {}

ModelMessage::ModelMessage(const quint32 idMessage, const quint32 idRoom, const quint32 idUser,
                           const QDateTime& date, const QDateTime& lastEditionDate, const QByteArray& content) :
    _idMessage(idMessage), _idRoom(idRoom), _idUser(idUser), _date(date), _lastEditionDate(lastEditionDate), _content(content) {}

ModelMessage::~ModelMessage(){}

void ModelMessage::modify(const QByteArray& content, const QDateTime lastEditionDate)
{
    _content = content;
    _lastEditionDate = lastEditionDate;
}

const QByteArray& ModelMessage::getContent() const
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

QDateTime& ModelMessage::getDate()
{
    return _date;
}

QDateTime& ModelMessage::getEditionDate()
{
    return _lastEditionDate;
}

void ModelMessage::setIdMessage(const quint32 id)
{
    _idMessage = id;
}

void ModelMessage::setDate(const QDateTime& date)
{
    _date = date;
}

void ModelMessage::setEditionDate(const QDateTime& date)
{
    _lastEditionDate = date;
}

void ModelMessage::setContent(const QByteArray& content)
{
    _content = content;
}

ModelUser::ModelUser() : _idUser(0), _userName(""), _firstName(""), _lastName(""), _isConnected(false), _lastConnection(QDateTime()), _image(QImage()) {}

ModelUser::ModelUser(const quint32 idUser, const QString& userName, const QString& firstName,
                     const QString& lastName, const bool isConnected, const QDateTime& lastConnection,
                     const QImage& image, const QSet<quint32> roomsIds) :
    _idUser(idUser), _userName(userName), _firstName(firstName),_lastName(lastName), _isConnected(isConnected),
    _lastConnection(lastConnection), _image(image), _roomsIds(roomsIds) {}
ModelUser::~ModelUser(){};

QDataStream& operator << (QDataStream& ds, const ModelRoom& r)
{
    return ds << r._idRoom << r._name << r._private << r._visible << r._picture << r._limitOfStoredMessage << r._admins << r._members << r._messages << r._secretKey;
}

QDataStream& operator >> (QDataStream& ds, ModelRoom& r)
{
    return ds >> r._idRoom >> r._name >> r._private >> r._visible >> r._picture >> r._limitOfStoredMessage >> r._admins >> r._members >> r._messages >> r._secretKey;
}

QDataStream& operator << (QDataStream& ds, const ModelMessage& m)
{
    return ds << m._idMessage << m._idRoom << m._idUser << m._date << m._lastEditionDate << m._content;
}

QDataStream& operator >> (QDataStream& ds, ModelMessage& m)
{
    return ds >> m._idMessage >> m._idRoom >> m._idUser >> m._date >> m._lastEditionDate >> m._content;
}

QDataStream& operator << (QDataStream& ds, const ModelUser& u)
{
    return ds << u._idUser << u._userName << u._firstName << u._lastName << u._isConnected << u._lastConnection << u._image << u._roomsIds;
}

QDataStream& operator >> (QDataStream& ds, ModelUser& u)
{
    return ds >> u._idUser >> u._userName >> u._firstName >> u._lastName >> u._isConnected >> u._lastConnection >> u._image >> u._roomsIds;
}

ModelRoom::ModelRoom() : _idRoom(0), _name(""), _private(false), _visible(false), _picture(QImage()), _limitOfStoredMessage(0), _admins(QSet<quint32>()), _members(QSet<quint32>()), _secretKey(AESKey()){}

ModelRoom::ModelRoom(const quint32 idRoom, const QString& name, const quint32 limitOfStoredMessage, const bool isPrivate,
          const bool isVisible, const QImage& picture, const QSet<quint32>& adminsIds, const QSet<quint32>& usersIds, const QMap<quint32, ModelMessage>& messages, const AESKey& secretKey) :
    _idRoom(idRoom), _name(name), _private(isPrivate), _visible(isVisible),
    _picture(picture), _limitOfStoredMessage(limitOfStoredMessage), _admins(adminsIds), _members(usersIds), _messages(messages), _secretKey(secretKey) {}

ModelRoom::~ModelRoom(){}

void ModelRoom::addUser(const quint32 idUser)
{
    _members.insert(idUser);
}

void ModelRoom::removeUser(const quint32 userId)
{
    _members.remove(userId);
}

void ModelRoom::addAdmin(const quint32 idUser)
{
    _members.insert(idUser);
    _admins.insert(idUser);
}

const QSet<quint32>& ModelRoom::getUsers() const
{
    return _members;
}

const QSet<quint32>& ModelRoom::getAdmins() const
{
    return _admins;
}

quint32 ModelRoom::getIdRoom() const
{
    return _idRoom;
}

QMap<quint32, ModelMessage>& ModelRoom::getMessages()
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

bool ModelChator::containsRoom(const quint32 idRoom) const
{
    return _rooms.contains(idRoom);
}

ModelUser& ModelChator::getUser(const quint32 idUser)
{
    return _users[idUser];
}

const ModelUser& ModelChator::getUser(const quint32 idUser) const
{
    return _users.find(idUser).value();
}

void ModelChator::addUser(const quint32 idUser, const QString& userName, const QString& firstName, const QString& lastName, const bool isConnected, const QDateTime& lastConnection, const QImage& image, const QSet<quint32> roomsIds)
{
    _users.insert(idUser, ModelUser(idUser, userName, firstName, lastName, isConnected, lastConnection, image, roomsIds));
}

void ModelChator::addUser(const ModelUser& user)
{
    _users.insert(user.getIdUser(), ModelUser(user));
}

void ModelChator::addRoom(const quint32 idRoom, const QString &name, const quint32 limitOfStoredMessage, const bool isPrivate, const bool isVisible, const QImage &picture, const QSet<quint32>& admins, const QSet<quint32>& users, const QMap<quint32, ModelMessage>& messages, const AESKey& secretKey)
{
    _rooms.insert(idRoom, ModelRoom(idRoom, name, limitOfStoredMessage, isPrivate, isVisible, picture, admins, users, messages, secretKey));
}

void ModelChator::addRoom(const ModelRoom& room)
{
    _rooms.insert(room.getIdRoom(), ModelRoom(room));
}

void ModelChator::addAdmin(const quint32 idRoom, const quint32 idUser)
{
    _rooms[idRoom].addAdmin(idUser);
}

void ModelChator::modifyRoom(const quint32 idRoom, const QString& name, const quint32 limitOfStoredMessage,
                             const bool isPrivate, const bool isVisible, const QImage& picture, QSet<quint32> admins,
                             QSet<quint32> users)
{
    _rooms[idRoom].modifyRoom(name, limitOfStoredMessage, isPrivate, isVisible, picture, admins, users);
}

void ModelChator::deleteRoom(const quint32 idRoom)
{
    QMap<quint32, ModelMessage> messages = _rooms[idRoom].getMessages();
    for (ModelMessage message : messages)
    {
        _rooms[idRoom].deleteMessage(message.getIdMessage());
    }

    _rooms.remove(idRoom);
}

ModelMessage& ModelChator::getMessage(const quint32 idRoom, const quint32 idMessage)
{
    return _rooms[idRoom].getMessage(idMessage);
}

void ModelChator::modifyMessage(const quint32 idRoom, const quint32 idMessage, const QByteArray& content, const QDateTime lastEditionDate)
{
    _rooms[idRoom].modifyMessage(idMessage, content, lastEditionDate);
}

void ModelChator::deleteMessage(const quint32 idRoom, const quint32 idMessage)
{
    _rooms[idRoom].deleteMessage(idMessage);
}

void ModelChator::removeUser(const quint32 userId, const quint32 roomId)
{
    _rooms[roomId].removeUser(userId);
}

void ModelChator::addMembershipRequest(const quint32 roomId, const ModelUser &user,
                                       const QByteArray &publicKey)
{
    ++_nbRequests;
    _requests.insert(_nbRequests, ModelRequest(_nbRequests, _rooms[roomId], user, publicKey));
}

void ModelChator::deleteRequest(const quint32 requestId)
{
    _requests.remove(requestId);
}

qint32 ModelChator::searchRequest(const quint32 roomId, const quint32 userId)
{
    for (ModelRequest request : _requests)
    {
        if (request.getRoom().getIdRoom() == roomId && request.getUser().getIdUser() == userId)
        {
            return request.getId();
        }
    }

    return -1;
}

QMap<quint32, ModelRequest> ModelChator::getRequests() const
{
    return _requests;
}

ModelRequest& ModelChator::getRequest(const quint32 idRequest)
{
    return _requests[idRequest];
}

ModelMessage& ModelRoom::getMessage(const quint32 idMessage)
{
    return _messages[idMessage];
}

void ModelRoom::addMessage(const quint32 idMessage, const quint32 idRoom, const quint32 idUser, const QDateTime& date, const QDateTime& lastEditionDate, const QByteArray& content)
{
    _messages.insert(idMessage, ModelMessage(idMessage, idRoom, idUser, date, lastEditionDate, content));
}

void ModelRoom::addMessage(const ModelMessage& message)
{
    _messages.insert(message.getIdMessage(), ModelMessage(message));
}

void ModelRoom::modifyMessage(const quint32 idMessage, const QByteArray& content, const QDateTime lastEditionDate)
{
    _messages[idMessage].modify(content, lastEditionDate);
}

void ModelRoom::deleteMessage(const quint32 idMessage)
{
    _messages.remove(idMessage);
}

void ModelRoom::modifyRoom(const QString& name, const quint32 limitOfStoredMessage, const bool isPrivate,
                           const bool isVisible, const QImage& picture, QSet<quint32> admins, QSet<quint32> users)
{
    _name = name;
    _limitOfStoredMessage = limitOfStoredMessage;
    _private = isPrivate;
    _visible = isVisible;
    _picture = picture;
    _admins = admins;
    _members = users;
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

const QString& ModelUser::getUserName() const
{
    return _userName;
}

const QString& ModelUser::getFirstName() const
{
    return _firstName;
}

const QString& ModelUser::getLastName() const
{
    return _lastName;
}

void ModelUser::setFirstName(const QString firstName)
{
    _firstName = firstName;
}

void ModelUser::setLastName(const QString lastName)
{
    _lastName = lastName;
}

bool ModelUser::isConnected() const
{
    return _isConnected;
}

const QImage& ModelUser::getImage() const
{
    return _image;
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

const QString& ModelRoom::getName() const
{ 
    return _name;
}

const QImage& ModelRoom::getPicture() const
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

AESKey& ModelRoom::getSecretKey()
{
    return _secretKey;
}

void ModelRoom::setIdRoom(const quint32 id)
{
    _idRoom = id;
}

QDataStream& operator<< (QDataStream& ds, const vector<unsigned char>& v)
{
    QByteArray ba((char*)v.data(), v.size());
    return ds << ba;
}

QDataStream& operator>> (QDataStream& ds, vector<unsigned char>& v)
{
    QByteArray ba;
    ds >> ba;
    v.resize(ba.size());
    memcpy(v.data(), ba.data(), v.size());
    return ds;
}

QDataStream& operator<< (QDataStream& ds, const vector<char>& v)
{
    QByteArray ba((char*)v.data(), v.size());
    return ds << ba;
}

QDataStream& operator>> (QDataStream& ds, vector<char>& v)
{
    QByteArray ba;
    ds >> ba;
    v.resize(ba.size());
    memcpy(v.data(), ba.data(), v.size());
    return ds;
}

QDataStream& operator<< (QDataStream& ds, const RSAPair& p)
{
    return ds << p.privateKey << p.publicKey;
}

QDataStream& operator>> (QDataStream& ds, RSAPair& p)
{
    return ds >> p.privateKey >> p.publicKey;
}

QDataStream& operator<< (QDataStream& ds, const AESKey& k)
{
    return ds << k.key << k.initializationVector;
}

QDataStream& operator>> (QDataStream& ds, AESKey& k)
{
    return ds >> k.key >> k.initializationVector;
}

void ModelUser::setImage(const QImage& image)
{
    _image = image;
}

void ModelRoom::setPicture(const QImage& picture)
{
    _picture = picture;
}

void ModelRoom::setKey(const AESKey& aeskey)
{
    _secretKey = aeskey;
}

ModelRequest::ModelRequest() {}

ModelRequest::ModelRequest(const quint32 id, const ModelRoom& room,
                           const ModelUser& user, const QByteArray& publicKey) :
    _id(id), _room(room), _user(user), _publicKey(publicKey) {}

ModelRequest::~ModelRequest() {}

quint32 ModelRequest::getId() const
{
    return _id;
}

ModelRoom ModelRequest::getRoom() const
{
    return _room;
}

ModelUser ModelRequest::getUser() const
{
    return _user;
}

QByteArray& ModelRequest::getPublicKey()
{
    return _publicKey;
}

ModelError::ModelError() {}
ModelError::ModelError(ErrorType errorType, QString errorString) : _errorType(errorType), _errorString(errorString) {}

QDataStream& operator<< (QDataStream& ds, const ModelError& r)
{
    return ds << (quint32) r._errorType << r._errorString;
}

ErrorType ModelError::getErrorType() const
{
    return _errorType;
}

QString ModelError::getErrorString() const
{
    return _errorString;
}

QDataStream& operator>> (QDataStream& ds, ModelError& r)
{
    quint32 errorType;
    ds >> errorType;
    r._errorType = (ErrorType) errorType;
    return ds >> r._errorString;
}
