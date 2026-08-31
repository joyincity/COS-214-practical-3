
/**
 * @file EventZone.h
 * @brief Defines the Composite class for the Mystifare event structure. It is also the
 * Subject and Observer in teh Observer design pattern
 *
 * This file contains the EventZone class, which serves as the
 * Composite in the Composite pattern. It can contain both leaves and other
 * composites, and participates in the Observer pattern as both an Observer
 * (receives notices from its parent) and a Subject (notifies its children).
 *
 * @note This is the ONLY Composite class in the system. Different instances
 *       with different names and children create the full event hierarchy.
 *
 * @author COS 214 group
 * @date 2026
 */
#ifndef EVENTZONE_H
#define EVENTZONE_H

#include "EventComponent.h"
#include "Observer.h"
#include "Control.h"
#include "Notice.h"
#include <vector>
#include <algorithm>
#include <iostream>

/**
 * @brief Composite class that can contain any EventComponent.
 *
 * This class participates in BOTH the Composite and Observer patterns:
 * - **Composite**: Contains children (Leaves or other EventZones).
 * - **Observer**: Listens to its parent Subject for notifications.
 * - **Subject**: Notifies its children of cascading alerts.
 *
 * This is the "bridge" class that enables cascading notifications through
 * multiple runtime levels (e.g., Control → NorthZone → MainStage → Leaf).
 *
 * @note EventZone owns its children—they are deleted in the destructor.
 * @warning Observers are stored as non-owning pointers. Children must
 *          detach themselves in their destructors if they observe this zone.
 *
 * @see EventComponent, Observer, Control
 */

class EventZone: public EventComponent,public Observer,public Control{
    private:
    /**
     * @brief The name of this zone (e.g., "North Zone", "Main Stage").
     */
    std::string zoneName;
    /**
     * @brief Vector of owned children (Leaves or other EventZones).
     *
     * This is the Composite pattern's child container. The EventZone
     * owns these pointers and deletes them in its destructor.
     */
    std::vector<EventComponent*> children;
    /**
     * @brief Whether this zone is currently open/active.
     */
    bool isOpen;
    /**
     * @brief Pointer to the parent Subject this zone observes.
     *
     * Stored so that the destructor can detach this zone from its parent.
     * This prevents dangling pointers when this zone is destroyed.
     */
    Control* parentSubject;//Subject this zone observes for self detach

    public:
        /**
     * @brief Constructs a new EventZone with the given name.
     *
     * Initializes the zone as open with no children and no parent subject.
     *
     * @param name The display name for this zone.
     */
    EventZone(const std::string& name):zoneName(name),isOpen(true),parentSubject(nullptr){}

    /**
     * @brief Destroys the EventZone and all owned children.
     *
     * 1. Detaches this zone from its parent Subject (if registered).
     * 2. Deletes all child components (ownership).
     *
     * @warning All children are deleted recursively. Ensure no other
     *          pointers to children exist outside this tree.
     */
    virtual ~EventZone(){
        if(parentSubject!=nullptr){
            parentSubject->detach(this);
            parentSubject=nullptr;
        }

        for(EventComponent* child:children){
            delete child;
        }
        children.clear();
    }

    /**
     * @brief Public wrapper to issue a notice from this zone.
     *
     * Allows external code to trigger notifications from this zone
     * (e.g., for testing or when this zone is the root of a cascade).
     *
     * @param notice The notice to issue.
     *
     * @note This is a public wrapper for the protected setEvent() method.
     * @see Control::setEvent()
     */
    void issueNotice(const Notice& notice) {
        setEvent(notice);
    }

    /** 
    * @brief Updates the internal parent Subject pointer.
     *
     * Used exclusively by the transfer mechanism (EventZone::transfer())
     * to update this zone's parentSubject pointer when it is moved to a
     * new parent. This ensures the destructor detaches from the correct Subject.
     *
     * @param subject The new Subject (parent) this zone should observe.
     *
     * @note This method is required by the EventComponent interface.
     * @see EventZone::transfer()
     */
    void setObservedSubject(Control* subject) override {
        parentSubject = subject;
    }

