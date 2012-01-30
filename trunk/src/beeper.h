/*
 * Authors (alphabetical order)
 * - Bertrand Songis <bsongis@gmail.com>
 * - Bryan J. Rentoul (Gruvin) <gruvin@gmail.com>
 *
 * gruvin9x is based on code named er9x by
 * Author - Erez Raviv <erezraviv@gmail.com>, which is in turn
 * was based on the original (and ongoing) project by Thomas Husterer,
 * th9x -- http://code.google.com/p/th9x/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef BEEPER_H
#define BEEPER_H

extern uint8_t g_beepCnt;
extern uint8_t beepAgain;
extern uint8_t beepAgainOrig;
extern uint8_t beepOn;
extern bool warble;
extern bool warbleC;

extern void beep(uint8_t val);

#define AUDIO_KEYPAD_UP()     beep(0)
#define AUDIO_KEYPAD_DOWN()   beep(0)
#define AUDIO_MENUS()         beep(0)
#define AUDIO_TRIM()          beep(1)
#define AUDIO_WARNING2()      beep(2)
#define AUDIO_WARNING1()      beep(3)
#define AUDIO_ERROR()         beep(4)
#define AUDIO_TIMER_30()      { beepAgain=2; beep(2); }
#define AUDIO_TIMER_20()      { beepAgain=1; beep(2); }
#define AUDIO_TIMER_10()      beep(2)
#define AUDIO_TIMER_LT3()     beep(2)
#define AUDIO_MINUTE_BEEP()   beep(2)
#define AUDIO_INACTIVITY()    beep(3)
#define AUDIO_MIX_WARNING_1() beep(1)
#define AUDIO_MIX_WARNING_3() beep(1)

#define IS_AUDIO_BUSY() (g_beepCnt || beepAgain || beepOn)

FORCEINLINE void AUDIO_HEARTBEAT()
{
    //cnt >/=0
    //beepon/off
    //beepagain y/n
    if(g_beepCnt) {
        if(!beepAgainOrig) {
            beepAgainOrig = g_beepCnt;
            beepOn = true;
        }
        g_beepCnt--;
    }
    else
    {
        if(beepAgain && beepAgainOrig) {
            beepOn = !beepOn;
            g_beepCnt = beepOn ? beepAgainOrig : 8;
            if(beepOn) beepAgain--;
        }
        else {
            beepAgainOrig = 0;
            beepOn = false;
            warble = false;
        }
    }

    // G: use original external buzzer for beeps
    if (beepOn) {
      //static bool warbleC;
      warbleC = warble && !warbleC;
      if (warbleC)
        PORTE &= ~(1 << OUT_E_BUZZER); // speaker output 'low'
      else
        PORTE |= (1 << OUT_E_BUZZER); // speaker output 'high'
    }
    else {
      PORTE &= ~(1 << OUT_E_BUZZER); // speaker output 'low'
    }
}

#endif
