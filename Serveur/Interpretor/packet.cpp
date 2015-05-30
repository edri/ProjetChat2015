/*
     * Created by Benoist Wolleb
     *
     * Implements packet.h.
*/

#include "packet.h"

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
