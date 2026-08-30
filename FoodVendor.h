#ifndef FOODVENDOR_H
#define FOODVENDOR_H

#include "EventComponent.h"
#include "Observer.h"
#include "Control.h"
#include <iostream>

class FoodVendor : public EventComponent, public Observer {
private:
    std::string unitName;
    std::string cuisineType;
    bool hasOutdoorSeating;
    bool isSuspended;
    bool isOpen;
    Control* registeredSubject;

public:
    FoodVendor(const std::string& name, const std::string& cuisine, bool outdoor = true)
        : unitName(name), cuisineType(cuisine), hasOutdoorSeating(outdoor),
          isSuspended(false), isOpen(true), registeredSubject(nullptr) {}

    virtual ~FoodVendor() {
        if (registeredSubject) {
            registeredSubject->detach(this);
            registeredSubject = nullptr;
        }
    }

    void open() override {
        isOpen = true;
        isSuspended = false;
        std::cout <<unitName << " (" << cuisineType 
                  << ") is now OPEN for service.\n";
    }
    
    void close() override {
        isOpen = false;
        std::cout <<unitName << " (" << cuisineType 
                  << ") is now CLOSED.\n";
    }
    
    void reportStatus() const override {
        std::cout << unitName << " [" << cuisineType << "] | "
                  << "Open: " << (isOpen ? "Yes" : "No")
                  << " | Suspended: " << (isSuspended ? "Yes" : "No")
                  << " | Outdoor: " << (hasOutdoorSeating ? "Yes" : "No") << "\n";
    }
    
    int getCapacity() const override { return hasOutdoorSeating ? 50 : 20; }
    std::string getName() const override { return unitName; }

    void update(Control* subject) override {
        if (registeredSubject == nullptr) {
            registeredSubject = subject;
        }

        if (!subject->hasCurrentNotice()) return;
        
        const Notice& notice = subject->getCurrentNotice();
        
        switch (notice.getType()) {
            case NoticeType::WEATHER_ALERT:
                if (hasOutdoorSeating && isOpen) {
                    isSuspended = true;
                    std::cout <<unitName << " SUSPENDED outdoor service "
                              << "due to weather alert!\n";
                    close();
                } else if (!hasOutdoorSeating) {
                    std::cout <<unitName << " INDOOR - continuing service.\n";
                }
                break;
                
            case NoticeType::RESUME:
                if (isSuspended) {
                    isSuspended = false;
                    isOpen = true;
                    std::cout <<unitName << " RESUMED normal service.\n";
                    open();
                }
                break;
                
            default:
                std::cout <<unitName << " noted: " 
                          << notice.getMessage() << "\n";
                break;
        }
    }
};

#endif // FOODVENDOR_H