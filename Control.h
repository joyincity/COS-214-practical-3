/**
 * @file Control.h
 * @brief Defines the base Subject cass for the Observer pattern.
 * 
 * This file contains the Control class,which serves as the base Subject
 * in the Observer pattern. It maintains a list of observers and provides
 * methods to attach,detach and notify them
 * 
 * @note Uses the PULL approach: observers call  getCurrentNotice() to
 * retrieve notification data
 * @warning Observers are stored as non-owning raw pointers. They must detach themselves in their
 * destructors to avoid dangling pointers
 * 
 * @author COS 214 group
 * @date 2026
 */


#ifndef CONTROL_H
#define CONTROL_H
#include <vector>
#include "Observer.h"
#include "Notice.h"
#include <algorithm>

/**
 * @brief Base Subject class that manages observer registration and notification.
 *
 * This class implements the Subject role in the Observer pattern.
 * It maintains a list of Observer pointers and provides:
 * - attach() / detach() for registration management
 * - setEvent() to store a notice and trigger notification
 * - notify() to broadcast the notice to all registered observers
 * - getCurrentNotice() for the PULL approach
 *
 * @note Observers are stored as non-owning raw pointers.
 *       The Subject does NOT delete observers on destruction.
 * @warning Observers must call detach(this) in their destructor to
 *          prevent dangling pointers in the Subject's list.
 */
class Control{
    private:
    /**
     * @brief The current notice stored in this Subject.
     * 
     * This is the state that observers pull when they call
     * getCurrentNotice()
     */
    Notice currentNotice;
    /**
     * @brief Flag indicating whether a valid notice is currently stored
     * 
     * Observers should check this with hasCurrentNotice() before 
     * calling getCurrentNotice() to avoid pulling stale data
     */
    bool hasNotice;

    protected:
    /**
     * @brief List of registered observers
     * 
     * This vector stores raw pointers to Observer objects
     * The Subject does NOT own these pointers-observers manage their own memory and detach themselves on
     * destruction,because observers' lifetimes should be independant of the subject. When the subject
     * is destructed the observer must stil persist
     */
    std::vector<Observer*> observers;
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
    virtual void setEvent(const Notice& notice){
        currentNotice=notice;
        hasNotice=true;
        notify();
    }

    public:
    /**
     * @brief Constructs a new Control Subject with no active notice
     * 
     * Initializes hasNotice to false and sets a default empty notice.
     */
    Control():currentNotice(Notice(NoticeType::OPEN,"Default",1,"")),hasNotice(false){}
    /**
     * @brief Virtual destructor for polymorphic destruction
     * 
     * @note Does NOT delete any observers(non-owning pointers).
     * Observers must detach theemselves in their destructors
     * @warning If observers remain attached when this Subject is destoyed, they will have dangling pointers. Ensure all observers call
     * detach(this) before they are destroyed
     */
    virtual ~Control()=default;
    /**
     * @brief Registers an observer for future event notifications.
     * 
     * This subject stores a non-owning pointer to the observer.
     * The observer must remain alive while registered and must call detach(this) before its destruction.
     * 
     * @param observer Pointter to the Observer to register. Must not be nullptr
     * 
     * Duplicate registration: If the observer is already registered it is ignored. Message saying it aready exists is output
     * @warning Observers must not be destroyed while registered without calling detach() first.
     * 
     * @see detach(),notify()
     */
    virtual void attach(Observer* observer){
        if(observer==nullptr){
            return;
        }

        //Check for duplicate registration and ignore registration if already exists
        auto it=std::find(observers.begin(),observers.end(),observer);
        if(it!=observers.end()){
            std::cout<<"Can not add an observer already in the list"<<std::endl;
            return;
        }

        observers.push_back(observer);
        std::cout<<"Observer successfully added to the list"<<std::endl;
    } 
    /**
     * @brief Deregisteres an observer from future event notifications
     * 
     * Removes the observer from the Subject's registration list
     * 
     * @param observer Pointer to the Observer to deregeister
     * 
     * Non-registered observer: If the observer is not found it is ignored.Output a message saying  the Observer doesnt exist.
     */
    virtual void detach(Observer* observer){
        if(observer==nullptr){
            return;
        }

        auto it=std::find(observers.begin(),observers.end(),observer);
        if(it!=observers.end()){
            observers.erase(it);
            std::cout<<"Observer successfully removed from the list"<<std::endl;
            return;
        }
        std::cout<<"Observer does not exist to detach"<<std::endl;
        //If not found,do nothing
    }

    /**
     * @brief Notifies all registered observers of the current notiice.
     * 
     * Calls update(this) on every registered observer,passing a pointer to this Subject so observers can
     * PULL the current notice data via getCurrentNotice() 
     * 
     * @pre hasNotice must be true
     * @post Every registered observer has  had update(this) called
     * 
     * @note This is the PULL approach: the Subject inly passes itself and observers pull the data
     * @see setEvent(),getCurrentNotice(),Observer::update()
     */
    virtual void notify(){
        if(!hasNotice){
            return;//No notice to notify about
        }

        for(Observer* observer:observers){
            observer->update(this);
        }
    }
    /**
     * @brief Returns a const reference to the current notice
     * 
     * Observers call this method inside their update() to PULL the notification data from the Subject
     * 
     * @return const Notice& Reference to the current notice
     * @pre hasCurrentNotice() must be true.
     * 
     * @see hasCurrentNotice,setEvent()
     */
    const Notice& getCurrentNotice() const{
        return currentNotice;
    }

    /**
     * @brief Checks whether a valid notice is currently available.
     * 
     * Observers should call this before getCurrentNotice() to ensure
     * they are enot pulling stale or uninitialized data
     * 
     * @return true if a valid notice is stored,false otherwise
     * 
     * @see getCurrentNotice() 
     */
    bool hasCurrentNotice() const{
        return hasNotice;
    }

    /**
     * @brief Returns the number of currently registered observers
     * 
     * Utility method for testing and debugging
     * 
     * @return size_t Number of observers in the registration list
     */
    size_t getObserverCount() const{
        return observers.size();
    }

};



#endif //CONTROL_H