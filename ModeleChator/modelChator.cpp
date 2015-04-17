#include "modelChator.h"

ModelChator::ModelChator() {}

ModelMessage::ModelMessage() {}
ModelMessage::~ModelMessage(){}

ModelUser::ModelUser() {}

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

ModelRoom::ModelRoom(const quint32 idRoom, const QString& name, const quint32 limitOfStoredMessage, const bool isPrivate,
          const bool isVisible, const QString& picture, QMap<quint32, ModelUser*>& admins, QMap<quint32, ModelUser*>& users) :
    _idRoom(idRoom), _name(name), _limitOfStoredMessage(limitOfStoredMessage), _private(isPrivate), _visible(isVisible),
    _picture(picture), _admins(admins), _users(users) {}

ModelRoom::~ModelRoom(){}

QMap<quint32, ModelUser*> ModelRoom::getUsers() const
{
    return _users;
}

quint32 ModelRoom::getIdRoom() const
{
    return _idRoom;
}

ModelRoom* ModelChator::getRoom(const quint32 idRoom) const
{
    return _rooms[idRoom];
}

void ModelChator::addUser(const quint32 idUser, const QString& userName, const QString& firstName, const QString& lastName, const bool isConnected, const QDateTime& lastConnection, const QImage& image)
{
    ModelUser* user = new ModelUser(idUser, userName, firstName, lastName, isConnected, lastConnection, image);
    _users.insert(idUser, user);
}

void ModelChator::addUser(ModelUser* user)
{
    _users.insert(user->getIdUser(), user);
}

void ModelChator::addRoom(const quint32 idRoom, const QString &name, const quint32 limitOfStoredMessage, const bool isPrivate, const bool isVisible, const QString &picture, QMap<quint32, ModelUser*>& admins, QMap<quint32, ModelUser*>& users)
{
    ModelRoom* room = new ModelRoom(idRoom, name, limitOfStoredMessage, isPrivate, isVisible, picture, admins, users);
    _rooms.insert(idRoom, room);
}

void ModelChator::addRoom(ModelRoom *room)
{
    _rooms.insert(room->getIdRoom(), room);
}

QMap<quint32, ModelRoom*> ModelChator::getUserRooms(const quint32 idUser) const
{
    QMap<quint32, ModelRoom*> userRooms;

    for (quint32 room : _rooms.keys())
    {
        for (quint32 user : _rooms.value(room)->getUsers().keys())
        {
            if (_users.value(user)->getIdUser() == idUser)
            {
                userRooms.insert(_rooms.value(room)->getIdRoom(), _rooms.value(room));
                break;
            }
        }
    }

    return userRooms;
}

quint32 ModelUser::getIdUser() const
{
    return _idUser;
}

QString ModelUser::getUserName() const
{
    return _userName;
}

QString ModelRoom::getName() const 
{ 
    return _name;
}

QString ModelRoom::getPicture() const
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

