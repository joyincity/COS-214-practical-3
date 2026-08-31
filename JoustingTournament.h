#ifndef JOUSTINGTOURNAMENT_H
#define JOUSTINGTOURNAMENT_H

#include "EventComponent.h"
#include "Observer.h"
#include "Control.h"
#include "Notice.h"
#include <iostream>
#include <vector>

class JoustingTournament : public EventComponent, public Observer {
private:
    std::string tournamentName;
    int currentRound;
    bool isActive;
    std::vector<std::string> knights;
    std::vector<int> scores;
    Control* registeredSubject;

public:
    JoustingTournament(const std::string& name) 
        : tournamentName(name), currentRound(0), isActive(false), registeredSubject(nullptr) {
        knights = {"Sir Lancelot", "Sir Gawain", "Sir Percival", "Sir Galahad"};
        scores = {0, 0, 0, 0};
    }

    virtual ~JoustingTournament() {
        if (registeredSubject) {
            registeredSubject->detach(this);
            registeredSubject = nullptr;
        }
    }

    void open() override {
        isActive = true;
        currentRound = 1;
        std::cout <<  tournamentName << " JOUSTING TOURNAMENT BEGINS!\n";
        std::cout << "  Round 1: " << knights[0] << " vs " << knights[1] << "\n";
    }

    void close() override {
        isActive = false;
        std::cout << tournamentName << " tournament CLOSED.\n";
    }

    void reportStatus() const override {
        std::cout << tournamentName << " | Round: " << currentRound
                  << " | Active: " << (isActive ? "Yes" : "No") << "\n";
        for (size_t i = 0; i < knights.size(); ++i) {
            std::cout << "    " << knights[i] << ": " << scores[i] << " points\n";
        }
    }

    int getCapacity() const override { return 1000; }
    std::string getName() const override { return tournamentName; }

    void registerWithSubject(Control* subject) {
        if (subject == nullptr) return;
        registeredSubject = subject;
        subject->attach(this);
    }

    void setObservedSubject(Control* subject) override {
        registeredSubject = subject;
    }

    void update(Control* subject) override {
        if (registeredSubject == nullptr) {
            registeredSubject = subject;
        }

        if (!subject->hasCurrentNotice()) return;
        const Notice& notice = subject->getCurrentNotice();

        switch (notice.getType()) {
            case NoticeType::OPEN:
                if (!isActive) open();
                break;

            case NoticeType::CLOSE:
                if (isActive) close();
                break;

            case NoticeType::SCHEDULE_CHANGE:
                if (isActive) {
                    currentRound++;
                    std::cout <<tournamentName << " ROUND " << currentRound
                              << ": " << knights[currentRound % knights.size()] 
                              << " vs " << knights[(currentRound + 1) % knights.size()] << "\n";
                }
                break;

            case NoticeType::WEATHER_ALERT:
                if (isActive) {
                    std::cout <<  tournamentName << " PAUSED due to weather!\n";
                }
                break;

            case NoticeType::RESUME:
                if (!isActive) {
                    isActive = true;
                    std::cout << tournamentName << " RESUMED!\n";
                }
                break;

            default:
                break;
        }
    }
};

#endif // JOUSTINGTOURNAMENT_H