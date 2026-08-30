#ifndef NOTICE_H
#define NOTICE_H

#include <string>

enum class NoticeType{
    OPEN,
    CLOSE,
    SCHEDULE_CHANGE,
    CAPACITY_ALERT,
    WEATHER_ALERT,
    PAUSE,
    RESUME,
    EVACUATE
};

/*Notice object containing the event type, a human readable message,severity level and affected zone*/
class Notice{
    private:
    NoticeType type;
    std::string message;
    int severity;
    std::string affectedZone;

public:
Notice(NoticeType t,const std::string& msg,int sev=3,const std::string& zone=""):type(t),message(msg),severity(sev),affectedZone(zone){}
/*return the type of notice-the enum*/
NoticeType getType() const{return type;}
/*returns a descriptive message*/
std::string getMessage() const{return message;}
/*returns the urgency level from 1-5*/
int getSeverity() const {return severity;}
/*returns which zone the notice applies to*/
std::string getAffectedZone() const{return affectedZone;}


};




#endif //NOTICE_H