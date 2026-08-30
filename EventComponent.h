#ifndef EVENTCOMPONENT_H
#define EVENTCOMPONENT_H

#include <iostream>
#include <string>

/*Abstract class for all event components leafs and composites inherit from this*/
class EventComponent{
    public:
    virtual ~EventComponent()=default;
    virtual void open()=0;
    virtual void close()=0;
    virtual void reportStatus() const=0;
    virtual int getCapacity() const=0;
    virtual std::string getName() const=0;
    /*sets new subject this component should observer when transferred*/
     virtual void setObservedSubject(Control* subject) = 0;
};

#endif //EVENTCOMPONENT_H