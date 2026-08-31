/**
 * @file TheWatch.h
 * @brief Defines a concrete security team leaf in the Mystifare fair.
 *
 * This file contains the TheWatch class, which represents a security team
 * ("The Watch") at the Mystifare fair. It is a Leaf in the Composite tree
 * that reacts to notices by remaining active during emergencies and
 * redirecting attendees to safety.
 *
 * @note This is one of 5 concrete leaf types required by the specification.
 *
 * @author COS 214 group
 * @date 2026
 */

#ifndef THEWATCH_H
#define THEWATCH_H

#include "EventComponent.h"
#include "Observer.h"
#include "Control.h"
#include <iostream>

/**
 * @brief Concrete security team that remains active during emergencies.
 *
 * This class represents a security detail ("The Watch") at the Mystifare fair.
 * It is a Leaf in the Composite pattern (cannot contain children) and a
 * Concrete Observer in the Observer pattern.
 *
 * **Reaction to notices:**
 * - `WEATHER_ALERT`: **REMAINS ACTIVE** and redirects attendees to shelter.
 * - `EVACUATE`: Guides attendees to exits.
 * - `OPEN`: Goes on duty.
 * - `CLOSE`: Goes off duty.
 * - `PAUSE`: Stands by.
 * - `RESUME`: Resumes normal patrol.
 * - `SCHEDULE_CHANGE`: Adjusts patrol schedule.
 * - `CAPACITY_ALERT`: Prepares crowd control.
 *
 * @note This is one of 5 concrete leaf types required by the specification.
 * @note Demonstrates different behaviour from other leaves (stays active
 *       during WEATHER_ALERT instead of closing).
 * @see EventComponent, Observer
 */

class TheWatch:public EventComponent,public Observer{
    private:
    /**
     * @brief The name of this security team (e.g., "Watch Team Alpha").
     */
    std::string unitName;

    /**
     * @brief The number of guards in this team.
     */
    int teamSize;

    /**
     * @brief Whether the team is currently on duty.
     */
    bool onDuty;

    /**
     * @brief Whether the team is currently redirecting attendees.
     */
    bool redirecting;

    /**
     * @brief Pointer to the Subject this observer is registered with.
     *
     * Stored for safe self-detachment in the destructor.
     */
    Control* registeredSubject;

    public:
    /**
     * @brief Constructs a new security team.
     *
     * Initializes the team as on duty, not redirecting, and not guiding evacuation.
     *
     * @param name The display name of the security team.
     * @param size The number of guards in the team. Defaults to 5.
     */
    TheWatch(const std::string& name,int size=5):unitName(name),teamSize(size),onDuty(true),redirecting(false),registeredSubject(nullptr){}

    /**
     * @brief Destructor that automatically detaches from the Subject.
     *
     * Prevents dangling pointers in the Subject's observer list.
     *
     * @note This is part of the safe destruction policy (Task 3.2).
     * @warning Must be called before the Subject is destroyed.
     */
    virtual ~TheWatch(){
        if(registeredSubject){
            registeredSubject->detach(this);
            registeredSubject=nullptr;
        }
    }

    /**
     * @brief Updates the internal Subject pointer for safe self-detachment.
     *
     * Used exclusively by the transfer mechanism (EventZone::transfer())
     * to update the registeredSubject pointer when this team is moved
     * to a new parent zone.
     *
     * @param subject The new Subject this team should observe.
     *
     * @see EventZone::transfer()
     */
    void setObservedSubject(Control* subject) override {
        registeredSubject = subject;
    }

    /**
     * @brief Puts the security team on duty.
     *
     * Sets the team to active and resets redirecting and evacuation flags.
     *
     * @post Team is on duty and ready to respond.
     * @see close()
     */
    void open() override{
        onDuty=true;
        redirecting=false;
        std::cout<<unitName<<" (Team of "<<teamSize<<") is ON DUTY.\n";
    }

    /**
     * @brief Takes the security team off duty.
     *
     * Sets onDuty to false.
     *
     * @post Team is off duty and not responding.
     * @see open()
     */
    void close() override{
        onDuty=false;
        std::cout<<unitName<<" is OFF DUTY.\n";
    }

    /**
     * @brief Reports the current status of the security team.
     *
     * Outputs name, team size, on-duty status, redirecting status,
     * and evacuation guiding status.
     *
     * @post No state changes occur (const operation).
     */
    void reportStatus() const override {
        std::cout <<unitName << " | Team: " << teamSize
                  << " | On Duty: " << (onDuty ? "Yes" : "No")
                  << " | Redirecting: " << (redirecting ? "Yes" : "No") << "\n";
    }

    /**
     * @brief Returns the crowd management capacity of this team.
     *
     * Each guard can manage approximately 100 attendees.
     *
     * @return int The total capacity = teamSize * 100.
     */
    int getCapacity() const override { return teamSize * 100; }

    /**
     * @brief Returns the name of this security team.
     *
     * @return std::string The team's display name.
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
     * - `WEATHER_ALERT`: **REMAINS ACTIVE** and redirects attendees.
     * - `EVACUATE`: Guides attendees to exits.
     * - `OPEN`: Goes on duty.
     * - `CLOSE`: Goes off duty.
     * - `PAUSE`: Stands by.
     * - `RESUME`: Resumes normal patrol.
     * - `SCHEDULE_CHANGE`: Adjusts patrol schedule.
     * - `CAPACITY_ALERT`: Prepares crowd control.
     *
     * @param subject The Subject that issued the notification.
     *
     * @note This is a Concrete Observer implementation.
     * @note TheWatch is the only leaf that REMAINS ACTIVE during
     *       WEATHER_ALERT (does not close).
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

     /**
     * @brief Returns whether the team is on duty.
     *
     * @return true if the team is on duty, false otherwise.
     */
    bool getIsOnDuty() const {
        return onDuty;
    }

    /**
     * @brief Returns whether the team is redirecting attendees.
     *
     * @return true if the team is redirecting, false otherwise.
     */
    bool getIsRedirecting() const {
        return redirecting;
    }

    /**
     * @brief Returns the number of guards in this team.
     *
     * @return int The team size.
     */
    int getTeamSize() const {
        return teamSize;
    }
    
};




#endif //THEWATCH_H