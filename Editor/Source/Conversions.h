#pragma once

#include <QString>
#include "Types/VanguardString.h"


inline bool operator !=(const Vanguard::String& vString, const QString& qString) { return qString != vString.GetCharPointer(); }
inline bool operator !=(const QString& qString, const Vanguard::String& vString) { return qString != vString.GetCharPointer(); }

inline bool operator ==(const Vanguard::String& vString, const QString& qString) { return qString == vString.GetCharPointer(); }
inline bool operator ==(const QString& qString, const Vanguard::String& vString) { return qString == vString.GetCharPointer(); }

inline Vanguard::String ToVanguardString(const QString& qString) { return qString.toStdString().c_str(); }
inline QString ToQString(const Vanguard::String& vString) { return vString.GetCharPointer();}