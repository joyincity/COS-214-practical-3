/**
 * @file EventComponent.h
 * @brief Defines the abstract base cllass for all event components
 * 
 * This file contains the EventComponent abstract  class,which serves as
 * the Component interface in the Composite pattern. All concrete leaves and composites in Mystifare
 * fair hierarchy inherit from this class
 * 
 * @note This is teh root of the Composite pattern tree
 * 
 * @warning This is an abstract class and cannot be instantitated directly.All methods are pure virtual and must be overriden
 * 
 * @author COS 214 group
 * @date 2026
 */
#ifndef EVENTCOMPONENT_H
#define EVENTCOMPONENT_H

#include <iostream>
#include <string>

/**
 * @brief Forward declaration of the Control(Subject) class
 * 
 * This is required for the setObservedSubject() method,which allows components to update their internal
 * Subject pointer during transfers
 */
class Control;

/**
 * @brief Abstract base class for all event components in the Composite pattern.
 * 
 * This class defines the common interface for both leaves and composites
 * in the Mystifaree fair event structure.Every part of the event tree
 * (e.g. Performance,TheWatch,EventZone) inherits from this class.
 * 
 * The Composite pattern allows clients to treat individual objects(leaves)
 * and compositions(composites) uniformly
 * 
 * @note All concrete subclasses must implement all pure virtual methodds.
 * @see EventZone(Composite),Performance(Leaf)
 */
class EventComponent{
    public:
    /**
     * @brief Virtual destructor for polymorphic deletion
     * 
     * Ensures that the derived class destructors are called
     * when deleting an object through a base class pointer.
     */
    virtual ~EventComponent()=default;

    /**
     * @brief Opens the event component and all its children(if any).
     * 
     * For leaves: Opens the specific unit (e.g. starts a performance,
     * opens a vendor,activates security).
     * For composite: Opens itself and recursively opens all children
     * 
     * @post The component is in an open/active state
     * @see close()
     */
    virtual void open()=0;

    /**
     * @brief Closes the event component and all its children(if any).
     * 
     * For leaves: Closes tthe specific unit(e.g. ends a performance, closes a vendor,deactivates security).
     * For composites: Closes itself and recursively closes all children
     * 
     * @post The component is in a closed/inactive state
     * @see open()
     */
    virtual void close()=0;
    /**
     * @brief Reports theh current status of the component to the console
     * 
     * Outputs details such as:
     * -Name and type of the component
     * -Whether it is open/active
     * -State specific information(e.g.,paused,suspended,capacity).
     * For composites: Reports its own status and recursively reports the status of all children
     * 
     * @post No state changes occur-this is a const(read only) operation
     * @see getCapacity(),getName()
     */
    virtual void reportStatus() const=0;
    /**
     * @brief returns the capacity of this event component
     * 
     * For leaves: Returns the unit's capacity(e.g.,500 for outdoor stage,50 for a vendor,2 for a tent).
     * For composites: Returns the sum of the capacities of the children
     * 
     * @return int The total capacity of this component
     * 
     * @see reportStatus()
     */
    virtual int getCapacity() const=0;
    /**
     * @brief returns the name/identified for this component
     * 
     * Used for logging,status reports and debugging
     * 
     * @return std::string The name of this component(e.g."North Zone")
     */
    virtual std::string getName() const=0;
    /**
     * @brief Updates the internal Subject pointer for safe self detachment.
     * 
     * This method is used exclusively by the transfer mechanism
     * to update the component's interna subject pointer when it is moved to a new
     * parent. This ensures that the destructor detaches from the correct Subject
     * 

     @param subject the Subject(parent) this component should observe.
     @note The default implementation is empty derived classes that observe
     a Subject(all leaves and EventZone) must override this to store the pointer
     @see EventZone::transfer()
     */
     virtual void setObservedSubject(Control* subject) = 0;
};

#endif //EVENTCOMPONENT_H