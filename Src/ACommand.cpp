#include "stdafx.h"
#include "ACommand.h"

namespace aproch
{
    bool ACommand::IsValid(const CommandId& cmdId)
    {
        const QRegExp regExp(AStr("^(([a-z])+([a-z\\d])*)(\.(([a-z])+([a-z\\d])*))*$"), Qt::CaseInsensitive);
        return regExp.exactMatch(cmdId);
    }
}
