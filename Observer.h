/**
 * @file Observer.h
 * @brief Defines the Observer interface for the Observer pattern.
 *
 * This file contains the Observer abstract class, which defines the
 * interface that all concrete observers in the Mystifare fair must
 * implement. It uses the PULL approach: observers receive a Subject
 * pointer and pull the current notice data via getCurrentNotice().
 *
 * @note All concrete observers must override update(Control* subject).
 * @warning Observers must detach themselves from the Subject in their
 *          destructors to prevent dangling pointers.
 *
 * @author Cos 214 group
 * @date 2026
 */
#ifndef OBSERVER_H
#define OBSERVER_H

/**
 * @brief Forward declaration of the Control (Subject) class.
 *
 * This forward declaration allows the Observer interface to refer
 * to Control without including Control.h, which would create a
 * circular dependency.
 */
class Control;

/**
 * @brief Abstract interface for objects that observe Subjects.
 *
 * This class defines the update() method that Subjects call to notify
 * observers of changes. Using the PULL approach, the observer receives
 * a Control* pointer and must call subject->getCurrentNotice() to
 * retrieve the notification data.
 *
 * **PULL Approach:**
 * 1. Subject calls observer->update(this) (passes Subject pointer).
 * 2. Observer checks subject->hasCurrentNotice().
 * 3. Observer calls subject->getCurrentNotice() to pull the notice.
 * 4. Observer reacts based on the pulled notice data.
 *
 * @note This is the abstract base class for all concrete observers.
 * @warning Observers must store the Subject pointer to safely detach
 *          themselves in their destructors.
 *
 * @see Control, Notice
 */
class Observer{
    public:
    /**
     * @brief Virtual destructor for polymorphic deletion.
     *
     * Ensures that derived class destructors are called when deleting
     * an object through a base class pointer.
     */
    virtual ~Observer()=default;
    /**
     * @brief Called by the Subject when a new notice is available.
     *
     * This method implements the PULL approach. The observer should:
     * 1. Store the subject pointer for self-detachment on destruction.
     * 2. Call subject->hasCurrentNotice() to check if a notice exists.
     * 3. Call subject->getCurrentNotice() to pull the notice data.
     * 4. React based on the notice's type, message, severity, etc.
     *
     * @param subject The Subject that issued the notification.
     *                This is a Control* (base Subject class).
     *
     * @note This method is pure virtual—all concrete observers
     *       must override it.
     * @note Uses the PULL approach—no data is pushed directly.
     *
     * @see Control::getCurrentNotice(), Control::hasCurrentNotice()
     */
    virtual void update(Control* subject)=0;
};


#endif //OBSERVER_H