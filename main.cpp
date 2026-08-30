#include <iostream>
#include "EventControl.h"
#include "EventZone.h"
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
    printSection("Cascading Notification + Runtime Reorganisation");

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

    printSection("Demo Complete");
    std::cout << "Cascading notification through 3+ levels\n";
    std::cout << "Runtime reorganisation (transfer)\n";

    return 0;
}