#ifndef EVENTZONE_H
#define EVENTZONE_H

#include "EventComponent.h"
#include "Observer.h"
#include "Control.h"
#include "Notice.h"
#include <vector>
#include <algorithm>
#include <iostream>

class EventZone: public EventComponent,public Observer,public Control{
    private:
    std::string zoneName;
    std::vector<EventComponent*> children;
    bool isOpen;
    Control* parentSubject;//Subject this zone observes for self detach

    public:
    EventZone(const std::string& name):zoneName(name),isOpen(true),parentSubject(nullptr){}
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

    void add(EventComponent* component){
        children.push_back(component);
    }
    
    void remove(EventComponent* component){
        auto it=std::find(children.begin(),children.end(),component);
        if(it!=children.end()){
            children.erase(it);
        }
    }

    void registerWithSubject(Control* subject){
        if(subject==nullptr) return;
        parentSubject=subject;
        subject->attach(this);
    }

        void open() override {
        isOpen = true;
        std::cout <<zoneName << " is OPEN.\n";
        for (EventComponent* child : children) {
            child->open();
        }
    }

    void close() override {
        isOpen = false;
        std::cout <<zoneName << " is CLOSED.\n";
        for (EventComponent* child : children) {
            child->close();
        }
    }

    void reportStatus() const override {
        std::cout << zoneName << " | Open: " 
                  << (isOpen ? "Yes" : "No")
                  << " | Children: " << children.size() << "\n";
        for (EventComponent* child : children) {
            child->reportStatus();
        }
    }

    int getCapacity() const override {
        int total = 0;
        for (EventComponent* child : children) {
            total += child->getCapacity();
        }
        return total;
    }

    std::string getName() const override { return zoneName; }

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

    void notify() override {
        std::cout << " [" << zoneName << "] Notifying " 
                  << getObserverCount() << " child observers...\n";
        Control::notify();
    }

    void registerChildAsObserver(Observer* child) {
        this->attach(child);
    }

    size_t getChildCount() const { return children.size(); }

};



#endif //EVENTZONE_H