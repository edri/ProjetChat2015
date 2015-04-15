#include "packet.h"

Error::Error() {}
Error::Error(ErrorType errorType, QString errorString) : _errorType(errorType), _errorString(errorString) {}

QDataStream& operator<< (QDataStream& ds, const Error& r)
{
    return ds << (quint32) r._errorType << r._errorString;
}

QDataStream& operator>> (QDataStream& ds, Error& r)
{
    quint32 errorType;
    ds >> errorType;
    r._errorType = (ErrorType) errorType;
    return ds >> r._errorString;
}