    /**
     * @brief Adds a child component to this zone.
     *
     * The zone takes ownership of the child. The child will be deleted
     * when this zone is destroyed.
     *
     * @param component Pointer to the component to add.
     * @pre component must not be nullptr.
     * @post component is now owned by this zone.
     *
     * @note Duplicate children are allowed (no uniqueness check).
     * @see remove()
     */
    void add(EventComponent* component){
        children.push_back(component);
    }

    
    /**
     * @brief Removes a child component from this zone (releases ownership).
     *
     * This method does NOT delete the child. It only removes the pointer
     * from the children vector, releasing ownership.
     *
     * @param component Pointer to the component to remove.
     * @pre component must be a child of this zone.
     * @post component is no longer owned by this zone.
     *
     * @note Use this method for transfers (Task 4.2). The caller must
     *       ensure the child is re-added to another zone or manually deleted.
     * @warning If the child is not re-added elsewhere, it will leak.
     *
     * @see add(), EventZone::transfer()
     */
    void remove(EventComponent* component){
        auto it=std::find(children.begin(),children.end(),component);
        if(it!=children.end()){
            children.erase(it);
        }
    }

    /**
     * @brief Registers this zone as an observer of a parent Subject.
     *
     * Stores the parent Subject pointer for safe self-detachment in the
     * destructor, and calls parent->attach(this) to add this zone to
     * the parent's observer list.
     *
     * @param subject The parent Subject to observe.
     * @pre subject must not be nullptr.
     * @post This zone is registered with the parent Subject.
     *
     * @note This is the "bridge" method that connects the Composite to
     *       its parent for cascading notifications.
     *
     * @see Observer::update(), detach()
     */
    void registerWithSubject(Control* subject){
        if(subject==nullptr) return;
        parentSubject=subject;
        subject->attach(this);
    }

    /**
     * @brief Opens this zone and all its children recursively.
     *
     * Sets isOpen = true, then calls open() on every child component.
     *
     * @post This zone and all children are open.
     * @see close()
     */
    void open() override {
        isOpen = true;
        std::cout <<zoneName << " is OPEN.\n";
        for (EventComponent* child : children) {
            child->open();
        }
    }

    /**
     * @brief Closes this zone and all its children recursively.
     *
     * Sets isOpen = false, then calls close() on every child component.
     *
     * @post This zone and all children are closed.
     * @see open()
     */
    void close() override {
        isOpen = false;
        std::cout <<zoneName << " is CLOSED.\n";
        for (EventComponent* child : children) {
            child->close();
        }
    }

    /**
     * @brief Reports the status of this zone and all its children recursively.
     *
     * Outputs the zone's name and open status, then calls reportStatus()
     * on every child component.
     *
     * @post No state changes occur (const operation).
     * @see getCapacity(), getName()
     */

    void reportStatus() const override {
        std::cout << zoneName << " | Open: " 
                  << (isOpen ? "Yes" : "No")
                  << " | Children: " << children.size() << "\n";
        for (EventComponent* child : children) {
            child->reportStatus();
        }
    }

    /**
     * @brief Returns the total capacity of this zone.
     *
     * Sums the capacity of all children components.
     *
     * @return int The sum of all children's capacities.
     *
     * @see EventComponent::getCapacity()
     */
    int getCapacity() const override {
        int total = 0;
        for (EventComponent* child : children) {
            total += child->getCapacity();
        }
        return total;
    }

    /**
     * @brief Returns the name of this zone.
     *
     * @return std::string The zone's name.
     */
    std::string getName() const override { return zoneName; }

