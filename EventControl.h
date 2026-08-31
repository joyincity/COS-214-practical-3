/**
 * @file EventControl.h
 * @brief Defines the concrete Subbject for the Mystifare control centre
 * 
 * This file contains the EventControl class, which is the ConcrETESubject
 * inn the Observer patter. It represents the main coordination hub for the Mystifare fair, issuing
 * fair-wide notics to all registered observers
 * 
 * @note EventControl is NOT part of the Composite tree. It exists separatey as a top level coordinator for the entire event
 * 
 * @author COS 214 group
 * @date 2026
 */
#ifndef EVENTCONTROL_H
#define EVENTCONTROL_H

#include "Control.h"
#include <iostream>

/**
 * @brief Concrete Subject that represents the Mystifare control centre
 * 
 * This class extends Control to provide a concrete implementation of the SubjEct role in the 
 * Observer pattern. It serves as the central command point for the entire fair, issuing
 * notices such as WEATHER_ALERT,CAPACITY_ALERT,EVACUATE,etc.,to all registered zones and observers.
 * 
 * @note EventControl is not part of the Composite pattern-it does not 
 * contain EventComponents. It ony holds Observer pointers
 * 
 * @see Control(Base Subject),Observer,EventZone
 */
class EventControl:public Control{
    private:
    /**
     * @brief The display name of this control centre
     * 
     * Used for logging and identification in console output
     */
    std::string controlName;

    public:
    /** 
    @brief Constructs a new EventControl with a given name.
    
    @param name The display name for thus control centre. Defaults to "Main Control".
    */
    EventControl(const std::string& name="Main Control"):controlName(name){}

    /**
     * @brief Virtual destructor for polymorphic deletion
     * 
     * @note Does not delete any observers(non-owning pointers). Observers must detach themselves in their
     * destructors.
     */
    virtual ~EventControl()=default;

    /**
     * @brief Stores a new notice and triggers notification to all observers.
     * 
     * This is the core method that drives the PULL approach:
     * 1.Stores the notice in currentNotice
     * 2.Sets hasNoTICE=TRUE
     * 3.Calls notify() to broadcast to all registered observers.
     * 
     * @param notice the Notice to store and broadcast
     * 
     * @note This method is protected so derived classes can call it 
     * @see  notify(),getCurrentNotice
     */
    void setEvent(const Notice& notice) override{
        std::cout<<"\n["<<controlName<<"] Issuing: "<<notice.getMessage()<<"\n";
        Control::setEvent(notice);
    }
    /**
     * @brief Notifies all registered observers.
     *
     * Overrides Control::notify() to add custom logging.
     * Calls update(this) on every registered observer, passing a pointer
     * to this Subject so observers can PULL the current notice data.
     *
     * @post Every registered observer has had update(this) called.
     *
     * @note This uses the PULL approach: observers call getCurrentNotice()
     *       to retrieve the notification data.
     *
     * @see Control::notify(), Observer::update()
     */
    void notify() override{
        std::cout<<" Notifying "<<observers.size()<<" registered observers...\n";
        Control::notify();
    }
    /**
     * @brief Returns the name of the control centre.
     *
     * Used for logging and identification.
     *
     * @return const std::string& Reference to the control centre's name.
     */
    const std::string& getName() const{return controlName;}
};
#endif //EVENTCONTROL_H