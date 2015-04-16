#include "modelChator.h"

ModelMessage::ModelMessage() {};
ModelMessage::~ModelMessage(){};

ModelUser::ModelUser() {};
ModelUser::~ModelUser(){};

QDataStream& operator<< (QDataStream& ds, const ModelRoom& r)
{
    return ds;
}

QDataStream& operator>> (QDataStream& ds, ModelRoom& r)
{
    return ds;
}

QDataStream& operator<< (QDataStream& ds, const ModelMessage& m)
{
    return ds << m.idMessage << m.idUser << m.date << m.content;
}

QDataStream& operator>> (QDataStream& ds, ModelMessage& m)
{
    return ds >> m.idMessage >> m.idUser >> m.date >> m.content;
}

QDataStream& operator<< (QDataStream& ds, const ModelUser& u)
{
    return ds << u.userName << u.firstName << u.lastName << u.isConnected << u.lastConnection << u.image;
}

QDataStream& operator>> (QDataStream& ds, ModelUser& u)
{
    return ds >> u.userName >> u.firstName >> u.lastName >> u.isConnected >> u.lastConnection >> u.image;
}

ModelRoom::ModelRoom(){}
ModelRoom::~ModelRoom(){}

QMap<quint32, ModelUser*> ModelRoom::getUsers() const
{
    return users;
}

ModelRoom ModelChator::getRoom(const quint32 idRoom) const
{
    return rooms[idRoom];
}

QString ModelUser::getUserName() const
{
    return userName;
}

QString ModelRoom::getName() const 
{ 
    return name;
}

QString ModelRoom::getPicture() const
{
    return picture;
}

quint32 ModelRoom::getLimit() const
{
    return limitOfStoredMessage;
}

bool ModelRoom::isPrivate() const
{
    return _private;
}
bool ModelRoom::isVisible() const
{
    return visible;
}

