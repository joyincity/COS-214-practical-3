#ifndef THEWATCH_H
#define THEWATCH_H

#include "EventComponent.h"
#include "Observer.h"
#include "Control.h"
#include <iostream>

class TheWatch:public EventComponent,public Observer{
    private:
    std::string unitName;
    int teamSize;
    bool onDuty;
    bool redirecting;
    Control* registeredSubject;

    public:
    TheWatch(const std::string& name,int size=5):unitName(name),teamSize(size),onDuty(true),redirecting(false),registeredSubject(nullptr){}
    virtual ~TheWatch(){
        if(registeredSubject){
            registeredSubject->detach(this);
            registeredSubject=nullptr;
        }
    }

    void setObservedSubject(Control* subject) override {
        registeredSubject = subject;
    }

    void open() override{
        onDuty=true;
        redirecting=false;
        std::cout<<unitName<<" (Team of "<<teamSize<<") is ON DUTY.\n";
    }

    void close() override{
        onDuty=false;
        std::cout<<unitName<<" is OFF DUTY.\n";
    }

    void reportStatus() const override {
        std::cout <<unitName << " | Team: " << teamSize
                  << " | On Duty: " << (onDuty ? "Yes" : "No")
                  << " | Redirecting: " << (redirecting ? "Yes" : "No") << "\n";
    }

    int getCapacity() const override { return teamSize * 100; }
    std::string getName() const override { return unitName; }

    void update(Control* subject) override {
        if (registeredSubject == nullptr) {
            registeredSubject = subject;
        }

        if (!subject->hasCurrentNotice()) return;
        
        const Notice& notice = subject->getCurrentNotice();
        
        switch (notice.getType()) {
            case NoticeType::WEATHER_ALERT:
                redirecting = true;
                std::cout <<unitName << " REMAINS ACTIVE. "
                          << "Redirecting attendees to shelter.\n";
                break;
                
            case NoticeType::RESUME:
                redirecting = false;
                std::cout << unitName << " normal patrol resumed.\n";
                break;
                
            case NoticeType::EVACUATE:
                if (onDuty) {
                    std::cout <<unitName << " GUIDING attendees to exits.\n";
                }
                break;
            
            case NoticeType::OPEN:
                open();
                break;

            case NoticeType::CLOSE:
                if (onDuty) {
                    std::cout <<unitName << " CLOSING for the day.\n";
                    close();
                }
                break;

            case NoticeType::PAUSE:
                std::cout << unitName << " ON STAND BY (pause).\n";
                break;
            
            case NoticeType::SCHEDULE_CHANGE:
                std::cout <<unitName << " adjusting patrol schedule: "
                          << notice.getMessage() << "\n";
                break;

            case NoticeType::CAPACITY_ALERT:
                std::cout <<unitName << " CAPACITY ALERT: "
                          << "Preparing crowd control (Cap: " << getCapacity() << ")\n";
                break;
                
            default:
                std::cout <<unitName << " noted: " 
                          << notice.getMessage() << "\n";
                break;
        }
    }
    
};




#endif //THEWATCH_H