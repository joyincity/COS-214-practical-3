#ifndef VIPPASS_H
#define VIPPASS_H

#include "EventComponent.h"
#include "Observer.h"
#include "Control.h"
#include "Notice.h"
#include <iostream>

class VipPass : public EventComponent, public Observer {
private:
    std::string passHolderName;
    bool isActive;
    int priorityLevel; 
    Control* registeredSubject;

public:
    VipPass(const std::string& name, int priority = 3) 
        : passHolderName(name), isActive(true), priorityLevel(priority), registeredSubject(nullptr) {}

    virtual ~VipPass() {
        if (registeredSubject) {
            registeredSubject->detach(this);
            registeredSubject = nullptr;
        }
    }

    void setObservedSubject(Control* subject) override {
        registeredSubject = subject;
    }

    void open() override {
        isActive = true;
        std::cout << " VIP Pass for " << passHolderName << " ACTIVATED (Priority: " 
                  << priorityLevel << ")\n";
    }

    void close() override {
        isActive = false;
        std::cout << "VIP Pass for " << passHolderName << " DEACTIVATED.\n";
    }

    void reportStatus() const override {
        std::cout <<  passHolderName << " | Active: " << (isActive ? "Yes" : "No")
                  << " | Priority: " << priorityLevel << "\n";
    }

    int getCapacity() const override { return 1; }
    std::string getName() const override { return "VIP: " + passHolderName; }

    void registerWithSubject(Control* subject) {
        if (subject == nullptr) return;
        registeredSubject = subject;
        subject->attach(this);
    }

    void update(Control* subject) override {
        if (registeredSubject == nullptr) {
            registeredSubject = subject;
        }

        if (!subject->hasCurrentNotice()) return;
        const Notice& notice = subject->getCurrentNotice();

        switch (notice.getType()) {
            case NoticeType::CAPACITY_ALERT:
                if (isActive) {
                    std::cout << "VIP " << passHolderName << " granted priority entry! "
                              << "(Priority " << priorityLevel << ")\n";
                }
                break;

            case NoticeType::EVACUATE:
                if (isActive) {
                    std::cout << "VIP " << passHolderName << " evacuated to VIP lounge.\n";
                }
                break;

            case NoticeType::OPEN:
                open();
                break;

            case NoticeType::CLOSE:
                close();
                break;

            default:
                break;
        }
    }
};

#endif // VIPPASS_H