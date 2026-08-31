/**
 * @file main.cpp
 * @brief Main entry point for the Mystifare EventFlow demonstration.
 *
 * This file contains the main() function which demonstrates:
 * - Cascading notifications through 3+ runtime levels (Task 3.4)
 * - Runtime reorganisation (transfer) between zones (Task 4.2)
 * - 5 concrete leaves reacting differently to notices (Task 3.2)
 * - All 8 notice types being issued (Task 3.3)
 *
 * @note This main program is designed to be run with `make run`.
 * @author Cos 214 group
 * @date 2026
 */
#include <iostream>
#include "EventControl.h"
#include "EventZone.h"
#include "Performance.h"
#include "TheWatch.h"
#include "FoodVendor.h"
#include "Tent.h"
#include "MerchVendor.h"
#include "JoustingTournament.h"
#include "VipPass.h"

/**
 * @brief Prints a section header to the console.
 *
 * @param title The title of the section to print.
 */
void printSection(const std::string& title) {
    std::cout << "\n========================================\n";
    std::cout << "  " << title << "\n";
    std::cout << "========================================\n";
}

/**
 * @brief Main entry point of the EventFlow demonstration.
 *
 * Demonstrates:
 * 1. Cascading notification (WEATHER_ALERT) through 3+ runtime levels:
 *    - Level 1: EventControl
 *    - Level 2: EventZone (North Zone)
 *    - Level 3: EventZone (Main Stage, Tent Area)
 *    - Level 4: Leaves (Performance, TheWatch, FoodVendor, Tent, MerchVendor)
 * 2. Runtime reorganisation: Transfer of a FoodVendor from NorthZone to SouthZone.
 * 3. 5 concrete leaves reacting differently to the same WEATHER_ALERT.
 *
 * @return int Exit status (0 = success).
 */
