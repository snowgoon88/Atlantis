#ifndef MODEL_H
#define MODEL_H

/**
 * Virtual class of a Model to be observed.
 */
 #include <list>
 #include <observer.h>

using ObserverPtr = Observer*;

class Model
{
public:
    void attach_observer( ObserverPtr obs );
    void detach_observer( ObserverPtr obs );
    void notify_observers( int signal = 0);

private:
    std::list<ObserverPtr> _observer_list;
};
#endif // MODEL_H
