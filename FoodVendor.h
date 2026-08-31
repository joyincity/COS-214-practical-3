/**
 * @file FoodVendor.h
 * @brief Defines a leaf in the Composite Pattern and an ConcreteObserver in the Observer pattern in the Mystifare fair.
 *
 * This file contains the FoodVendor class, which represents a food vendor
 * in the Composite tree. It is a Leaf that reacts to notices by suspending
 * or resuming service based on its outdoor seating status. So it is also an Observer in the Observer pattern.
 *
 * @note FoodVendor is a Concrete Observer and a Leaf in the Composite pattern.
 *
 * @author COS 214 group
 * @date 2026
 */
#ifndef FOODVENDOR_H
#define FOODVENDOR_H

#include "EventComponent.h"
#include "Observer.h"
#include "Control.h"
#include <iostream>

/**
 * @brief Concrete food vendor that sells food and drinks.
 *
 * This class represents a food vendor at the Mystifare fair. It is a Leaf
 * in the Composite pattern (cannot contain children) and a Concrete Observer
 * in the Observer pattern.
 *
 * **Reaction to notices:**
 * - `WEATHER_ALERT`: If outdoor seating exists, **SUSPENDS** service and closes.
 * - `RESUME`: Resumes normal service.
 * - `EVACUATE`: Evacuates customers and closes.
 * - `OPEN`: Opens for service.
 * - `CLOSE`: Closes for the day.
 * - `PAUSE`: Pauses service.
 * - `SCHEDULE_CHANGE`: Logs schedule updates.
 * - `CAPACITY_ALERT`: Logs capacity management.
 *
 * @note This is one of 5 concrete leaf types required by the specification.
 * @see EventComponent, Observer
 */

class FoodVendor : public EventComponent, public Observer {
private:
    /**
     * @brief The name of this vendor (e.g., "Dragon's Feast").
     */
    std::string unitName;

    /**
     * @brief The type of cuisine served (e.g., "Medieval Cuisine", "Italian").
     */
    std::string cuisineType;

    /**
     * @brief Whether this vendor has outdoor seating.
     *
     * Determines the reaction to WEATHER_ALERT. If true, the vendor
     * suspends service; if false, the vendor continues unaffected.
     */
    bool hasOutdoorSeating;

    /**
     * @brief Whether service is currently suspended.
     */
    bool isSuspended;

    /**
     * @brief Whether the vendor is currently open for service.
     */
    bool isOpen;

    /**
     * @brief Pointer to the Subject this observer is registered with.
     *
     * Stored for safe self-detachment in the destructor.
     */
    Control* registeredSubject;

    /**
     * @brief Vector containing all the allergens for a food
     */
    std::vector<std::string> allergens;  

    /** 
    @brief shows allergens depending on the cuisine chosen(if that cuisine has allergens)
    */
    bool displayAllergens;

public:
    /**
     * @brief Constructs a new FoodVendor.
     *
     * Initializes the vendor as open, not suspended, and not evacuated.
     *
     * @param name The display name of the vendor.
     * @param cuisine The type of cuisine served.
     * @param outdoor Whether the vendor has outdoor seating. Defaults to true.
     */
    FoodVendor(const std::string& name, const std::string& cuisine, bool outdoor = true)
        : unitName(name), cuisineType(cuisine), hasOutdoorSeating(outdoor),
          isSuspended(false), isOpen(true), registeredSubject(nullptr),displayAllergens(false)
    {
        if (cuisine == "Medieval Cuisine" || cuisine == "Italian") {
            allergens = {"gluten", "dairy"};
        } else if (cuisine == "Asian") {
            allergens = {"nuts", "soy"};
        } else {
            allergens = {"none"};
        }

    }

