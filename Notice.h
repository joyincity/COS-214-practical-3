/**
 * @file Notice.h
 * @brief Defines the Notice class and NoticeType enum for event notifications.
 *
 * This file contains the NoticeType enumeration and the Notice class,
 * which represents a notification/notice in the EventFlow system.
 *
 * The Notice class is the state object that is passed between Subjects
 * and Observers using the PULL approach. Observers call getCurrentNotice()
 * to retrieve the notice data.
 *
 * @note This class is used by both the Subject and Observer pattern
 *       implementations.
 *
 * @author Your Team Name
 * @date 2026
 */
#ifndef NOTICE_H
#define NOTICE_H

#include <string>

/**
 * @brief Types of notices that can be issued in the Mystifare fair.
 *
 * This enumeration defines all possible notice types that can be issued
 * by Subjects (EventControl or EventZone). Each type represents a
 * different kind of event that observers may react to.
 *
 * **Categories:**
 * - **Ordinary Operational:** OPEN, CLOSE, SCHEDULE_CHANGE, PAUSE, RESUME
 * - **Capacity-Related:** CAPACITY_ALERT
 * - **Safety-Related:** WEATHER_ALERT, EVACUATE
 *
 * @note At least 6 notice types are required by Task 3.3.
 * @see Notice
 */

enum class NoticeType{
    /**
     * @brief Open an area/venue.
     *
     * Indicates that an area, zone, or venue should open for operations.
     * Ordinary operational notice.
     */
    OPEN,

    /**
     * @brief Close an area/venue.
     *
     * Indicates that an area, zone, or venue should close operations.
     * Ordinary operational notice.
     */
    CLOSE,

    /**
     * @brief Schedule change notification.
     *
     * Indicates that the schedule has changed (e.g., show times moved).
     * Ordinary operational notice.
     */
    SCHEDULE_CHANGE,

    /**
     * @brief Capacity limit alert.
     *
     * Indicates that an area is approaching or has reached capacity limits.
     * Capacity-related notice.
     */
    CAPACITY_ALERT,

    /**
     * @brief Severe weather warning.
     *
     * Indicates that severe weather is approaching.
     * Safety-related notice.
     */
    WEATHER_ALERT,

    /**
     * @brief Pause operations.
     *
     * Indicates that operations should pause temporarily.
     * Ordinary operational notice.
     */
    PAUSE,

    /**
     * @brief Resume operations.
     *
     * Indicates that operations should resume after a pause.
     * Ordinary operational notice.
     */
    RESUME,

    /**
     * @brief Evacuate the area.
     *
     * Indicates that the area should be evacuated immediately.
     * Safety-related notice.
     */
    EVACUATE,

    /**
     * @brief cusine from a food vendor has allergens
     * Safety-related notice
     */
    ALLERGEN_ALERT
};

/**
 * @brief Represents a notification/notice in the EventFlow system.
 *
 * This class encapsulates all relevant information about an event change.
 * It is the state object that is stored in the Subject and retrieved
 * by Observers using the PULL approach.
 *
 * **Fields:**
 * - `type`: The type of notice (e.g., WEATHER_ALERT)
 * - `message`: Human-readable description
 * - `severity`: Urgency level (1 = low, 5 = critical)
 * - `affectedZone`: Target zone (empty = all zones)
 *
 * @note This class is used with the PULL approach: observers call
 *       Control::getCurrentNotice() to retrieve the notice.
 * @see NoticeType, Control::getCurrentNotice()
 */
class Notice{
    private:
    /**
     * @brief The type of this notice.
     *
     * Determines how observers react to the notice.
     */
    NoticeType type;

    /**
     * @brief Human-readable message describing the notice.
     *
     * Used for logging and console output.
     */
    std::string message;

    /**
     * @brief Urgency level of the notice (1-5).
     *
     * 1 = low priority, 5 = critical emergency.
     */
    int severity;

    /**
     * @brief Name of the affected zone.
     *
     * Empty string means the notice applies to all zones.
     */
    std::string affectedZone;

public:
    /**
     * @brief Constructs a new Notice object.
     *
     * @param t The type of notice (e.g., WEATHER_ALERT).
     * @param msg Human-readable message describing the notice.
     * @param sev Severity level (1-5). Defaults to 3 (medium).
     * @param zone Name of the affected zone. Empty = all zones. Defaults to "".
     *
     * @pre sev must be between 1 and 5.
     * @post A fully initialized Notice object is created.
     *
     * @see NoticeType
     */
    Notice(NoticeType t,const std::string& msg,int sev=3,const std::string& zone=""):type(t),message(msg),severity(sev),affectedZone(zone){}

    /**
     * @brief Gets the type of this notice.
     *
     * Observers use this to determine how to react.
     *
     * @return NoticeType The type of notice.
     */
    NoticeType getType() const{return type;}

    /**
     * @brief Gets the human-readable message of this notice.
     *
     * Used for logging and console output.
     *
     * @return std::string The message string.
     */
    std::string getMessage() const{return message;}

    /**
     * @brief Gets the severity level of this notice.
     *
     * @return int Severity level (1 = low, 5 = critical).
     */
    int getSeverity() const {return severity;}

    /**
     * @brief Gets the name of the affected zone.
     *
     * @return std::string The zone name. Empty string = all zones.
     */
    std::string getAffectedZone() const{return affectedZone;}


};




#endif //NOTICE_H