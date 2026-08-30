#ifndef CONTROL_H
#define CONTROL_H
#include <vector>
#include "Observer.h"
#include "Notice.h"
#include <algorithm>
class Control{
    private:
    Notice currentNotice;
    bool hasNotice;

    protected:
    std::vector<Observer*> observers;
    virtual void setEvent(const Notice& notice){
        currentNotice=notice;
        hasNotice=true;
        notify();
    }

    public:
    Control():currentNotice(Notice(NoticeType::OPEN,"Default",1,"")),hasNotice(false){}
    virtual ~Control()=default;
    virtual void attach(Observer* observer){
        if(observer==nullptr){
            return;
        }

        //Check for duplicate registration and ignore registration if already exists
        auto it=std::find(observers.begin(),observers.end(),observer);
        if(it!=observers.end()){
            return;
        }

        observers.push_back(observer);
    } 

    virtual void detach(Observer* observer){
        if(observer==nullptr){
            return;
        }

        auto it=std::find(observers.begin(),observers.end(),observer);
        if(it!=observers.end()){
            observers.erase(it);
        }

        //If not found,do nothing
    }

    virtual void notify(){
        if(!hasNotice){
            return;//No notice to notify about
        }

        for(Observer* observer:observers){
            observer->update(this);
        }
    }

    const Notice& getCurrentNotice() const{
        return currentNotice;
    }

    /*for observers to avoid pulling empty data*/
    bool hasCurrentNotice() const{
        return hasNotice;
    }

    /*testing and debugging the current number of registered observers*/
    size_t getObserverCount() const{
        return observers.size();
    }

};



#endif //CONTROL_H