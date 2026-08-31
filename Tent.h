/**
 * @file Tent.h
 * @brief Defines a Leaf in the Composite pattern and ConcereteObserver in Observer pattern in the Mystifare fair.
 *
 * This file contains the Tent class, which represents a camping tent
 * in the Accommodation area of the Mystifare fair. It is a Leaf in the
 * Composite tree that reacts to notices by preparing for emergency
 * occupancy during weather alerts.
 *
 * @note This is one of 5 concrete leaf types required by the specification.
 *
 * @author COS 214 group
 * @date 2026
 */
#ifndef TENT_H
#define TENT_H

#include "EventComponent.h"
#include "Observer.h"
#include "Control.h"
#include <iostream>

/**
 * @brief Concrete tent unit that provides emergency accommodation.
 *
 * This class represents a tent at the Mystifare fair. It is a Leaf in the
 * Composite pattern (cannot contain children) and a Concrete Observer in
 * the Observer pattern.
 *
 * **Reaction to notices:**
 * - `WEATHER_ALERT`: **PREPARES** for emergency occupancy (opens).
 * - `EVACUATE`: Evacuates and closes.
 * - `OPEN`: Opens the tent.
 * - `CLOSE`: Closes the tent.
 * - `PAUSE`: Stands by.
 * - `RESUME`: Returns to normal state (closes).
 * - `SCHEDULE_CHANGE`: Logs check-in time changes.
 * - `CAPACITY_ALERT`: Logs capacity management.
 *
 * @note This is one of 5 concrete leaf types required by the specification.
 * @see EventComponent, Observer
 */
class Tent : public EventComponent, public Observer {
private:
    /**
     * @brief The name of this tent (e.g., "Tent #1").
     */
    std::string unitName;

    /**
     * @brief The maximum number of occupants this tent can hold.
     */
    int maxOccupancy;

    /**
     * @brief Whether the tent is prepared for occupancy.
     */
    bool isPrepared;

    /**
     * @brief Whether the tent is currently occupied.
     */
    bool isOccupied;

    /**
     * @brief Pointer to the Subject this observer is registered with.
     *
     * Stored for safe self-detachment in the destructor.
     */
    Control* registeredSubject;

public:
    /**
     * @brief Constructs a new Tent.
     *
     * Initializes the tent as not prepared and not occupied.
     *
     * @param name The display name of the tent.
     * @param capacity The maximum number of occupants. Defaults to 2.
     */
    Tent(const std::string& name, int capacity = 2)
        : unitName(name), maxOccupancy(capacity), 
          isPrepared(false), isOccupied(false), registeredSubject(nullptr) {}

    /**
     * @brief Destructor that automatically detaches from the Subject.
     *
     * Prevents dangling pointers in the Subject's observer list.
     *
     * @note This is part of the safe destruction policy (Task 3.2).
     * @warning Must be called before the Subject is destroyed.
     */
    virtual ~Tent() {
        if (registeredSubject) {
            registeredSubject->detach(this);
            registeredSubject = nullptr;
        }
    }

    /**
     * @brief Updates the internal Subject pointer for safe self-detachment.
     *
     * Used exclusively by the transfer mechanism (EventZone::transfer())
     * to update the registeredSubject pointer when this tent is moved
     * to a new parent zone.
     *
     * @param subject The new Subject this tent should observe.
     *
     * @see EventZone::transfer()
     */
    void setObservedSubject(Control* subject) override {
        registeredSubject = subject;
    }
    
    /**
     * @brief Opens the tent and prepares it for occupancy.
     *
     * Sets isPrepared = true.
     *
     * @post Tent is prepared and ready for occupants.
     * @see close()
     */
    void open() override {
        isPrepared = true;
        std::cout << unitName << " is OPEN (Capacity: " 
                  << maxOccupancy << ").\n";
    }
    
    /**
     * @brief Closes the tent.
     *
     * Sets isPrepared = false.
     *
     * @post Tent is closed and not available for occupancy.
     * @see open()
     */
    void close() override {
        isPrepared = false;
        std::cout <<unitName << " is CLOSED.\n";
    }
    
    /**
     * @brief Reports the current status of the tent.
     *
     * Outputs name, capacity, prepared status, and occupied status.
     *
     * @post No state changes occur (const operation).
     */
    void reportStatus() const override {
        std::cout <<unitName << " | Capacity: " << maxOccupancy
                  << " | Prepared: " << (isPrepared ? "Yes" : "No")
                  << " | Occupied: " << (isOccupied ? "Yes" : "No") << "\n";
    }
    
    /**
     * @brief Returns the maximum occupancy of this tent.
     *
     * @return int The maximum number of occupants.
     */
    int getCapacity() const override { return maxOccupancy; }

    /**
     * @brief Returns the name of this tent.
     *
     * @return std::string The tent's display name.
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
     * - `WEATHER_ALERT`: **PREPARES** for emergency occupancy (opens).
     * - `EVACUATE`: Evacuates and closes.
     * - `OPEN`: Opens the tent.
     * - `CLOSE`: Closes the tent.
     * - `PAUSE`: Stands by.
     * - `RESUME`: Returns to normal state (closes).
     * - `SCHEDULE_CHANGE`: Logs check-in time changes.
     * - `CAPACITY_ALERT`: Logs capacity management.
     *
     * @param subject The Subject that issued the notification.
     *
     * @note This is a Concrete Observer implementation.
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
                isPrepared = true;
                std::cout << unitName << " PREPARED for emergency "
                          << "occupancy! (Capacity: " << maxOccupancy << ")\n";
                open();
                break;
                
            case NoticeType::RESUME:
                isPrepared = false;
                std::cout <<unitName << " returned to normal state.\n";
                break;
            
            case NoticeType::OPEN:
                open();
                break;

            case NoticeType::CLOSE:
                if (isPrepared) {
                    std::cout <<unitName << " CLOSING for the day.\n";
                    close();
                }
                break;

            case NoticeType::PAUSE:
                std::cout <<unitName << " standing by.\n";
                break;

                        case NoticeType::SCHEDULE_CHANGE:
                std::cout <<unitName << " check-in time changed: "
                          << notice.getMessage() << "\n";
                break;

            case NoticeType::CAPACITY_ALERT:
                std::cout <<unitName << " CAPACITY ALERT: "
                          << "Max occupancy " << maxOccupancy << " reached.\n";
                break;
            default:
                std::cout << unitName << " noted: " 
                          << notice.getMessage() << "\n";
                break;
        }
    }

    /**
     * @brief Returns whether the tent is prepared for occupancy.
     *
     * @return true if the tent is prepared, false otherwise.
     */
    bool getIsPrepared() const {
        return isPrepared;
    }

    /**
     * @brief Returns whether the tent is currently occupied.
     *
     * @return true if the tent is occupied, false otherwise.
     */
    bool getIsOccupied() const {
        return isOccupied;
    }

    /**
     * @brief Sets the occupied status of the tent.
     *
     * @param occupied The new occupied status.
     */
    void setOccupied(bool occupied) {
        isOccupied = occupied;
    }

    /**
     * @brief Returns the maximum occupancy of this tent.
     *
     * @return int The maximum number of occupants.
     */
    int getMaxOccupancy() const {
        return maxOccupancy;
    }
};

#endif // TENT_H