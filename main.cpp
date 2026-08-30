// main.cpp - Task 3.3 Test
#include <iostream>
#include "EventControl.h"
#include "Performance.h"
#include "TheWatch.h"
#include "FoodVendor.h"
#include "Tent.h"
#include "MerchVendor.h"

void printSection(const std::string& title) {
    std::cout << "\n========================================\n";
    std::cout << "  " << title << "\n";
    std::cout << "========================================\n";
}

int main() {
    printSection("Task 3.3: All 8 Notice Types Test");

    EventControl control("Mystifare Control");
    std::cout << "✅ Created EventControl\n\n";

    // Create 5 observers
    Performance romeo("Main Stage", "Romeo & Juliet", true);
    TheWatch watch("Watch Team Alpha", 5);
    FoodVendor food("Dragon's Feast", "Medieval Cuisine", true);
    Tent tent("Tent #1", 2);
    MerchVendor merch("Merchant's Guild", "Costumes & Souvenirs", true);

    // Register all observers
    control.attach(&romeo);
    control.attach(&watch);
    control.attach(&food);
    control.attach(&tent);
    control.attach(&merch);
    std::cout << "✅ Registered 5 observers\n\n";

    // ==========================================
    // Test all 8 notice types
    // ==========================================

    // 1. OPEN (Ordinary Operational)
    std::cout << "--- 1. OPEN ---\n";
    Notice openNotice(NoticeType::OPEN, "Good morning! All venues opening.", 1);
    control.setEvent(openNotice);

    // 2. CLOSE (Ordinary Operational)
    std::cout << "\n--- 2. CLOSE ---\n";
    Notice closeNotice(NoticeType::CLOSE, "End of day. Closing all venues.", 2);
    control.setEvent(closeNotice);

    // 3. SCHEDULE_CHANGE (Ordinary Operational)
    std::cout << "\n--- 3. SCHEDULE_CHANGE ---\n";
    Notice scheduleNotice(NoticeType::SCHEDULE_CHANGE, 
                          "Main Stage show moved to 7pm.", 2);
    control.setEvent(scheduleNotice);

    // 4. CAPACITY_ALERT (Capacity-related)
    std::cout << "\n--- 4. CAPACITY_ALERT ---\n";
    Notice capacityNotice(NoticeType::CAPACITY_ALERT, 
                          "North Zone at 90% capacity!", 3);
    control.setEvent(capacityNotice);

    // 5. WEATHER_ALERT (Safety-related)
    std::cout << "\n--- 5. WEATHER_ALERT ---\n";
    Notice weatherNotice(NoticeType::WEATHER_ALERT, 
                         "Storm approaching! Seek shelter.", 4);
    control.setEvent(weatherNotice);

    // 6. PAUSE (Ordinary Operational)
    std::cout << "\n--- 6. PAUSE ---\n";
    Notice pauseNotice(NoticeType::PAUSE, 
                       "Temporary pause due to technical issue.", 3);
    control.setEvent(pauseNotice);

    // 7. RESUME (Ordinary Operational)
    std::cout << "\n--- 7. RESUME ---\n";
    Notice resumeNotice(NoticeType::RESUME, 
                        "All clear! Resume operations.", 1);
    control.setEvent(resumeNotice);

    // 8. EVACUATE (Safety-related)
    std::cout << "\n--- 8. EVACUATE ---\n";
    Notice evacuateNotice(NoticeType::EVACUATE, 
                          "Evacuate all zones immediately!", 5);
    control.setEvent(evacuateNotice);

    // Final RESUME to bring everything back
    std::cout << "\n--- 9. RESUME (final cleanup) ---\n";
    control.setEvent(resumeNotice);

    printSection("Task 3.3 Complete ✅");
    std::cout << "All 8 notice types tested:\n";
    std::cout << "  ✅ OPEN (Ordinary Operational)\n";
    std::cout << "  ✅ CLOSE (Ordinary Operational)\n";
    std::cout << "  ✅ SCHEDULE_CHANGE (Ordinary Operational)\n";
    std::cout << "  ✅ CAPACITY_ALERT (Capacity-related)\n";
    std::cout << "  ✅ WEATHER_ALERT (Safety-related)\n";
    std::cout << "  ✅ PAUSE (Ordinary Operational)\n";
    std::cout << "  ✅ RESUME (Ordinary Operational)\n";
    std::cout << "  ✅ EVACUATE (Safety-related)\n";

    return 0;
}