/**
 * @file JoustingTournament.h
 * @brief Defines a jousting tournament feature for the Mystifare fair.
 *
 * This file contains the JoustingTournament class, which represents an
 * original feature (Task 4.4) for the Mystifare Renaissance Fair. It
 * simulates a jousting tournament with knights, rounds, and scores.
 *
 * @note This is one of the 3 original features required by Task 4.4.
 * @note This class is NOT part of the Composite tree—it is a standalone
 *       feature that observes the control centre.
 *
 * @author COS 214 group
 * @date 2026
 */
#ifndef JOUSTINGTOURNAMENT_H
#define JOUSTINGTOURNAMENT_H

#include "EventComponent.h"
#include "Observer.h"
#include "Control.h"
#include "Notice.h"
#include <iostream>
#include <vector>

/**
 * @brief Concrete Observer that represents a jousting tournament.
 *
 * This class simulates a jousting tournament at the Mystifare fair.
 * It is a Concrete Observer that reacts to notices from the control centre.
 *
 * **Reaction to notices:**
 * - `OPEN`: Starts the tournament (begins Round 1).
 * - `CLOSE`: Ends the tournament.
 * - `SCHEDULE_CHANGE`: Advances to the next round with new matchups.
 * - `WEATHER_ALERT`: Pauses the tournament (does not close).
 * - `RESUME`: Resumes the tournament if paused.
 *
 * **Original Feature:** This class demonstrates an engaging, interactive
 * attraction that responds to event-wide notices without being part of
 * the Composite tree, showcasing how the Observer pattern can be used
 * for non-structural features.
 *
 * @note This is a Concrete Observer that is NOT part of the Composite tree.
 * @see EventComponent, Observer
 */
class JoustingTournament : public EventComponent, public Observer {
private:
    /**
     * @brief The name of this tournament (e.g., "Royal Joust").
     */
    std::string tournamentName;

    /**
     * @brief The current round number (starts at 0).
     */
    int currentRound;

    /**
     * @brief Whether the tournament is currently active.
     */
    bool isActive;

    /**
     * @brief Whether the tournament is paused due to weather.
     */
    bool isPaused;

    /**
     * @brief List of knight names participating in the tournament.
     */
    std::vector<std::string> knights;

    /**
     * @brief List of scores corresponding to each knight.
     */
    std::vector<int> scores;

    /**
     * @brief Pointer to the Subject this tournament is registered with.
     *
     * Stored for safe self-detachment in the destructor.
     */
    Control* registeredSubject;

public:
    /**
     * @brief Constructs a new JoustingTournament.
     *
     * Initializes the tournament with four default knights, scores of 0,
     * round 0, and inactive/paused states set to false.
     *
     * @param name The display name of the tournament.
     */
    JoustingTournament(const std::string& name) 
        : tournamentName(name), currentRound(0), isActive(false), registeredSubject(nullptr) {
        knights = {"Sir Lancelot", "Sir Gawain", "Sir Percival", "Sir Galahad"};
        scores = {0, 0, 0, 0};
    }

    /**
     * @brief Destructor that automatically detaches from the Subject.
     *
     * Prevents dangling pointers in the Subject's observer list.
     *
     * @note This is part of the safe destruction policy (Task 3.2).
     * @warning Must be called before the Subject is destroyed.
     */
    virtual ~JoustingTournament() {
        if (registeredSubject) {
            registeredSubject->detach(this);
            registeredSubject = nullptr;
        }
    }

    /**
     * @brief Opens the tournament and begins Round 1.
     *
     * Sets the tournament to active, resets paused state, and displays
     * the first matchup.
     *
     * @post Tournament is active and Round 1 has begun.
     * @see close()
     */
    void open() override {
        isActive = true;
        currentRound = 1;
        std::cout <<  tournamentName << " JOUSTING TOURNAMENT BEGINS!\n";
        std::cout << "  Round 1: " << knights[0] << " vs " << knights[1] << "\n";
    }

    /**
     * @brief Closes the tournament.
     *
     * Sets isActive to false and announces the winner.
     *
     * @post Tournament is closed and no longer active.
     * @see open()
     */
    void close() override {
        isActive = false;
        std::cout << tournamentName << " tournament CLOSED.\n";
    }