int main() {
    printSection("Construction of Composition");

    EventControl control("Mystifare Control");

    EventZone* northZone = new EventZone("North Zone");
    EventZone* mainStage = new EventZone("Main Stage");
    EventZone* tentArea = new EventZone("Tent Area");
    EventZone* southZone = new EventZone("South Zone");



    Performance* perf = new Performance("Hamlet", "Hamlet", true);
    TheWatch* sec = new TheWatch("Watch Team Beta", 3);
    FoodVendor* foodVendor = new FoodVendor("Pizza Palace", "Italian", true);
    Tent* tent1 = new Tent("Tent #1", 2);
    MerchVendor* merchVendor = new MerchVendor("Souvenir Shop", "Souvenirs", true);

    northZone->add(mainStage);
    northZone->add(tentArea);
    northZone->add(foodVendor);
    mainStage->add(perf);
    mainStage->add(sec);
    tentArea->add(tent1);
    tentArea->add(merchVendor);

    printSection("Composite traversal");
    std::cout << "\n-- Full North Zone status --\n";
    northZone->reportStatus();

    std::cout<< "\n-- Query: North Zone total capacity --\n";
    std::cout<< "North Zone total capacity: " << northZone->getCapacity() << "\n";

    std::cout<< "\n-- Query: Main Stage child count --\n";
    std::cout<< "Main Stage children: " << mainStage->getChildCount() << "\n";

    printSection("Observer Registration");

    northZone->registerWithSubject(&control);
    mainStage->registerWithSubject(northZone);
    tentArea->registerWithSubject(northZone);

    mainStage->attach(perf);
    mainStage->attach(sec);
    tentArea->attach(tent1);
    tentArea->attach(merchVendor);
    northZone->attach(foodVendor);

    std::cout << "\n--- Tree Structure ---\n";
    std::cout << "  Control (Level 1)\n";
    std::cout << "    └── North Zone (Level 2)\n";
    std::cout << "          ├── Main Stage (Level 3)\n";
    std::cout << "          │     ├── Hamlet (Level 4)\n";
    std::cout << "          │     └── Watch Team Beta (Level 4)\n";
    std::cout << "          ├── Tent Area (Level 3)\n";
    std::cout << "          │     ├── Tent #1 (Level 4)\n";
    std::cout << "          │     └── Souvenir Shop (Level 4)\n";
    std::cout << "          └── Pizza Palace (Level 4)\n";

    printSection("Registration Change");
    std::cout<<"\n--Detatch Hamlet from Main Stage --\n";
    mainStage->detach(perf);
    std::cout<<"Main Stage Observers: "<<mainStage->getObserverCount()<<"\n";
    std::cout<<"\n--Re-attach Hamlet to Main Stage --\n";
    mainStage->attach(perf);
    std::cout<<"Main Stage Observers: "<<mainStage->getObserverCount()<<"\n";


    printSection("Cascading Notification + Runtime Reorganisation");
    printSection("Cascading Notification: WEATHER_ALERT");

    Notice weatherAlert(NoticeType::WEATHER_ALERT, 
                        "Storm approaching! Seek shelter.", 4);
    control.setEvent(weatherAlert);

    printSection("Runtime Reorganisation: Transfer");

    std::cout << "\n--- Transferring 'Pizza Palace' from North Zone to South Zone ---\n";
    EventZone::transfer(foodVendor, northZone, southZone);

    std::cout << "\n--- After Transfer ---\n";
    std::cout << "North Zone observers: " << northZone->getObserverCount() << "\n";
    std::cout << "South Zone observers: " << southZone->getObserverCount() << "\n";

    std::cout << "\n--- Issuing notice from South Zone to prove re-registration ---\n";
    Notice testNotice(NoticeType::CAPACITY_ALERT, 
                      "South Zone test: transferred vendor should hear this.", 2);
    southZone->issueNotice(testNotice);

    std::cout << "\n--- Cleaning up ---\n";
    delete northZone;
    delete southZone;

    std::cout << "Cascading notification through 3+ levels\n";
    std::cout << "Runtime reorganisation (transfer)\n\n\n\n\n";

    printSection("Mystifare Fair - Original Features Demo (Task 4.4)");

    EventControl control2("Mystifare Control");

    FoodVendor* foodVendor2 = new FoodVendor("Dragon's Feast", "Medieval Cuisine", true);
    control2.attach(foodVendor2);
    std::cout << "Feature 1: FoodVendor with allergens: gluten, dairy\n";

    JoustingTournament* tournament = new JoustingTournament("Royal Joust");
    control2.attach(tournament);
    std::cout << "Feature 2: Jousting tournament created\n";

    VipPass* vip = new VipPass("King Arthur", 5);
    control2.attach(vip);
    std::cout << "Feature 3: VIP Pass for King Arthur (Priority 5)\n";

    printSection("Notices");

    std::cout << "\n--- 1. OPEN ---\n";
    control2.setEvent(Notice(NoticeType::OPEN, "Good morning!", 1));

    std::cout << "\n--- 2. SCHEDULE_CHANGE (Jousting updates) ---\n";
    control2.setEvent(Notice(NoticeType::SCHEDULE_CHANGE, "New match schedule!", 2));

    std::cout << "\n--- 3. WEATHER_ALERT (Tournament pauses) ---\n";
    control2.setEvent(Notice(NoticeType::WEATHER_ALERT, "Storm approaching!", 4));

    std::cout << "\n--- 4. CAPACITY_ALERT (VIP priority, FoodVendor allergens) ---\n";
    control2.setEvent(Notice(NoticeType::CAPACITY_ALERT, "Area at capacity!", 3));

    std::cout << "\n--- 5. RESUME (Tournament resumes) ---\n";
    control2.setEvent(Notice(NoticeType::RESUME, "All clear!", 1));

    std::cout << "\n--- Cleaning up ---\n";
    delete foodVendor2;
    delete tournament;
    delete vip;





    return 0;
}