    /**
     * @brief Destructor that automatically detaches from the Subject.
     *
     * Prevents dangling pointers in the Subject's observer list.
     *
     * @note This is part of the safe destruction policy 
     * @warning Must be called before the Subject is destroyed.
     */
    virtual ~FoodVendor() {
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
     * @brief Opens the food vendor for service.
     *
     * Sets isOpen = true and resets suspended 
     *
     * @post Vendor is open and ready to serve customers.
     * @see close()
     */
    void open() override {
        isOpen = true;
        isSuspended = false;
        std::cout <<unitName << " (" << cuisineType 
                  << ") is now OPEN for service.\n";
    }
    
    /**
     * @brief Closes the food vendor.
     *
     * Sets isOpen = false.
     *
     * @post Vendor is closed and cannot serve customers.
     * @see open()
     */
    void close() override {
        isOpen = false;
        std::cout <<unitName << " (" << cuisineType 
                  << ") is now CLOSED.\n";
    }
    
    /**
     * @brief Reports the current status of the vendor.
     *
     * Outputs name, cuisine type, open status, suspended status,
     * evacuated status, and whether it has outdoor seating.
     *
     * @post No state changes occur (const operation).
     */
    void reportStatus() const override {
        std::cout << unitName << " [" << cuisineType << "] | "
                  << "Open: " << (isOpen ? "Yes" : "No")
                  << " | Suspended: " << (isSuspended ? "Yes" : "No")
                  << " | Outdoor: " << (hasOutdoorSeating ? "Yes" : "No") << "\n";
    }
    
    /**
     * @brief Returns the customer capacity of this vendor.
     *
     * Outdoor vendors have a capacity of 50, indoor vendors have 20.
     *
     * @return int The maximum number of customers this vendor can serve.
     */
    int getCapacity() const override { return hasOutdoorSeating ? 50 : 20; }

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
     * - `WEATHER_ALERT`: If outdoor, SUSPENDS service and closes.
     * - `EVACUATE`: Evacuates customers and closes.
     * - `OPEN`: Opens for service.
     * - `CLOSE`: Closes for the day.
     * - `PAUSE`: Pauses service.
     * - `RESUME`: Resumes normal service.
     * - `SCHEDULE_CHANGE`: Logs schedule updates.
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
                if (hasOutdoorSeating && isOpen) {
                    isSuspended = true;
                    std::cout <<unitName << " SUSPENDED outdoor service "
                              << "due to weather alert!\n";
                    close();
                } else if (!hasOutdoorSeating) {
                    std::cout <<unitName << " INDOOR - continuing service.\n";
                }
                break;

                case NoticeType::ALLERGEN_ALERT:  // New notice type
                    displayAllergens = true;
                    std::cout << "  🍽️ " << unitName << " DISPLAYING ALLERGENS: ";
                    for (const auto& a : allergens) {
                        std::cout << a << " ";
                    }
                    std::cout << "\n";
                    break;
                
            case NoticeType::RESUME:
                if (isSuspended) {
                    isSuspended = false;
                    isOpen = true;
                    std::cout <<unitName << " RESUMED normal service.\n";
                    open();
                }
                break;
            
            case NoticeType::OPEN:
                if (!isOpen) {
                    open();
                }
                break;

            case NoticeType::CLOSE:
                if (isOpen) {
                    std::cout <<unitName << " CLOSING for the day.\n";
                    close();
                }
                break;

            case NoticeType::PAUSE:
                if (isOpen) {
                    isSuspended = true;
                    std::cout <<unitName << " PAUSED service.\n";
                }
                break;
            
            case NoticeType::SCHEDULE_CHANGE:
                std::cout << unitName << " adjusting hours: "
                          << notice.getMessage() << "\n";
                break;

            case NoticeType::CAPACITY_ALERT:
                std::cout << unitName << " CAPACITY ALERT: "
                          << "Managing queue (Cap: " << getCapacity() << ")\n";
                break;
            default:
                std::cout <<unitName << " noted: " 
                          << notice.getMessage() << "\n";
                break;
        }
    }
};

#endif // FOODVENDOR_H