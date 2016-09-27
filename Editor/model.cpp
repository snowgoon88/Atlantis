
#include <model.h>
#include <algorithm>

// ***************************************************** Model::attach_observer
void Model::attach_observer( ObserverPtr obs )
{
    _observer_list.push_back(obs);
}
// ***************************************************** Model::detach_observer
void Model::detach_observer( ObserverPtr obs )
{
    auto obs_it = std::find( _observer_list.begin(), _observer_list.end(), obs);
    if( obs_it != _observer_list.end() ) {
        _observer_list.erase( obs_it );
    }
}
// **************************************************** Model::notify_observers
void Model::notify_observers( int signal )
{
    for( auto& obs : _observer_list) {
        obs->update( signal );
    }
}
// ****************************************************************************
