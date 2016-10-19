#ifndef LOCALISATIONLISTENER_H
#define LOCALISATIONLISTENER_H

/**
 * Listen for std::pair<ARegion*,Objet*>.
 */
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

using Localisation= std::pair<ARegion*,Object*>;

class LocalisationListener
{
public:
    virtual void receive_localisation(Localisation& loc)
    {
        std::cout << "LocalisationListener::receive_localisation" << std::endl;
    };
};

#endif // LOCALISATIONLISTENER
