#ifndef TENT_H
#define TENT_H

#include "EventComponent.h"
#include "Observer.h"
#include "Control.h"
#include <iostream>

class Tent : public EventComponent, public Observer {
private:
    std::string unitName;
    int maxOccupancy;
    bool isPrepared;
    bool isOccupied;
    Control* registeredSubject;

public:
    Tent(const std::string& name, int capacity = 2)
        : unitName(name), maxOccupancy(capacity), 
          isPrepared(false), isOccupied(false), registeredSubject(nullptr) {}

    virtual ~Tent() {
        if (registeredSubject) {
            registeredSubject->detach(this);
            registeredSubject = nullptr;
        }
    }

    void open() override {
        isPrepared = true;
        std::cout << unitName << " is OPEN (Capacity: " 
                  << maxOccupancy << ").\n";
    }
    
    void close() override {
        isPrepared = false;
        std::cout <<unitName << " is CLOSED.\n";
    }
    
    void reportStatus() const override {
        std::cout <<unitName << " | Capacity: " << maxOccupancy
                  << " | Prepared: " << (isPrepared ? "Yes" : "No")
                  << " | Occupied: " << (isOccupied ? "Yes" : "No") << "\n";
    }
    
    int getCapacity() const override { return maxOccupancy; }
    std::string getName() const override { return unitName; }

    void update(Control* subject) override {
        if (registeredSubject == nullptr) {
            registeredSubject = subject;
        }

        if (!subject->hasCurrentNotice()) return;
        
        const Notice& notice = subject->getCurrentNotice();
        
        switch (notice.getType()) {
            case NoticeType::WEATHER_ALERT:
                isPrepared = true;
                std::cout << unitName << " PREPARED for emergency "
                          << "occupancy! (Capacity: " << maxOccupancy << ")\n";
                open();
                break;
                
            case NoticeType::RESUME:
                isPrepared = false;
                std::cout <<unitName << " returned to normal state.\n";
                break;
                
            default:
                std::cout << unitName << " noted: " 
                          << notice.getMessage() << "\n";
                break;
        }
    }
};

#endif // TENT_H