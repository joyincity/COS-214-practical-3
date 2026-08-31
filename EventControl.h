#ifndef EVENTCONTROL_H
#define EVENTCONTROL_H

#include "Control.h"
#include <iostream>

class EventControl:public Control{
    private:
    std::string controlName;

    public:
    EventControl(const std::string& name="Main Control"):controlName(name){}
    virtual ~EventControl()=default;
    void setEvent(const Notice& notice) override{
        std::cout<<"\n["<<controlName<<"] Issuing: "<<notice.getMessage()<<"\n";
        Control::setEvent(notice);
    }
    void notify() override{
        std::cout<<" Notifying "<<observers.size()<<" registered observers...\n";
        Control::notify();
    }

    const std::string& getName() const{return controlName;}
};
#endif //EVENTCONTROL_H