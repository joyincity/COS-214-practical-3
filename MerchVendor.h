/**
 * @file MerchVendor.h
 * @brief Defines a Leaf in Composite in the Mystifare fair and a ConcreteObserver in Observer.
 *
 * This file contains the MerchVendor class, which represents a merchandise
 * vendor selling costumes, souvenirs, and crafts at the Mystifare fair.
 * It is a Leaf in the Composite tree that reacts to notices by packing up
 * or unpacking merchandise based on its outdoor status.
 *
 * @note This is one of 5 concrete leaf types required by the specification.
 *
 * @author COS 214 group
 * @date 2026
 */



#ifndef MERCHVENDOR_H
#define MERCHVENDOR_H

#include "EventComponent.h"
#include "Observer.h"
#include "Control.h"
#include <iostream>

/**
 * @brief Concrete merchandise vendor that sells costumes, souvenirs, and crafts.
 *
 * This class represents a merchandise vendor at the Mystifare fair. It is a
 * Leaf in the Composite pattern (cannot contain children) and a Concrete
 * Observer in the Observer pattern.
 *
 * **Reaction to notices:**
 * - `WEATHER_ALERT`: If outdoor, **PACKS UP** merchandise and closes.
 * - `RESUME`: Unpacks and reopens for sales.
 * - `CLOSE`: Secures inventory and locks up.
 * - `EVACUATE`: Evacuates customers and secures merchandise.
 * - `OPEN`: Opens for shopping.
 * - `PAUSE`: Pauses shopping.
 * - `SCHEDULE_CHANGE`: Logs store hours updates.
 * - `CAPACITY_ALERT`: Logs capacity management.
 *
 * @note This is one of 5 concrete leaf types required by the specification.
 * @note Demonstrates condition-based decision: `if (isOutdoor && isOpen)`
 *       (Task 4.3).
 * @see EventComponent, Observer
 */
class MerchVendor : public EventComponent, public Observer {
private:
    /**
     * @brief The name of this vendor (e.g., "Merchant's Guild").
     */
    std::string unitName;

    /**
     * @brief The type of merchandise sold (e.g., "Costumes", "Souvenirs").
     */
    std::string merchType;

    /**
     * @brief Whether this vendor is located outdoors.
     *
     * Determines the reaction to WEATHER_ALERT. If true, the vendor
     * packs up merchandise; if false, the vendor continues unaffected.
     */
    bool isOutdoor;

    /**
     * @brief Whether the vendor is currently open for shopping.
     */
    bool isOpen;

    /**
     * @brief Whether merchandise is packed away.
     *
     * When true, the vendor is closed and inventory is secured.
     */
    bool isPacked;

    
    /**
     * @brief Pointer to the Subject this observer is registered with.
     *
     * Stored for safe self-detachment in the destructor.
     */
    Control* registeredSubject;
    
    /**
     * @brief Whether the vendor has been evacuated.
     */
    bool isEvacuated;


public:
    /**
     * @brief Constructs a new MerchVendor.
     *
     * Initializes the vendor as open, not packed, and not evacuated.
     *
     * @param name The display name of the vendor.
     * @param merch The type of merchandise sold.
     * @param outdoor Whether the vendor is located outdoors. Defaults to true.
     */
    MerchVendor(const std::string& name, const std::string& merch, bool outdoor = true)
        : unitName(name), merchType(merch), isOutdoor(outdoor), isOpen(true), isPacked(false), registeredSubject(nullptr) {}

    /**
     * @brief Destructor that automatically detaches from the Subject.
     *
     * Prevents dangling pointers in the Subject's observer list.
     *
     * @note This is part of the safe destruction policy (Task 3.2).
     * @warning Must be called before the Subject is destroyed.
     */
    virtual ~MerchVendor() {
        if (registeredSubject) {
            registeredSubject->detach(this);
            registeredSubject = nullptr;
        }
    }

    /**
     * @brief Updates the internal Subject pointer for safe self-detachment.
     *
     * Used exclusively by the transfer mechanism (EventZone::transfer())
     * to update the registeredSubject pointer when this vendor is moved
     * to a new parent zone.
     *
     * @param subject The new Subject this vendor should observe.
     *
     * @see EventZone::transfer()
     */
    void setObservedSubject(Control* subject) override {
        registeredSubject = subject;
    }

    /**
     * @brief Opens the merchandise vendor for shopping.
     *
     * Sets isOpen = true and resets packed and evacuated flags.
     *
     * @post Vendor is open and ready for customers.
     * @see close()
     */
    void open() override {
        isOpen = true;
        isPacked = false;
        std::cout <<unitName << " (" << merchType << ") is now OPEN for shopping.\n";
    }

    /**
     * @brief Closes the merchandise vendor.
     *
     * Sets isOpen = false and secures inventory.
     *
     * @post Vendor is closed and inventory is secured.
     * @see open()
     */
    void close() override {
        isOpen = false;
        std::cout << unitName << " (" << merchType << ") is now CLOSED. Inventory secured.\n";
    }

    /**
     * @brief Reports the current status of the vendor.
     *
     * Outputs name, merchandise type, open status, packed status,
     * evacuated status, and whether it is located outdoors.
     *
     * @post No state changes occur (const operation).
     */
    void reportStatus() const override {
        std::cout << unitName << " [" << merchType << "] | "
                  << "Open: " << (isOpen ? "Yes" : "No")
                  << " | Packed: " << (isPacked ? "Yes" : "No")
                  << " | Outdoor: " << (isOutdoor ? "Yes" : "No") << "\n";
    }

    /**
     * @brief Returns the customer capacity of this vendor.
     *
     * Outdoor vendors have a capacity of 30, indoor vendors have 15.
     *
     * @return int The maximum number of customers this vendor can serve.
     */
    int getCapacity() const override {
        return isOutdoor ? 30 : 10; // Max customers in store
    }

    /**
     * @brief Returns the name of this vendor.
     *
     * @return std::string The vendor's display name.
     */
    std::string getName() const override { return unitName; }


    /**
     * @brief Called by the Subject when a new notice is available.
     *
     * This method implements the PULL approach:
     * 1. Stores the Subject pointer for self-detachment.
     * 2. Checks if a notice is available via hasCurrentNotice().
     * 3. Pulls the notice data via getCurrentNotice().
     * 4. Reacts based on the notice type.
     *
     * **Reactions:**
     * - `WEATHER_ALERT`: If outdoor, PACKS UP merchandise and closes.
     * - `EVACUATE`: Evacuates customers and secures merchandise.
     * - `OPEN`: Opens for shopping.
     * - `CLOSE`: Closes and secures inventory.
     * - `PAUSE`: Pauses shopping.
     * - `RESUME`: Unpacks and resumes sales.
     * - `SCHEDULE_CHANGE`: Logs store hours updates.
     * - `CAPACITY_ALERT`: Logs capacity management.
     *
     * @param subject The Subject that issued the notification.
     *
     * @note This is a Concrete Observer implementation.
     * @note Demonstrates condition-based decision: `if (isOutdoor && isOpen)`
     *       (Task 4.3).
     * @see Control::getCurrentNotice()
     */
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
                if (isPacked || isEvacuated) {
                    isPacked = false;
                    isOpen = true;
                    std::cout <<unitName << " UNPACKED and RESUMED sales.\n";
                    open();
                }
                break;

            case NoticeType::EVACUATE:
                if (isOpen) {
                    isEvacuated = true;
                    std::cout <<unitName << " EVACUATING customers "
                              << "and securing merchandise.\n";
                    close();
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