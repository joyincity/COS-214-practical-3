/**
 * @file Performance.h
 * @brief Defines a Leaf(Composite) and Observer(Observer) in the Mystifare fair.
 *
 * This file contains the Performance class, which represents a scheduled
 * musical or theatrical performance at the Mystifare fair. It is a Leaf
 * in the Composite tree that reacts to notices by pausing, resuming, or
 * evacuating based on its outdoor status.
 *
 * @note This is one of 5 concrete leaf types required by the specification.
 *
 * @author COS214 group
 * @date 2026
 */
#ifndef PERFORMANCE_H
#define  PERFORMANCE_H

#include "EventComponent.h"
#include "Observer.h"
#include "Control.h"
#include "Notice.h"
#include <iostream>

/**
 * @brief Concrete performance (play or music) that can be paused/resumed.
 *
 * This class represents a scheduled performance at the Mystifare fair.
 * It is a Leaf in the Composite pattern (cannot contain children) and
 * a Concrete Observer in the Observer pattern.
 *
 * **Reaction to notices:**
 * - `WEATHER_ALERT`: If outdoor, **PAUSES** the show and closes the stage.
 * - `EVACUATE`: Evacuates attendees and closes.
 * - `OPEN`: Opens the performance.
 * - `CLOSE`: Closes the performance.
 * - `PAUSE`: Pauses the performance temporarily.
 * - `RESUME`: Resumes the performance if paused or evacuated.
 * - `SCHEDULE_CHANGE`: Logs schedule updates.
 * - `CAPACITY_ALERT`: Logs capacity management.
 *
 * @note This is one of 5 concrete leaf types required by the specification.
 * @note Demonstrates condition-based decision: `if (isOutdoor && isActive)`
 *       (Task 4.3).
 * @see EventComponent, Observer
 */
class Performance:public EventComponent,public Observer{
    private:
    /**
     * @brief The name of this performance unit (e.g., "Main Stage").
     */
    std::string unitName;

    /**
     * @brief The title of the show (e.g., "Romeo & Juliet").
     */
    std::string showTitle;

    /**
     * @brief Whether this performance is located outdoors.
     *
     * Determines the reaction to WEATHER_ALERT. If true, the performance
     * pauses; if false, the performance continues unaffected.
     */
    bool isOutdoor;

    /**
     * @brief Whether the performance is currently paused.
     */
    bool isPaused;

    /**
     * @brief Whether the performance is currently active/open.
     */
    bool isActive;

    /**
     * @brief Whether the performance has been evacuated.
     */
    bool isEvacuated;

    /**
     * @brief Pointer to the Subject this observer is registered with.
     *
     * Stored for safe self-detachment in the destructor.
     */
    Control* registeredSubject;

    public:
    /**
     * @brief Constructs a new Performance.
     *
     * Initializes the performance as active, not paused, and not evacuated.
     *
     * @param name The display name of the performance unit.
     * @param title The title of the show.
     * @param outdoor Whether the performance is located outdoors. Defaults to true.
     */
    Performance(const std::string& name,const std::string& title,bool outdoor=true): unitName(name),showTitle(title),isOutdoor(outdoor),isPaused(false),isActive(true),registeredSubject(nullptr){}

    /**
     * @brief Destructor that automatically detaches from the Subject.
     *
     * Prevents dangling pointers in the Subject's observer list.
     *
     * @note This is part of the safe destruction policy (Task 3.2).
     * @warning Must be called before the Subject is destroyed.
     */
    virtual ~Performance(){
        if(registeredSubject!=nullptr){
            registeredSubject->detach(this);
            registeredSubject=nullptr;
        }
    }

    /**
     * @brief Updates the internal Subject pointer for safe self-detachment.
     *
     * Used exclusively by the transfer mechanism (EventZone::transfer())
     * to update the registeredSubject pointer when this performance is moved
     * to a new parent zone.
     *
     * @param subject The new Subject this performance should observe.
     *
     * @see EventZone::transfer()
     */
    void setObservedSubject(Control* subject) override {
        registeredSubject = subject;
    }

    /**
     * @brief Opens the performance.
     *
     * Sets the performance to active and resets paused and evacuated flags.
     *
     * @post Performance is active and ready to begin.
     * @see close()
     */
    void open() override{
        isActive=true;
        isPaused=false;
        std::cout<<unitName<<" ("<<showTitle<<") is now OPEN.\n";
    }

    /**
     * @brief Closes the performance.
     *
     * Sets the performance to inactive.
     *
     * @post Performance is closed and no longer active.
     * @see open()
     */
    void close() override{
        isActive=false;
        std::cout<<unitName<<"("<<showTitle<<" )is now CLOSED.\n";
    }

    /**
     * @brief Reports the current status of the performance.
     *
     * Outputs name, show title, active status, paused status,
     * evacuated status, and whether it is located outdoors.
     *
     * @post No state changes occur (const operation).
     */
    void reportStatus() const override{
        std::cout<<unitName<<" ["<<showTitle<<"] | "<<"Active: "<<(isActive?"Yes":"No")<<" | Paused: "<<(isPaused?"Yes":"No")<<" | Outdoor: "<<(isOutdoor?"Yes":"No")<<"\n";
    }

