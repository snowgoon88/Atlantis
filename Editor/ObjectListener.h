#ifndef OBJECTLISTENER_H
#define OBJECTLISTENER_H

/**
 * Listen for int (type of Object) from UnitEditorDialog.
 */
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class ObjectListener
{
public:
    virtual void receive_object( int type )
    {
        std::cout << "ObjectListener::receive_object" << std::endl;
    };
};

#endif // OBJECTLISTENER