    /**
     * @brief Called by the parent Subject when a new notice is available.
     *
     * This is the entry point for cascading notifications. This zone:
     * 1. Stores the parent Subject pointer (if not already stored).
     * 2. Pulls the notice data via subject->getCurrentNotice().
     * 3. Logs the receipt of the notice.
     * 4. Cascades the notice to all child observers via setEvent(notice).
     * 5. Applies zone-specific behaviour (e.g., close() on CLOSE notice).
     *
     * @param subject The Subject that issued the notification.
     *
     * @note This method uses the PULL approach: the zone pulls the
     *       notice data from the Subject.
     * @see Control::setEvent(), Control::notify()
     */
    void update(Control* subject) override {
        if (parentSubject == nullptr) {
            parentSubject = subject;
        }

        if (!subject->hasCurrentNotice()) return;

        const Notice& notice = subject->getCurrentNotice();

        std::cout << "\n [" << zoneName << "] Received notice from parent: "
                  << notice.getMessage() << "\n";

        std::cout << " [" << zoneName << "] Cascading to " 
                  << getObserverCount() << " child observers...\n";

        // Use the Subject's setEvent() to forward to all registered children
        // This stores the notice and calls notify() on all child observers
        this->setEvent(notice);

        // Also apply Composite operation
        if (notice.getType() == NoticeType::CLOSE) {
            close();
        } else if (notice.getType() == NoticeType::OPEN) {
            open();
        }
    }



    /**
     * @brief Override of Control::notify() to add custom logging.
     *
     * Calls the base class (Control) notify() to forward the notice to
     * all registered child observers.
     *
     * @post Every registered child observer has had update(this) called.
     *
     * @see Control::notify()
     */
    void notify() override {
        std::cout << " [" << zoneName << "] Notifying " 
                  << getObserverCount() << " child observers...\n";
        Control::notify();
    }

    /**
     * @brief Registers a child as an observer of this zone.
     *
     * Convenience method that calls attach(observer) on this zone.
     *
     * @param child Pointer to the Observer child to register.
     *
     * @see Control::attach()
     */
    void registerChildAsObserver(Observer* child) {
        this->attach(child);
    }

    /** 
    * @brief Returns the number of child components in this zone.
     *
     * @return size_t The number of children.
     */
    size_t getChildCount() const { return children.size(); }

    /**
     * @brief Transfers ownership of a unit from one zone to another.
     *
     * This method handles BOTH:
     * - Composite ownership: remove from old parent, add to new parent.
     * - Observer registration: detach from old parent, attach to new parent.
     * - Internal pointer updates: setObservedSubject() to ensure safe
     *   self-detachment in destructors.
     *
     * @param unit The component to transfer (Leaf or sub-Composite).
     * @param fromZone The current parent zone (releases ownership).
     * @param toZone The new parent zone (acquires ownership).
     *
     * @pre unit, fromZone, and toZone must not be nullptr.
     * @pre unit must be a child of fromZone.
     * @post unit is now owned by toZone and observes toZone.
     * @post unit is no longer owned by or observing fromZone.
     *
     * @warning If the unit is an Observer, its destructor will now
     *          detach from toZone instead of fromZone.
     *
     * @see remove(), add(), setObservedSubject()
     */
    static void transfer(EventComponent* unit, EventZone* fromZone, EventZone* toZone) {
        if (unit == nullptr || fromZone == nullptr || toZone == nullptr) {
            return;
        }

        // 1. Detach as Observer from the OLD parent (if it is observing it)
        Observer* obs = dynamic_cast<Observer*>(unit);
        if (obs != nullptr) {
            fromZone->detach(obs);
        }

        // 2. Transfer Composite ownership (break old link, establish new link)
        fromZone->remove(unit);  // Does NOT delete the unit
        toZone->add(unit);       // Takes ownership

        // 3. Update the internal Subject pointer inside the unit
        // This ensures the destructor detaches from the CORRECT subject.
        //changes the field of the observer but has not yet attached it 
        unit->setObservedSubject(toZone);

        // 4. Re-attach as Observer to the NEW parent
        if (obs != nullptr) {
            toZone->attach(obs);
        }

        std::cout << " Transferred '" << unit->getName() 
                  << "' from '" << fromZone->getName() 
                  << "' to '" << toZone->getName() << "'\n";
    }


};



#endif //EVENTZONE_H