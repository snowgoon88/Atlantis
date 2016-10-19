#ifndef UNITEDITLISTENER_H
#define UNITEDITLISTENER_H

/**
 * Listen for nothing from UnitEditorDialog.
 */
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class UnitEditListener
{
public:
    virtual void receive_unitedit()
    {
        std::cout << "UnitEditListener::receive_unitedit" << std::endl;
    };
};

#endif // UNITEDITLISTENER
