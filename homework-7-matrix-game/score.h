#ifndef SCORE_H
#define SCORE_H

#include <EEPROM.h>
#include "utility.h"

#define SCORE_MEMORY_ADDRESS 900
#define NUMBER_OF_SCORES_KEPT 3 
#define ADDRESS_AFTER_LAST_SCORE (SCORE_MEMORY_ADDRESS + NUMBER_OF_SCORES_KEPT * sizeof(unsigned long))

// scoring related
#define WORST_SCORE 0xFFFFFFFF

#define FIRST_SCORE_IDX 0
#define SECOND_SCORE_IDX 1
#define THIRD_SCORE_IDX 2

#define PLAYER_FIRST_RANK 1

// i'm keeping the score by timing the time it takes the player to finish a level
class score
{
private:
    unsigned long m_timerBeginning = DEFAULT_TIME_VAL;
    unsigned long m_score = 0;

    void updateMemoryScores(unsigned long scores[NUMBER_OF_SCORES_KEPT])
    {
        byte scoreIdx = FIRST_SCORE_IDX;
        for(int addr = SCORE_MEMORY_ADDRESS; addr < ADDRESS_AFTER_LAST_SCORE; addr += sizeof(unsigned long))
        {
            EEPROM.put(addr, scores[scoreIdx ++]);
        }
    }

    void writeScoreToMemory(unsigned long p_scoreToWrite)
    {
        unsigned long scores[NUMBER_OF_SCORES_KEPT];
        byte scoreIdx = FIRST_SCORE_IDX;
        for(int addr = SCORE_MEMORY_ADDRESS; addr < ADDRESS_AFTER_LAST_SCORE; addr += sizeof(unsigned long))
        {
            EEPROM.get(addr, scores[scoreIdx]);
            Serial.print(scores[scoreIdx++]);
            Serial.println();
        }

        if(p_scoreToWrite < scores[FIRST_SCORE_IDX])
        {
            scores[THIRD_SCORE_IDX] = scores[SECOND_SCORE_IDX];
            scores[SECOND_SCORE_IDX] = scores[FIRST_SCORE_IDX];
            scores[FIRST_SCORE_IDX] = p_scoreToWrite;
        }
        else if(p_scoreToWrite < scores[SECOND_SCORE_IDX])
        {
            scores[THIRD_SCORE_IDX] = scores[SECOND_SCORE_IDX];
            scores[SECOND_SCORE_IDX] = p_scoreToWrite;
        }
        else if(p_scoreToWrite < scores[THIRD_SCORE_IDX])
        {
            scores[THIRD_SCORE_IDX] = p_scoreToWrite;
        }

        updateMemoryScores(scores);
    }

public:
    void startCounting()
    {
        m_timerBeginning = millis();
    }

    unsigned long getTheScore()
    {
        m_score = millis() - m_timerBeginning;

        writeScoreToMemory(m_score);        

        return m_score;
    }

    void clearScores()
    {
        for(int addr = SCORE_MEMORY_ADDRESS; addr < ADDRESS_AFTER_LAST_SCORE; addr += sizeof(unsigned long))
        {
            EEPROM.put(addr, WORST_SCORE);
        }
    }

    void printHighScores()
    {
        int playerRank = PLAYER_FIRST_RANK;
        for(int addr = SCORE_MEMORY_ADDRESS; addr < ADDRESS_AFTER_LAST_SCORE; addr += sizeof(unsigned long))
        {
            unsigned long score;
            EEPROM.get(addr, score);
            Serial.print(F("no. "));
            Serial.print(playerRank);
            Serial.print(F(": "));
            Serial.println(score);
            playerRank ++;
        }
    }
};

#endif