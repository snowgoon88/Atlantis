#ifndef OBSERVER_H
#define OBSERVER_H

/**
 * Virtual class that can observe a model.
 */
class Observer
{
public:
    virtual void update( int signal=0 ) = 0;
};

#endif // OBSERVER_H

