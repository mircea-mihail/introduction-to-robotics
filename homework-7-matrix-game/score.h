#ifndef SCORE_H
#define SCORE_H

#include <EEPROM.h>
#include "utility.h"

#define SCORE_MEMORY_ADDRESS 900
#define NUMBER_OF_SCORES_KEPT 3 

// i'm keeping the score by timing the time it takes the player to finish a level
class score
{
private:
    unsigned long m_timerBeginning = DEFAULT_TIME_VAL;
    unsigned long m_score = 0;

    void writeScoreToMemory(unsigned long p_scoreToWrite)
    {
        unsigned long scores[3];
        byte scoreIdx = 0;
        for(int addr = SCORE_MEMORY_ADDRESS; addr < NUMBER_OF_SCORES_KEPT; addr += sizeof(unsigned long))
        {
            EEPROM.get(addr, scores[scoreIdx]);
            scoreIdx ++;
        }

        if(p_scoreToWrite > scores[0])
        {
            scores[2] = scores[1];
            scores[1] = scores[0];
            scores[0] = p_scoreToWrite;
        }
        else if(p_scoreToWrite > scores[1])
        {
            scores[2] = scores[1];
            scores[1] = p_scoreToWrite;
        }
        else if(p_scoreToWrite > scores[2])
        {
            scores[2] = p_scoreToWrite;
        }
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
        for(int addr = SCORE_MEMORY_ADDRESS; addr < NUMBER_OF_SCORES_KEPT; addr += sizeof(unsigned long))
        {
            EEPROM.put(addr, 0);
        }
    }
};

#endif