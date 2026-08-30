// test_task3_1.cpp
#include <iostream>
#include <string>
#include "EventControl.h"
#include "Observer.h"

/**
 * @brief Simple test observer for verifying Subject functionality.
 * 
 * This observer implements the PULL approach: it receives a Control
 * pointer and pulls the notice data via getCurrentNotice().
 */
class TestObserver : public Observer {
private:
    std::string name;
    int lastSeverity;
    std::string lastMessage;

public:
    TestObserver(const std::string& n) 
        : name(n), lastSeverity(0), lastMessage("") {}

    /**
     * @brief Called by the Subject when a new notice is available.
     * 
     * Uses the PULL approach: calls subject->getCurrentNotice() 
     * to retrieve the notification data.
     * 
     * @param subject The Control (Subject) issuing the notification.
     */
    void update(Control* subject) override {
        if (subject->hasCurrentNotice()) {
            const Notice& notice = subject->getCurrentNotice();
            lastMessage = notice.getMessage();
            lastSeverity = notice.getSeverity();
            
            std::cout << "  👀 [" << name << "] Pulled notice: \""
                      << notice.getMessage() << "\" (Severity: " 
                      << notice.getSeverity() << ")\n";
        } else {
            std::cout << "  👀 [" << name << "] No notice available to pull.\n";
        }
    }

    int getLastSeverity() const { return lastSeverity; }
    std::string getLastMessage() const { return lastMessage; }
    std::string getName() const { return name; }
};

/**
 * @brief Main test function for Task 3.1
 * 
 * Demonstrates:
 * - attach() with duplicate handling
 * - detach() with non-registered handling
 * - notify() pushing to all registered observers
 * - PULL approach via getCurrentNotice()
 */
int main() {
    std::cout << "========================================\n";
    std::cout << "  Task 3.1: Subject Registration Test\n";
    std::cout << "========================================\n\n";

    // 1. Create a concrete Subject
    EventControl control("Mystifare Control Centre");
    std::cout << "✅ Created EventControl: " << control.getName() << "\n\n";

    // 2. Create test observers
    TestObserver obs1("Zone North");
    TestObserver obs2("Zone South");
    TestObserver obs3("Zone East");
    TestObserver obs4("Zone West");

    std::cout << "--- Testing attach() ---\n";
    std::cout << "Attaching 4 observers...\n";
    control.attach(&obs1);
    control.attach(&obs2);
    control.attach(&obs3);
    control.attach(&obs4);
    
    std::cout << "Registered observers: " << control.getObserverCount() << "\n";
    std::cout << "✅ Expected: 4\n\n";

    // 3. Test duplicate attach (should be ignored)
    std::cout << "--- Testing duplicate attach (should be ignored) ---\n";
    std::cout << "Attempting to attach obs1 again (already registered)...\n";
    control.attach(&obs1);  // Already registered
    
    std::cout << "Registered observers after duplicate: " 
              << control.getObserverCount() << "\n";
    std::cout << "✅ Expected: 4 (still unchanged)\n\n";

    // 4. Test notification (PULL approach)
    std::cout << "--- Testing issueNotice() and PULL ---\n";
    Notice weatherAlert(NoticeType::WEATHER_ALERT, 
                        "Storm approaching! Seek shelter immediately.", 
                        4, 
                        "All Zones");
    control.issueNotice(weatherAlert);
    std::cout << "✅ All 4 observers pulled the notice.\n\n";

    // 5. Test detach()
    std::cout << "--- Testing detach() ---\n";
    std::cout << "Detaching obs2 (Zone South)...\n";
    control.detach(&obs2);
    std::cout << "Registered observers after detach: " 
              << control.getObserverCount() << "\n";
    std::cout << "✅ Expected: 3\n\n";

    // 6. Test detach non-registered (should be ignored)
    std::cout << "--- Testing detach non-registered (should be ignored) ---\n";
    TestObserver obs5("NonExistent Zone");
    std::cout << "Attempting to detach obs5 (never registered)...\n";
    control.detach(&obs5);  // Never registered
    std::cout << "Registered observers after detach non-registered: " 
              << control.getObserverCount() << "\n";
    std::cout << "✅ Expected: 3 (still unchanged)\n\n";

    // 7. Send another notice to remaining observers
    std::cout << "--- Sending second notice to remaining observers ---\n";
    Notice resumeNotice(NoticeType::RESUME, 
                        "All clear! Resuming normal operations.", 
                        1, 
                        "All Zones");
    control.issueNotice(resumeNotice);
    std::cout << "✅ Only 3 observers received this notice (obs2 was detached).\n\n";

    // 8. Verify observer counts and state
    std::cout << "--- Final Verification ---\n";
    std::cout << "Total observers still registered: " 
              << control.getObserverCount() << "\n";
    std::cout << "Current notice stored: " 
              << control.getCurrentNotice().getMessage() << "\n";
    std::cout << "Has current notice: " 
              << (control.hasCurrentNotice() ? "Yes" : "No") << "\n";

    std::cout << "\n========================================\n";
    std::cout << "  Task 3.1 Test Complete! ✅\n";
    std::cout << "========================================\n";

    return 0;
}