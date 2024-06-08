#include "stdafx.h"
#include "AWinUITheme.h"

APROCH_NAMESPACE_BEGIN

APROCH_INIT_SINGLETON(AWinUITheme);

class AWinUIThemePrivate
{
public:
    AWinUIThemePrivate(AWinUITheme* q_ptr)
        : q(q_ptr)
    {
        reloadSystemStyle();
    }

    void reloadSystemStyle()
    {
    }

    AWinUITheme* q = nullptr;
};

AWinUITheme::AWinUITheme()
    : d(new AWinUIThemePrivate(this))
{
}

AWinUITheme::~AWinUITheme()
{
}

APROCH_NAMESPACE_END