    /**
     * @brief Reports the current status of the tournament.
     *
     * Outputs tournament name, round, active status, and scores of all knights.
     *
     * @post No state changes occur (const operation).
     */
    void reportStatus() const override {
        std::cout << tournamentName << " | Round: " << currentRound
                  << " | Active: " << (isActive ? "Yes" : "No") << "\n";
        for (size_t i = 0; i < knights.size(); ++i) {
            std::cout << "    " << knights[i] << ": " << scores[i] << " points\n";
        }
    }

    /**
     * @brief Returns the spectator capacity of the tournament.
     *
     * @return int Fixed capacity of 1000 spectators.
     */
    int getCapacity() const override { return 1000; }

    /**
     * @brief Returns the name of this tournament.
     *
     * @return std::string The tournament's display name.
     */
    std::string getName() const override { return tournamentName; }

    /**
     * @brief Registers this tournament as an observer of a Subject.
     *
     * Stores the Subject pointer for safe self-detachment in the destructor,
     * and calls subject->attach(this) to add this tournament to the
     * Subject's observer list.
     *
     * @param subject The Subject to observe.
     * @pre subject must not be nullptr.
     * @post This tournament is registered with the Subject.
     *
     * @see Observer::update()
     */
    void registerWithSubject(Control* subject) {
        if (subject == nullptr) return;
        registeredSubject = subject;
        subject->attach(this);
    }

     /**
     * @brief Updates the internal Subject pointer for safe self-detachment.
     *
     * Used by the transfer mechanism to update the registeredSubject pointer.
     *
     * @param subject The new Subject this tournament should observe.
     *
     * @note This method is required by the EventComponent interface.
     */
    void setObservedSubject(Control* subject) override {
        registeredSubject = subject;
    }

    /**
    * @brief Called by the Subject when a new notice is available.
     *
     * This method implements the PULL approach:
     * 1. Stores the Subject pointer for self-detachment.
     * 2. Checks if a notice is available via hasCurrentNotice().
     * 3. Pulls the notice data via getCurrentNotice().
     * 4. Reacts based on the notice type.
     *
     * **Reactions:**
     * - `OPEN`: Begins the tournament.
     * - `CLOSE`: Ends the tournament and announces the winner.
     * - `SCHEDULE_CHANGE`: Advances to the next round with new matchups.
     * - `WEATHER_ALERT`: Pauses the tournament (does not close).
     * - `RESUME`: Resumes the tournament if paused.
     *
     * @param subject The Subject that issued the notification.
     *
     * @note This is a Concrete Observer implementation.
     * @note This is one of the 3 original features (Task 4.4).
     * @see Control::getCurrentNotice()
     */
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
                if (isActive && !isPaused) {
                    currentRound++;
                    std::cout <<tournamentName << " ROUND " << currentRound
                              << ": " << knights[currentRound % knights.size()] 
                              << " vs " << knights[(currentRound + 1) % knights.size()] << "\n";
                }
                break;

            case NoticeType::WEATHER_ALERT:
                if (isActive) {
                    isPaused=true;
                    std::cout <<  tournamentName << " PAUSED due to weather!\n";
                }
                break;

            case NoticeType::RESUME:
                if (isPaused) {
                    isPaused=false;
                    isActive = true;
                    std::cout << tournamentName << " RESUMED!\n";
                }
                break;

            default:
                break;
        }
    }

     /**
     * @brief Returns the current round number.
     *
     * @return int The current round.
     */
    int getCurrentRound() const {
        return currentRound;
    }

    /**
     * @brief Returns whether the tournament is active.
     *
     * @return true if the tournament is currently running.
     */
    bool getIsActive() const {
        return isActive;
    }

    /**
     * @brief Returns whether the tournament is paused.
     *
     * @return true if the tournament is paused.
     */
    bool getIsPaused() const {
        return isPaused;
    }

    /**
     * @brief Returns the score of a specific knight.
     *
     * @param index The index of the knight in the knights vector.
     * @return int The knight's score, or -1 if index is out of bounds.
     */
    int getScore(size_t index) const {
        if (index >= scores.size()) return -1;
        return scores[index];
    }

};

#endif // JOUSTINGTOURNAMENT_H