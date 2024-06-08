#include "stdafx.h"
#include "ACommand.h"

APROCH_NAMESPACE_BEGIN

bool ACommand::IsValid(const CommandId &cmdId)
{
    QRegularExpression regExp(AStr("^(([a-z])+([a-z\\d])*)(\.(([a-z])+([a-z\\d])*))*$"), 
                              QRegularExpression::CaseInsensitiveOption);
    return regExp.match(cmdId).hasMatch();
}

APROCH_NAMESPACE_END
