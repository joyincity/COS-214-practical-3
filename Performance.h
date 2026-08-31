#ifndef PERFORMANCE_H
#define  PERFORMANVE_H

#include "EventComponent.h"
#include "Observer.h"
#include "Control.h"
#include "Notice.h"
#include <iostream>

class Performance:public EventComponent,public Observer{
    private:
    std::string unitName;
    std::string showTitle;
    bool isOutdoor;
    bool isPaused;
    bool isActive;

    Control* registeredSubject;

    public:
    Performance(const std::string& name,const std::string& title,bool outdoor=true): unitName(name),showTitle(title),isOutdoor(outdoor),isPaused(false),isActive(true),registeredSubject(nullptr){}
    virtual ~Performance(){
        if(registeredSubject!=nullptr){
            registeredSubject->detach(this);
            registeredSubject=nullptr;
        }
    }

    void setObservedSubject(Control* subject) override {
        registeredSubject = subject;
    }
    
    /*Event Component Interface*/
    void open() override{
        isActive=true;
        isPaused=false;
        std::cout<<unitName<<" ("<<showTitle<<") is now OPEN.\n";
    }

    void close() override{
        isActive=false;
        std::cout<<unitName<<"("<<showTitle<<" )is now CLOSED.\n";
    }

    void reportStatus() const override{
        std::cout<<unitName<<" ["<<showTitle<<"] | "<<"Active: "<<(isActive?"Yes":"No")<<" | Paused: "<<(isPaused?"Yes":"No")<<" | Outdoor: "<<(isOutdoor?"Yes":"No")<<"\n";
    }

    int getCapacity() const override{
        return isOutdoor?500:200;
    }

    std::string getName() const override{return unitName;}

    /*Observer Interface*/
    void update(Control* subject)override{
        if(registeredSubject==nullptr){
            registeredSubject=subject;
        }

        if(!subject->hasCurrentNotice())return;

        const Notice& notice=subject->getCurrentNotice();

        switch(notice.getType()){
            /*If outdoors and ongoing then pause and close*/
            case NoticeType::WEATHER_ALERT:
                if(isOutdoor && isActive){
                    isPaused=true;
                    std::cout<<unitName<<" ("<<showTitle<<" ) PAUSED due to weather alert!\n";
                    close();            
                }
                else if(!isOutdoor){
                    std::cout<<unitName<<" ("<<showTitle<<") INDOOR-continuing unaffected.\n";
                }
                break;
            /*If resume and was paused resume*/
            case NoticeType::RESUME:
                if(isPaused){
                    isPaused=false;
                    isActive=true;
                    std::cout<<unitName<<" ("<<showTitle<<") RESUMED!\n";
                    open();
                }
                break;
            /*if Evacuate then no longer active*/
            case NoticeType::EVACUATE:
                if(isActive){
                    isActive=false;
                    std::cout<<unitName<<" ("<<showTitle<<") EVACUTAED!\n";
                }
                break;
            case NoticeType::OPEN:
                if(!isActive){
                    open();
                }
                break;
            case NoticeType::CLOSE:
                if(isActive){
                    std::cout<<unitName<<" ("<<showTitle<<") CLOSING for the day.\n";
                    close();
                }
                break;
            case NoticeType::PAUSE:
                if(isActive && !isPaused){
                    isPaused=true;
                    std::cout<<unitName<<" ("<<showTitle<<") PAUSED (temporary).\n";
                }
                break;
             case NoticeType::SCHEDULE_CHANGE:
                std::cout <<unitName << " (" << showTitle
                          << ") SCHEDULE CHANGED: " << notice.getMessage() << "\n";
                break;
            case NoticeType::CAPACITY_ALERT:
                std::cout << unitName << " (" << showTitle
                          << ") CAPACITY ALERT: " << notice.getMessage()
                          << " (Current cap: " << getCapacity() << ")\n";
                break;

            default:
                std::cout <<unitName << " noted: "
                          << notice.getMessage() << "\n";
                break;
        }
    }
};  
#endif //PERFORMANCE_H