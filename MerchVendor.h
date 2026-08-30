
#ifndef MERCHVENDOR_H
#define MERCHVENDOR_H

#include "EventComponent.h"
#include "Observer.h"
#include "Control.h"
#include <iostream>

class MerchVendor : public EventComponent, public Observer {
private:
    std::string unitName;
    std::string merchType;
    bool isOutdoor;
    bool isOpen;
    bool isPacked;
    Control* registeredSubject;

public:
    MerchVendor(const std::string& name, const std::string& merch, bool outdoor = true)
        : unitName(name), merchType(merch), isOutdoor(outdoor), isOpen(true), isPacked(false), registeredSubject(nullptr) {}

    virtual ~MerchVendor() {
        if (registeredSubject) {
            registeredSubject->detach(this);
            registeredSubject = nullptr;
        }
    }

    void open() override {
        isOpen = true;
        isPacked = false;
        std::cout <<unitName << " (" << merchType << ") is now OPEN for shopping.\n";
    }

    void close() override {
        isOpen = false;
        std::cout << unitName << " (" << merchType << ") is now CLOSED. Inventory secured.\n";
    }

    void reportStatus() const override {
        std::cout << unitName << " [" << merchType << "] | "
                  << "Open: " << (isOpen ? "Yes" : "No")
                  << " | Packed: " << (isPacked ? "Yes" : "No")
                  << " | Outdoor: " << (isOutdoor ? "Yes" : "No") << "\n";
    }

    int getCapacity() const override {
        return isOutdoor ? 30 : 10; // Max customers in store
    }

    std::string getName() const override { return unitName; }

    void update(Control* subject) override {
        if (registeredSubject == nullptr) {
            registeredSubject = subject;
        }

        if (!subject->hasCurrentNotice()) return;

        const Notice& notice = subject->getCurrentNotice();

        switch (notice.getType()) {
            case NoticeType::WEATHER_ALERT:
                if (isOutdoor && isOpen) {
                    isPacked = true;
                    std::cout <<unitName << " PACKING up outdoor merchandise due to weather!\n";
                    close(); 
                } else if (!isOutdoor) {
                    std::cout <<unitName << " INDOOR store - continuing operations.\n";
                }
                break;

            case NoticeType::CLOSE:
                if (isOpen) {
                    std::cout <<unitName << " CLOSING for the day. Items packed.\n";
                    close();
                }
                break;

            case NoticeType::RESUME:
                if (isPacked) {
                    isPacked = false;
                    isOpen = true;
                    std::cout <<unitName << " UNPACKED and RESUMED sales.\n";
                    open();
                }
                break;
             case NoticeType::OPEN:
                if (!isOpen) {
                    open();
                }
                break;

            case NoticeType::PAUSE:
                if (isOpen) {
                    std::cout << unitName << " PAUSED shopping.\n";
                }
                break;

             case NoticeType::SCHEDULE_CHANGE:
                std::cout << unitName << " store hours updated: "
                          << notice.getMessage() << "\n";
                break;

            case NoticeType::CAPACITY_ALERT:
                std::cout << unitName << " CAPACITY ALERT: "
                          << "Limiting entry (Cap: " << getCapacity() << ")\n";
                break;
            default:
                std::cout <<unitName << " noted: " << notice.getMessage() << "\n";
                break;
        }
    }
};

#endif // MERCHVENDOR_H