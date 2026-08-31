#ifndef OBSERVER_H
#define OBSERVER_H
class Control;

class Observer{
    public:
    virtual ~Observer()=default;
    /*pass subject pointer to pull current notice data from getCurrentNotice()*/
    virtual void update(Control* subject)=0;
};


#endif //OBSERVER_H