/**
 * @file VipPass.h
 * @brief Defines a VIP Pass feature for the Mystifare fair.
 *
 * This file contains the VipPass class, which represents a VIP pass
 * system for the Mystifare fair. It is an original feature (Task 4.4)
 * that grants priority access to attendees based on their priority level.
 *
 * @note This is one of the 3 original features required by Task 4.4.
 * @note This class is NOT part of the Composite tree—it is a standalone
 *       feature that observes the control centre.
 *
 * @author Your Team Name
 * @date 2026
 */
#ifndef VIPPASS_H
#define VIPPASS_H

#include "EventComponent.h"
#include "Observer.h"
#include "Control.h"
#include "Notice.h"
#include <iostream>

/**
 * @brief Concrete Observer that represents a VIP Pass system.
 *
 * This class simulates a VIP pass system at the Mystifare fair.
 * VIP passes have priority levels (1–5) that grant holders priority
 * entry during capacity alerts and special treatment during evacuations.
 *
 * **Reaction to notices:**
 * - `CAPACITY_ALERT`: Grants priority entry to the VIP holder.
 * - `EVACUATE`: Redirects the VIP to the VIP lounge.
 * - `OPEN`: Activates the VIP pass.
 * - `CLOSE`: Deactivates the VIP pass.
 *
 * **Original Feature:** This class demonstrates an interactive perk
 * system that responds to event-wide notices without being part of
 * the Composite tree, showcasing how the Observer pattern can be used
 * for non-structural features.
 *
 * @note This is a Concrete Observer that is NOT part of the Composite tree.
 * @note This is one of the 3 original features required by Task 4.4.
 * @see EventComponent, Observer
 */
class VipPass : public EventComponent, public Observer {
private:
    /**
     * @brief The name of the VIP pass holder (e.g., "King Arthur").
     */
    std::string passHolderName;

    /**
     * @brief Whether the VIP pass is currently active.
     */
    bool isActive;

    /**
     * @brief The priority level of this VIP pass (1–5).
     *
     * Higher priority gives greater access privileges.
     */
    int priorityLevel; 

    /**
     * @brief Pointer to the Subject this observer is registered with.
     *
     * Stored for safe self-detachment in the destructor.
     */
    Control* registeredSubject;

public:

    /**
     * @brief Constructs a new VIP Pass.
     *
     * Initializes the pass as active with the given priority level.
     *
     * @param name The name of the VIP pass holder.
     * @param priority The priority level (1–5). Defaults to 3.
     */
    VipPass(const std::string& name, int priority = 3) 
        : passHolderName(name), isActive(true), priorityLevel(priority), registeredSubject(nullptr) {}

     /**
     * @brief Destructor that automatically detaches from the Subject.
     *
     * Prevents dangling pointers in the Subject's observer list.
     *
     * @note This is part of the safe destruction policy (Task 3.2).
     * @warning Must be called before the Subject is destroyed.
     */
    virtual ~VipPass() {
        if (registeredSubject) {
            registeredSubject->detach(this);
            registeredSubject = nullptr;
        }
    }

    /**
     * @brief Updates the internal Subject pointer for safe self-detachment.
     *
     * Used by the transfer mechanism (if implemented) to update the
     * registeredSubject pointer.
     *
     * @param subject The new Subject this VIP pass should observe.
     */
    void setObservedSubject(Control* subject) override {
        registeredSubject = subject;
    }

    /**
     * @brief Activates the VIP pass.
     *
     * Sets the pass to active and prints an activation message.
     *
     * @post VIP pass is active and can be used.
     * @see close()
     */
    void open() override {
        isActive = true;
        std::cout << " VIP Pass for " << passHolderName << " ACTIVATED (Priority: " 
                  << priorityLevel << ")\n";
    }

    /**
     * @brief Deactivates the VIP pass.
     *
     * Sets the pass to inactive.
     *
     * @post VIP pass is inactive and cannot be used.
     * @see open()
     */
    void close() override {
        isActive = false;
        std::cout << "VIP Pass for " << passHolderName << " DEACTIVATED.\n";
    }

        /**
     * @brief Reports the current status of the VIP pass.
     *
     * Outputs the pass holder name, active status, and priority level.
     *
     * @post No state changes occur (const operation).
     */
    void reportStatus() const override {
        std::cout <<  passHolderName << " | Active: " << (isActive ? "Yes" : "No")
                  << " | Priority: " << priorityLevel << "\n";
    }

       /**
     * @brief Returns the capacity of this VIP pass.
     *
     * @return int Always 1 (one VIP holder).
     */
    int getCapacity() const override { return 1; }

      /**
     * @brief Returns the name of this VIP pass.
     *
     * @return std::string The VIP pass holder's name prefixed with "VIP: ".
     */
    std::string getName() const override { return "VIP: " + passHolderName; }

       /**
     * @brief Registers this VIP pass as an observer of a Subject.
     *
     * Stores the Subject pointer for safe self-detachment in the destructor,
     * and calls subject->attach(this) to add this VIP pass to the Subject's
     * observer list.
     *
     * @param subject The Subject to observe.
     * @pre subject must not be nullptr.
     * @post This VIP pass is registered with the Subject.
     *
     * @see Observer::update()
     */
    void registerWithSubject(Control* subject) {
        if (subject == nullptr) return;
        registeredSubject = subject;
        subject->attach(this);
    }

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
     * - `CAPACITY_ALERT`: Grants priority entry to the VIP holder.
     * - `EVACUATE`: Redirects the VIP to the VIP lounge.
     * - `OPEN`: Activates the VIP pass.
     * - `CLOSE`: Deactivates the VIP pass.
     *
     * @param subject The Subject that issued the notification.
     *
     * @note This is a Concrete Observer implementation.
     * @note This is one of the 3 original features (Task 4.4).
     * @see Control::getCurrentNotice()
     */
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

     /**
     * @brief Returns whether the VIP pass is active.
     *
     * @return true if the pass is active, false otherwise.
     */
    bool getIsActive() const {
        return isActive;
    }

    /**
     * @brief Returns the priority level of this VIP pass.
     *
     * @return int The priority level (1–5).
     */
    int getPriorityLevel() const {
        return priorityLevel;
    }

    /**
     * @brief Returns the name of the VIP pass holder.
     *
     * @return std::string The holder's name.
     */
    std::string getPassHolderName() const {
        return passHolderName;
    }
};

#endif // VIPPASS_H