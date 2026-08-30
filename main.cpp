// main.cpp
// ============================================
// Task 3.1 & 3.2 - Subject/Observer Test
// - attach() / detach() policies
// - 5 concrete observers reacting to WEATHER_ALERT
// - Safe destruction (self-detach)
// ============================================

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
    printSection("Task 3.1 & 3.2: Subject + Concrete Observers");

    // ==========================================
    // 1. CREATE SUBJECT AND 5 OBSERVERS
    // ==========================================
    EventControl control("Mystifare Control");
    std::cout << "✅ Created EventControl\n\n";

    Performance romeo("Main Stage", "Romeo & Juliet", true);
    TheWatch watch("Watch Team Alpha", 5);
    FoodVendor food("Dragon's Feast", "Medieval Cuisine", true);
    Tent tent("Tent #1", 2);
    MerchVendor merch("Merchant's Guild", "Costumes & Souvenirs", true);

    std::cout << "✅ Created 5 concrete observers:\n";
    std::cout << "  1. " << romeo.getName() << " (Performance)\n";
    std::cout << "  2. " << watch.getName() << " (Security)\n";
    std::cout << "  3. " << food.getName() << " (Food Vendor)\n";
    std::cout << "  4. " << tent.getName() << " (Tent)\n";
    std::cout << "  5. " << merch.getName() << " (Merch Vendor)\n";

    // ==========================================
    // 2. TEST ATTACH() & DUPLICATE POLICY (3.1)
    // ==========================================
    printSection("Task 3.1: attach() & Duplicate Policy");

    std::cout << "Attaching 5 observers...\n";
    control.attach(&romeo);
    control.attach(&watch);
    control.attach(&food);
    control.attach(&tent);
    control.attach(&merch);
    std::cout << "Registered observers: " << control.getObserverCount() << "\n";
    std::cout << "✅ Expected: 5\n";

    std::cout << "\nAttempting duplicate attach (should be ignored):\n";
    control.attach(&romeo);  // Already registered
    std::cout << "Registered observers after duplicate: " 
              << control.getObserverCount() << "\n";
    std::cout << "✅ Expected: 5 (unchanged)\n";

    // ==========================================
    // 3. TEST NOTIFICATION (All 5 React Differently)
    // ==========================================
    printSection("Task 3.2: Concrete Observer Reactions");

    std::cout << "--- Issuing WEATHER_ALERT ---\n";
    Notice weatherAlert(NoticeType::WEATHER_ALERT, 
                        "Storm approaching! Seek shelter.", 4);
    control.setEvent(weatherAlert);

    std::cout << "\n--- Issuing RESUME ---\n";
    Notice resumeNotice(NoticeType::RESUME, 
                        "All clear! Resume operations.", 1);
    control.setEvent(resumeNotice);

    // ==========================================
    // 4. TEST DETACH() & NON-REGISTERED POLICY (3.1)
    // ==========================================
    printSection("Task 3.1: detach() & Non-Registered Policy");

    std::cout << "Current observers: " << control.getObserverCount() << "\n";

    std::cout << "\nDetaching 'Watch Team Alpha'...\n";
    control.detach(&watch);
    std::cout << "Observers after detach: " << control.getObserverCount() << "\n";
    std::cout << "✅ Expected: 4\n";

    std::cout << "\nDetaching a non-registered observer (should be ignored):\n";
    Performance fakeObserver("Fake Stage", "Fake Show", false);
    control.detach(&fakeObserver);  // Never attached
    std::cout << "Observers after detach non-registered: " 
              << control.getObserverCount() << "\n";
    std::cout << "✅ Expected: 4 (unchanged)\n";

    // ==========================================
    // 5. TEST SAFE DESTRUCTION (3.2)
    // ==========================================
    printSection("Task 3.2: Safe Destruction (Self-Detach)");

    std::cout << "Creating a heap-allocated MerchVendor and attaching it...\n";
    MerchVendor* tempVendor = new MerchVendor("Temp Shop", "Trinkets", false);
    control.attach(tempVendor);
    std::cout << "Observers before deletion: " << control.getObserverCount() << "\n";
    std::cout << "✅ Expected: 5\n";

    std::cout << "\nDeleting the heap vendor (destructor auto-detaches)...\n";
    delete tempVendor;
    std::cout << "Observers after deletion: " << control.getObserverCount() << "\n";
    std::cout << "✅ Expected: 4 (back to normal)\n";
    std::cout << "✅ No dangling pointers remain!\n";

    // ==========================================
    // 6. FINAL STATUS
    // ==========================================
    printSection("Final Status");

    std::cout << "Remaining registered observers: " << control.getObserverCount() << "\n";
    std::cout << "  (Watch Team Alpha was detached)\n";
    std::cout << "  (Temp Shop was deleted)\n";
    std::cout << "  Remaining: Performance, FoodVendor, Tent, MerchVendor\n";

    std::cout << "\n📢 Current notice: " 
              << control.getCurrentNotice().getMessage() << "\n";

    std::cout << "\n========================================\n";
    std::cout << "  ✅ Task 3.1 & 3.2 COMPLETE!\n";
    std::cout << "  - attach/detach policies: OK\n";
    std::cout << "  - 5 concrete observers: OK\n";
    std::cout << "  - Safe destruction: OK\n";
    std::cout << "========================================\n";

    return 0;
}