    /**
     * @brief Returns the audience capacity of this performance.
     *
     * Outdoor stages have a capacity of 500, indoor stages have 200.
     *
     * @return int The maximum number of attendees this performance can hold.
     */
    int getCapacity() const override{
        return isOutdoor?500:200;
    }

    /**
     * @brief Returns the name of this performance unit.
     *
     * @return std::string The performance's display name.
     */
    std::string getName() const override{return unitName;}

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
     * - `WEATHER_ALERT`: If outdoor, **PAUSES** the show and closes.
     * - `EVACUATE`: Evacuates attendees and closes.
     * - `OPEN`: Opens the performance.
     * - `CLOSE`: Closes the performance.
     * - `PAUSE`: Pauses the performance temporarily.
     * - `RESUME`: Resumes the performance if paused or evacuated.
     * - `SCHEDULE_CHANGE`: Logs schedule updates.
     * - `CAPACITY_ALERT`: Logs capacity management.
     *
     * @param subject The Subject that issued the notification.
     *
     * @note This is a Concrete Observer implementation.
     * @note Demonstrates condition-based decision: `if (isOutdoor && isActive)`
     *       (Task 4.3).
     * @see Control::getCurrentNotice()
     */
    void update(Control* subject)override{
        if(registeredSubject==nullptr){
            registeredSubject=subject;
        }

        if(!subject->hasCurrentNotice())return;

        const Notice& notice=subject->getCurrentNotice();

        switch(notice.getType()){
            /*If outdoors and ongoing then pause and close*/
            case NoticeType::WEATHER_ALERT:
                if(isOutdoor && isActive){
                    isPaused=true;
                    std::cout<<unitName<<" ("<<showTitle<<" ) PAUSED due to weather alert!\n";
                    close();            
                }
                else if(!isOutdoor){
                    std::cout<<unitName<<" ("<<showTitle<<") INDOOR-continuing unaffected.\n";
                }
                break;
            /*If resume and was paused resume*/
            case NoticeType::RESUME:
                if(isPaused || isEvacuated){
                    isPaused=false;
                    isEvacuated=false;
                    isActive=true;
                    std::cout<<unitName<<" ("<<showTitle<<") RESUMED!\n";
                    open();
                }
                break;
            /*if Evacuate then no longer active*/
            case NoticeType::EVACUATE:
                if(isActive){
                    isActive=false;
                    isEvacuated=true;
                    std::cout<<unitName<<" ("<<showTitle<<") EVACUTAED!\n";
                }
                break;
            case NoticeType::OPEN:
                if(!isActive){
                    open();
                }
                break;
            case NoticeType::CLOSE:
                if(isActive){
                    std::cout<<unitName<<" ("<<showTitle<<") CLOSING for the day.\n";
                    close();
                }
                break;
            case NoticeType::PAUSE:
                if(isActive && !isPaused){
                    isPaused=true;
                    std::cout<<unitName<<" ("<<showTitle<<") PAUSED (temporary).\n";
                }
                break;
             case NoticeType::SCHEDULE_CHANGE:
                std::cout <<unitName << " (" << showTitle
                          << ") SCHEDULE CHANGED: " << notice.getMessage() << "\n";
                break;
            case NoticeType::CAPACITY_ALERT:
                std::cout << unitName << " (" << showTitle
                          << ") CAPACITY ALERT: " << notice.getMessage()
                          << " (Current cap: " << getCapacity() << ")\n";
                break;

            default:
                std::cout <<unitName << " noted: "
                          << notice.getMessage() << "\n";
                break;
        }
    }

     /**
     * @brief Registers this performance as an observer of a Subject.
     *
     * Stores the Subject pointer for safe self-detachment in the destructor,
     * and calls subject->attach(this) to add this performance to the
     * Subject's observer list.
     *
     * @param subject The Subject to observe.
     * @pre subject must not be nullptr.
     * @post This performance is registered with the Subject.
     *
     * @see Observer::update()
     */
    void registerWithSubject(Control* subject) {
        if (subject == nullptr) return;
        registeredSubject = subject;
        subject->attach(this);
    }

    // ==========================================
    // Query Methods
    // ==========================================

    /**
     * @brief Returns whether the performance is currently paused.
     *
     * @return true if the performance is paused, false otherwise.
     */
    bool getIsPaused() const {
        return isPaused;
    }

    /**
     * @brief Returns whether the performance is currently active.
     *
     * @return true if the performance is active/open, false otherwise.
     */
    bool getIsActive() const {
        return isActive;
    }

    /**
     * @brief Returns whether the performance is located outdoors.
     *
     * @return true if the performance is outdoors, false otherwise.
     */
    bool getIsOutdoor() const {
        return isOutdoor;
    }

    /**
     * @brief Returns the title of the show.
     *
     * @return std::string The show title.
     */
    std::string getShowTitle() const {
        return showTitle;
    }


};  
#endif //PERFORMANCE_H