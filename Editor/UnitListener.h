#ifndef UNITLISTENER_H
#define UNITLISTENER_H

/**
 * Listen for Unit*.
 */
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class UnitListener
{
public:
    virtual void receive_unit(Unit* unit)
    {
        std::cout << "UnitListener::receive_unit" << std::endl;
    };
};

#endif // UNITLISTENER
