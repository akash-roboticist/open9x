/*
 * Authors (alphabetical order)
 * - Bertrand Songis <bsongis@gmail.com>
 * - Bryan J. Rentoul (Gruvin) <gruvin@gmail.com>
 * - Cameron Weeks <th9xer@gmail.com>
 * - Erez Raviv
 * - Jean-Pierre Parisy
 * - Karl Szmutny <shadow@privy.de>
 * - Michael Blandford
 * - Michal Hlavinka
 * - Pat Mackenzie
 * - Philip Moss
 * - Rob Thomson
 * - Romolo Manfredini <romolo.manfredini@gmail.com>
 * - Thomas Husterer
 *
 * open9x is based on code named
 * gruvin9x by Bryan J. Rentoul: http://code.google.com/p/gruvin9x/,
 * er9x by Erez Raviv: http://code.google.com/p/er9x/,
 * and the original (and ongoing) project by
 * Thomas Husterer, th9x: http://code.google.com/p/th9x/
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

#include "open9x.h"

const pm_uchar sticks[] PROGMEM = {
#include "sticks.lbm"
};

enum EnumTabDiag {
  e_Setup,
#if defined(SDCARD)
  e_Sd,
#endif
  e_Trainer,
  e_Vers,
  e_Keys,
  e_Ana,
#if defined(PCBSKY9X)
  e_Hardware,
#endif
  e_Calib
};

void menuProcSetup(uint8_t event);
#if defined(SDCARD)
void menuProcSd(uint8_t event);
#endif
void menuProcTrainer(uint8_t event);
void menuProcDiagVers(uint8_t event);
void menuProcDiagKeys(uint8_t event);
void menuProcDiagAna(uint8_t event);
void menuProcHardware(uint8_t event);
void menuProcDiagCalib(uint8_t event);

const MenuFuncP_PROGMEM menuTabDiag[] PROGMEM = {
  menuProcSetup,
#if defined(SDCARD)
  menuProcSd,
#endif
  menuProcTrainer,
  menuProcDiagVers,
  menuProcDiagKeys,
  menuProcDiagAna,
#if defined(PCBSKY9X)
  menuProcHardware,
#endif
  menuProcDiagCalib
};

#define GENERAL_PARAM_OFS   (2+16*FW)

#if defined(GRAPHICS)
void displaySlider(uint8_t x, uint8_t y, uint8_t value, uint8_t attr)
{
  lcd_putc(GENERAL_PARAM_OFS+2*FW+(value*FW), y, '$');
  lcd_hline(GENERAL_PARAM_OFS, y+3, 5*FW-1, SOLID);
  if (attr && (!(attr & BLINK) || !BLINK_ON_PHASE)) lcd_filled_rect(GENERAL_PARAM_OFS, y, 5*FW-1, FH-1);
}
#define SLIDER(y, value, min, max, label, values, event, attr) { \
          int8_t tmp = value; \
          displaySlider(GENERAL_PARAM_OFS, y, tmp, attr); \
          value = selectMenuItem(GENERAL_PARAM_OFS, y, label, NULL, tmp, min, max, attr, event); \
        }
#else
#define SLIDER(y, value, min, max, label, values, event, attr) \
          value = selectMenuItem(GENERAL_PARAM_OFS, y, label, values, value, min, max, attr, event)
#endif

enum menuProcSetupItems {
#if defined(RTCLOCK)
  ITEM_SETUP_RTC,
#endif
  ITEM_SETUP_BEEPER_MODE,
  ITEM_SETUP_BEEPER_LENGTH,
#if defined(AUDIO)
  ITEM_SETUP_SPEAKER_PITCH,
#endif
#if defined(VOICE)
  ITEM_SETUP_SPEAKER_VOLUME,
#endif
#if defined(HAPTIC)
  ITEM_SETUP_HAPTIC_MODE,
  ITEM_SETUP_HAPTIC_LENGTH,
  ITEM_SETUP_HAPTIC_STRENGTH,
#endif
#if defined(PCBSKY9X)
  ITEM_SETUP_BRIGHTNESS,
#endif
  ITEM_SETUP_CONTRAST,
  ITEM_SETUP_BATTERY_WARNING,
#if defined(PCBSKY9X)
  ITEM_SETUP_CAPACITY_WARNING,
  ITEM_SETUP_TEMPERATURE_WARNING,
#endif
  ITEM_SETUP_INACTIVITY_ALARM,
#if defined(BLUETOOTH)
  ITEM_SETUP_BT_BAUDRATE,
#endif
#if defined(ROTARY_ENCODERS)
  ITEM_SETUP_RE_NAVIGATION,
#endif
  ITEM_SETUP_FILTER_ADC,
  ITEM_SETUP_THROTTLE_REVERSED,
  ITEM_SETUP_MINUTE_BEEP,
  ITEM_SETUP_COUNTDOWN_BEEP,
  ITEM_SETUP_FLASH_BEEP,
  ITEM_SETUP_BACKLIGHT_MODE,
  ITEM_SETUP_BACKLIGHT_DELAY,
#if defined(SPLASH)
  ITEM_SETUP_DISABLE_SPLASH,
#endif
  ITEM_SETUP_MEMORY_WARNING,
  ITEM_SETUP_ALARM_WARNING,
#if defined(FRSKY)
  ITEM_SETUP_TIMEZONE,
  ITEM_SETUP_GPSFORMAT,
#endif
  ITEM_SETUP_RX_CHANNEL_ORD,
  ITEM_SETUP_STICK_MODE_LABELS,
  ITEM_SETUP_STICK_MODE,
  ITEM_SETUP_MAX
};

void menuProcSetup(uint8_t event)
{
#ifdef RTCLOCK
#define RTC_ZEROS 5,
#else
#define RTC_ZEROS
#endif
#ifdef AUDIO
#define AUDIO_ZEROS  0,
#else
#define AUDIO_ZEROS
#endif
#ifdef VOICE
#define VOICE_ZEROS  0,
#else
#define VOICE_ZEROS
#endif
#ifdef HAPTIC
#define HAPTIC_ZEROS 0, 0, 0,
#else
#define HAPTIC_ZEROS
#endif
#ifdef SPLASH
#define SPLASH_ZEROS 0,
#else
#define SPLASH_ZEROS
#endif
#ifdef FRSKY
#define FRSKY_ZEROS  0, 0,
#else
#define FRSKY_ZEROS
#endif
#ifdef PCBSKY9X
#define ARM_ZEROS  0, 0, 0,
#else
#define ARM_ZEROS
#endif
#ifdef BLUETOOTH
#define BLUETOOTH_ZEROS 0,
#else
#define BLUETOOTH_ZEROS
#endif
#ifdef ROTARY_ENCODERS
#define ROTARY_ENCODERS_ZEROS 0,
#else
#define ROTARY_ENCODERS_ZEROS
#endif

  MENU(STR_MENURADIOSETUP, menuTabDiag, e_Setup, ITEM_SETUP_MAX+1, {0, RTC_ZEROS 0, 0, AUDIO_ZEROS VOICE_ZEROS HAPTIC_ZEROS ARM_ZEROS BLUETOOTH_ZEROS ROTARY_ENCODERS_ZEROS 0, 0, 0, 0, 0, 0, 0, 0, 0, SPLASH_ZEROS 0, 0, 0, 0, FRSKY_ZEROS 0, (uint8_t)-1, 1});

  uint8_t sub = m_posVert - 1;

#if defined(RTCLOCK)
  static struct gtm t;
#endif

  for (uint8_t i=0; i<7; i++) {
    uint8_t y = 1*FH + i*FH;
    uint8_t k = i+s_pgOfs;
    uint8_t blink = ((s_editMode>0) ? BLINK|INVERS : INVERS);
    uint8_t attr = (sub == k ? blink : 0);

    switch(k) {
#if defined(RTCLOCK)
      case ITEM_SETUP_RTC:
        lcd_putsLeft(y, PSTR("Time"));
        lcd_putc(FW*7+6, y, '-'); lcd_putc(FW*10+4, y, '-');
        lcd_putc(FW*15+5, y, ':'); lcd_putc(FW*18+3, y, ':');
        for (uint8_t j=0; j<6; j++) { // 3 settings each for date and time (YMD and HMS)
          uint8_t rowattr = (m_posHorz==j) ? attr : 0;
          switch (j) {
            case 0:
              lcd_outdezAtt(FW*7+6, y, t.tm_year+1900, rowattr);
              if (rowattr && (s_editMode>0 || p1valdiff)) t.tm_year = checkIncDec(event, t.tm_year, 112, 200, 0);
              break;
            case 1:
              lcd_outdezNAtt(FW*10+4, y, t.tm_mon+1, rowattr|LEADING0, 2);
              if (rowattr && (s_editMode>0 || p1valdiff)) t.tm_mon = checkIncDec(event, t.tm_mon, 0, 11, 0);
              break;
            case 2:
            {
              int16_t year = 1900 + t.tm_year;
              int8_t dlim = (((((year%4==0) && (year %100!=0)) || (year%400==0)) && (t.tm_mon==1)) ? 1 : 0);
              int8_t dmon[] = {31,28,31,30,31,30,31,31,30,31,30,31}; // TODO in flash
              dlim += dmon[t.tm_mon];
              lcd_outdezNAtt(FW*13+2, y, t.tm_mday, rowattr|LEADING0, 2);
              if (rowattr && (s_editMode>0 || p1valdiff)) t.tm_mday = checkIncDec(event, t.tm_mday, 1, dlim, 0);
              break;
            }
            case 3:
              lcd_outdezNAtt(FW*15+5, y, t.tm_hour, rowattr|LEADING0, 2);
              if (rowattr && (s_editMode>0 || p1valdiff)) t.tm_hour = checkIncDec( event, t.tm_hour, 0, 23, 0);
              break;
            case 4:
              lcd_outdezNAtt(FW*18+3, y, t.tm_min, rowattr|LEADING0, 2);
              if (rowattr && (s_editMode>0 || p1valdiff)) t.tm_min = checkIncDec( event, t.tm_min, 0, 59, 0);
              break;
            case 5:
              lcd_outdezNAtt(FW*21+2, y, t.tm_sec, rowattr|LEADING0, 2);
              if (rowattr && (s_editMode>0 || p1valdiff)) t.tm_sec = checkIncDec( event, t.tm_sec, 0, 59, 0);
              break;
          }
        }

        if (attr && s_editMode<=0 && event == EVT_KEY_FIRST(KEY_MENU)) {
          // set the date and time into RTC chip
          rtc_settime(&t);
        }
        else if (sub!=ITEM_SETUP_RTC || s_editMode<=0) {
          gettime(&t);
        }
        break;
#endif

      case ITEM_SETUP_BEEPER_MODE:
        g_eeGeneral.beeperMode = selectMenuItem(GENERAL_PARAM_OFS, y, STR_BEEPERMODE, STR_VBEEPMODE, g_eeGeneral.beeperMode, -2, 1, attr, event);
#if defined(FRSKY)
        if (attr && checkIncDec_Ret) FRSKY_setModelAlarms();
#endif
        break;

      case ITEM_SETUP_BEEPER_LENGTH:
        SLIDER(y, g_eeGeneral.beeperLength, -2, 2, STR_BEEPERLEN, STR_VBEEPLEN, event, attr);
        break;

#if defined(AUDIO)
      case ITEM_SETUP_SPEAKER_PITCH:
        lcd_putsLeft( y, STR_SPKRPITCH);
        lcd_outdezAtt(GENERAL_PARAM_OFS, y, g_eeGeneral.speakerPitch, attr|LEFT);
        if (attr) {
          CHECK_INCDEC_GENVAR(event, g_eeGeneral.speakerPitch, 0, 20);
        }
        break;
#endif

#if defined(VOICE)
      case ITEM_SETUP_SPEAKER_VOLUME:
      {
        lcd_putsLeft(y, STR_SPEAKER_VOLUME);
#if defined(PCBSKY9X)
        lcd_outdezAtt(GENERAL_PARAM_OFS, y, g_eeGeneral.speakerVolume, attr|LEFT);
        if (attr) {
          CHECK_INCDEC_GENVAR(event, g_eeGeneral.speakerVolume, 0, NUM_VOL_LEVELS-1);
        }
#else
        uint8_t b ;
        b = g_eeGeneral.speakerVolume+7;
        lcd_outdezAtt(GENERAL_PARAM_OFS, y, b, attr|LEFT);
        if (attr) {
          CHECK_INCDEC_GENVAR(event, b, 0, 7);
          if (checkIncDec_Ret) {
            g_eeGeneral.speakerVolume = (int8_t)b-7;
            SET_VOLUME(b);
          }
        }
#endif
        break;
      }
#endif

#if defined(HAPTIC)
      case ITEM_SETUP_HAPTIC_MODE:
        g_eeGeneral.hapticMode = selectMenuItem(GENERAL_PARAM_OFS, y, STR_HAPTICMODE, STR_VBEEPMODE, g_eeGeneral.hapticMode, -2, 1, attr, event);
        break;

      case ITEM_SETUP_HAPTIC_LENGTH:
        SLIDER(y, g_eeGeneral.hapticLength, -2, 2, STR_HAPTICLENGTH, STR_VBEEPLEN, event, attr);
        break;

      case ITEM_SETUP_HAPTIC_STRENGTH:
        lcd_putsLeft( y, STR_HAPTICSTRENGTH);
        lcd_outdezAtt(GENERAL_PARAM_OFS, y, g_eeGeneral.hapticStrength, attr|LEFT);
        if (attr) {
          CHECK_INCDEC_GENVAR(event, g_eeGeneral.hapticStrength, 0, 5);
        }
        break;
#endif

#if defined(PCBSKY9X)
      case ITEM_SETUP_BRIGHTNESS:
        lcd_putsLeft(y, STR_BRIGHTNESS);
        lcd_outdezAtt(GENERAL_PARAM_OFS, y, 100-g_eeGeneral.backlightBright, attr|LEFT) ;
        if(attr) {
          uint8_t b ;
          b = 100 - g_eeGeneral.backlightBright;
          CHECK_INCDEC_GENVAR(event, b, 0, 100);
          g_eeGeneral.backlightBright = 100 - b;
        }
        break;
#endif

      case ITEM_SETUP_CONTRAST:
        lcd_putsLeft( y, STR_CONTRAST);
        lcd_outdezAtt(GENERAL_PARAM_OFS,y,g_eeGeneral.contrast, attr|LEFT);
        if(attr) {
          CHECK_INCDEC_GENVAR(event, g_eeGeneral.contrast, 10, 45);
          lcdSetContrast();
        }
        break;

      case ITEM_SETUP_BATTERY_WARNING:
        lcd_putsLeft( y,STR_BATTERYWARNING);
        putsVolts(GENERAL_PARAM_OFS, y, g_eeGeneral.vBatWarn, attr|LEFT);
        if(attr) CHECK_INCDEC_GENVAR(event, g_eeGeneral.vBatWarn, 40, 120); //4-12V
        break;

#if defined(PCBSKY9X)
      case ITEM_SETUP_CAPACITY_WARNING:
        lcd_putsLeft(y, STR_CAPAWARNING);
        putsTelemetryValue(GENERAL_PARAM_OFS, y, g_eeGeneral.mAhWarn*50, UNIT_MAH, attr|LEFT) ;
        if(attr) CHECK_INCDEC_GENVAR(event, g_eeGeneral.mAhWarn, 0, 100);
        break;

      case ITEM_SETUP_TEMPERATURE_WARNING:
        lcd_putsLeft(y, STR_TEMPWARNING);
        putsTelemetryValue(GENERAL_PARAM_OFS, y, g_eeGeneral.temperatureWarn, UNIT_DEGREES, attr|LEFT) ;
        if(attr) CHECK_INCDEC_GENVAR(event, g_eeGeneral.temperatureWarn, 0, 120); // 0 means no alarm
        break;
#endif

      case ITEM_SETUP_INACTIVITY_ALARM:
        lcd_putsLeft( y,STR_INACTIVITYALARM);
        lcd_outdezAtt(GENERAL_PARAM_OFS, y, g_eeGeneral.inactivityTimer, attr|LEFT);
        lcd_putc(lcdLastPos, y, 'm');
        if(attr) g_eeGeneral.inactivityTimer = checkIncDec(event, g_eeGeneral.inactivityTimer, 0, 250, EE_GENERAL); //0..250minutes
        break;

#if defined(BLUETOOTH)
      case ITEM_SETUP_BT_BAUDRATE:
        g_eeGeneral.btBaudrate = selectMenuItem(GENERAL_PARAM_OFS, y, PSTR("BT Baudrate"), PSTR("\005115k 9600 19200"), g_eeGeneral.btBaudrate, 0, 2, attr, event);
        if (attr && checkIncDec_Ret) {
          btInit();
        }
        break;
#endif

#if defined(ROTARY_ENCODERS)
      case ITEM_SETUP_RE_NAVIGATION:
        g_eeGeneral.reNavigation = selectMenuItem(GENERAL_PARAM_OFS, y, STR_RENAVIG, STR_VRENAVIG, g_eeGeneral.reNavigation, 0, ROTARY_ENCODERS, attr, event);
        if (attr && checkIncDec_Ret) {
          for (uint8_t i=0; i<ROTARY_ENCODERS; i++)
            g_rotenc[i] = 0;
          p1valdiff = 0;
          scrollRE = 0;
        }
        break;
#endif

      case ITEM_SETUP_FILTER_ADC:
        g_eeGeneral.filterInput = selectMenuItem(GENERAL_PARAM_OFS, y, STR_FILTERADC, STR_VFILTERADC, g_eeGeneral.filterInput, 0, 2, attr, event);
        break;

      case ITEM_SETUP_THROTTLE_REVERSED:
        g_eeGeneral.throttleReversed = onoffMenuItem( g_eeGeneral.throttleReversed, GENERAL_PARAM_OFS, y, STR_THROTTLEREVERSE, attr, event ) ;
        break;

      case ITEM_SETUP_MINUTE_BEEP:
        g_eeGeneral.minuteBeep = onoffMenuItem( g_eeGeneral.minuteBeep, GENERAL_PARAM_OFS, y, STR_MINUTEBEEP, attr, event ) ;
        break;

      case ITEM_SETUP_COUNTDOWN_BEEP:
        g_eeGeneral.preBeep = onoffMenuItem( g_eeGeneral.preBeep, GENERAL_PARAM_OFS, y, STR_BEEPCOUNTDOWN, attr, event ) ;
        break;

      case ITEM_SETUP_FLASH_BEEP:
        g_eeGeneral.flashBeep = onoffMenuItem( g_eeGeneral.flashBeep, GENERAL_PARAM_OFS, y, STR_FLASHONBEEP, attr, event ) ;
        break;

      case ITEM_SETUP_BACKLIGHT_MODE:
        g_eeGeneral.backlightMode = selectMenuItem(GENERAL_PARAM_OFS, y, STR_BLMODE, STR_VBLMODE, g_eeGeneral.backlightMode, e_backlight_mode_off, e_backlight_mode_on, attr, event);
        break;

      case ITEM_SETUP_BACKLIGHT_DELAY:
        lcd_putsLeft( y, STR_BLDELAY);
        lcd_outdezAtt(GENERAL_PARAM_OFS, y, g_eeGeneral.lightAutoOff*5, attr|LEFT);
        lcd_putc(lcdLastPos, y, 's');
        if(attr) CHECK_INCDEC_GENVAR(event, g_eeGeneral.lightAutoOff, 0, 600/5);
        break;

#ifdef SPLASH
      case ITEM_SETUP_DISABLE_SPLASH:
      {
        uint8_t b = 1-g_eeGeneral.disableSplashScreen;
        g_eeGeneral.disableSplashScreen = 1 - onoffMenuItem( b, GENERAL_PARAM_OFS, y, STR_SPLASHSCREEN, attr, event ) ;
        break;
      }
#endif

      case ITEM_SETUP_MEMORY_WARNING:
      {
        uint8_t b = 1-g_eeGeneral.disableMemoryWarning;
        g_eeGeneral.disableMemoryWarning = 1 - onoffMenuItem( b, GENERAL_PARAM_OFS, y, STR_MEMORYWARNING, attr, event ) ;
        break;
      }

      case ITEM_SETUP_ALARM_WARNING:
      {
        uint8_t b = 1-g_eeGeneral.disableAlarmWarning;
        g_eeGeneral.disableAlarmWarning = 1 - onoffMenuItem( b, GENERAL_PARAM_OFS, y, STR_ALARMWARNING, attr, event ) ;
        break;
      }

#if defined(FRSKY)
      case ITEM_SETUP_TIMEZONE:
        lcd_putsLeft(y, STR_TIMEZONE);
        lcd_outdezAtt(GENERAL_PARAM_OFS, y, g_eeGeneral.timezone, attr|LEFT);
        if (attr) CHECK_INCDEC_GENVAR(event, g_eeGeneral.timezone, -12, 12);
        break;

      case ITEM_SETUP_GPSFORMAT:
        g_eeGeneral.gpsFormat = selectMenuItem(GENERAL_PARAM_OFS, y, STR_GPSCOORD, STR_GPSFORMAT, g_eeGeneral.gpsFormat, 0, 1, attr, event);
        break;
#endif

      case ITEM_SETUP_RX_CHANNEL_ORD:
        lcd_putsLeft( y,STR_RXCHANNELORD);//   RAET->AETR
        for (uint8_t i=1; i<=4; i++)
          putsChnLetter(GENERAL_PARAM_OFS - FW + i*FW, y, channel_order(i), attr);
        if (attr) CHECK_INCDEC_GENVAR(event, g_eeGeneral.templateSetup, 0, 23);
        break;

      case ITEM_SETUP_STICK_MODE_LABELS:
        lcd_putsLeft(y, STR_MODE);
        for (uint8_t i=0; i<4; i++) lcd_img((6+4*i)*FW, y, sticks, i, 0);
        break;

      case ITEM_SETUP_STICK_MODE:
        lcd_putcAtt(2*FW, y, '1'+g_eeGeneral.stickMode, attr);
        for (uint8_t i=0; i<4; i++) putsChnRaw( (6+4*i)*FW, y, pgm_read_byte(modn12x3 + 4*g_eeGeneral.stickMode + i), 0);
        if (attr && s_editMode>0) {
          CHECK_INCDEC_GENVAR(event, g_eeGeneral.stickMode, 0, 3);
        }
        else if (stickMode != g_eeGeneral.stickMode) {
          pausePulses();
          stickMode = g_eeGeneral.stickMode;
          checkTHR();
          resumePulses();
          clearKeyEvents();
        }
        break;
    }
  }
}

#if defined(PCBSKY9X)
extern uint32_t sdAvailableAudioFiles; // TODO move that!
#endif

#if defined(SDCARD)
void menuProcSdInfo(uint8_t event)
{
  SIMPLE_SUBMENU(PSTR("SD INFO"), 1);

  lcd_putsLeft(2*FH, PSTR("Type:"));
  lcd_puts(10*FW, 2*FH, SD_IS_HC() ? STR_SDHC_CARD : STR_SD_CARD);

  lcd_putsLeft(3*FH, PSTR("Size:"));
  lcd_outdezAtt(10*FW, 3*FH, SD_GET_SIZE_MB(), LEFT);
  lcd_putc(lcdLastPos, 3*FH, 'M');

  lcd_putsLeft(4*FH, PSTR("Sectors:"));
  lcd_outdezAtt(10*FW, 4*FH, SD_GET_BLOCKNR()/1000, LEFT);
  lcd_putc(lcdLastPos, 4*FH, 'k');

  lcd_putsLeft(5*FH, PSTR("Speed:"));
  lcd_outdezAtt(10*FW, 5*FH, SD_GET_SPEED()/1000, LEFT);
  lcd_puts(lcdLastPos, 5*FH, "kb/s");

  // lcd_putsLeft(7*FH, PSTR("CSD:"));
}

void menuProcSd(uint8_t event)
{
  FILINFO fno;
  DIR dir;
  char *fn;   /* This function is assuming non-Unicode cfg. */
#if _USE_LFN
  TCHAR lfn[_MAX_LFN + 1];
  fno.lfname = lfn;
  fno.lfsize = sizeof(lfn);
#else
  char lfn[SD_SCREEN_FILE_LENGTH];
#endif

  uint8_t _event = event;
  if (s_menu_count) {
    event = 0;
  }

  SIMPLE_MENU(SD_IS_HC() ? STR_SDHC_CARD : STR_SD_CARD, menuTabDiag, e_Sd, 1+reusableBuffer.sd.count);

  if (s_editMode > 0)
    s_editMode = 0;

  switch(event) {
    case EVT_ENTRY:
      f_chdir("/");
      reusableBuffer.sd.offset = 65535;
      break;
#if defined(ROTARY_ENCODERS)
    case EVT_KEY_FIRST(BTN_REa):
    case EVT_KEY_FIRST(BTN_REb):
      if (!navigationRotaryEncoder(event))
        break;
      // no break
#endif
    case EVT_KEY_FIRST(KEY_RIGHT):
    case EVT_KEY_FIRST(KEY_MENU):
    {
      if (m_posVert > 0) {
        uint8_t index = m_posVert-1-s_pgOfs;
        if (reusableBuffer.sd.flags[index]) {
          killEvents(event);
          f_chdir(reusableBuffer.sd.lines[index]);
          s_pgOfs = 0;
          m_posVert = 1;
          reusableBuffer.sd.offset = 255;
        }
      }
      break;
    }
#if defined(ROTARY_ENCODERS)
    case EVT_KEY_LONG(BTN_REa):
    case EVT_KEY_LONG(BTN_REb):
      if (!navigationRotaryEncoder(event))
        break;
      // no break
#endif
    case EVT_KEY_LONG(KEY_MENU):
      killEvents(event);
      if (m_posVert == 0) {
        s_menu[s_menu_count++] = STR_SD_INFO;
        s_menu[s_menu_count++] = STR_SD_FORMAT;
      }
      else {
#if defined(PCBSKY9X)
        uint8_t index = m_posVert-1-s_pgOfs;
        char * line = reusableBuffer.sd.lines[index];
        if (!strcmp(line+strlen(line)-4, SOUNDS_EXT)) {
          s_menu[s_menu_count++] = STR_PLAY_FILE;
        }
#endif
        s_menu[s_menu_count++] = STR_DELETE_FILE;
        s_menu[s_menu_count++] = STR_RENAME_FILE;
        s_menu[s_menu_count++] = STR_COPY_FILE;
      }
      break;
  }

#define SD_ALPHABETICAL_ORDER
#ifdef SD_ALPHABETICAL_ORDER
  if (reusableBuffer.sd.offset != s_pgOfs) {
    char hidden_line[SD_SCREEN_FILE_LENGTH+1] = "";
    uint8_t hidden_flag = 0;

    if (s_pgOfs == 0) {
      reusableBuffer.sd.offset = 0;
    }
    else if (s_pgOfs >= reusableBuffer.sd.count-7) {
      reusableBuffer.sd.offset = s_pgOfs+1;
    }
    else if (s_pgOfs > reusableBuffer.sd.offset) {
      memcpy(hidden_line, reusableBuffer.sd.lines[0], sizeof(hidden_line));
      hidden_flag = reusableBuffer.sd.flags[0];
    }
    else {
      memcpy(hidden_line, reusableBuffer.sd.lines[6], sizeof(hidden_line));
      hidden_flag = reusableBuffer.sd.flags[6];
    }

    memset(reusableBuffer.sd.lines, 0, sizeof(reusableBuffer.sd.lines));
    memset(reusableBuffer.sd.flags, 0, sizeof(reusableBuffer.sd.flags));

    reusableBuffer.sd.count = 0;

    FRESULT res = f_opendir(&dir, ".");        /* Open the directory */
    if (res == FR_OK) {
      for (;;) {
        res = f_readdir(&dir, &fno);                   /* Read a directory item */
        if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
        if (fno.fname[0] == '.' && fno.fname[1] == '\0') continue;             /* Ignore dot entry */
#if _USE_LFN
        fn = *fno.lfname ? fno.lfname : fno.fname;
#else
        fn = fno.fname;
#endif
        reusableBuffer.sd.count++;

        bool isdir = (fno.fattrib & AM_DIR);

        for (uint8_t i=0; i<7; i++) {
          if (s_pgOfs >= reusableBuffer.sd.offset) {
            char *line = reusableBuffer.sd.lines[i];
            if ((!hidden_line[0] || (!isdir && hidden_flag) || (strcmp(hidden_line, fn) < 0)) && (line[0] == '\0' || (isdir&&!reusableBuffer.sd.flags[i]) || strcmp(fn, line) < 0)) {
              if (i < 7-1) {
                memmove(reusableBuffer.sd.lines[i+1], line, sizeof(reusableBuffer.sd.lines[i]) * (7-1-i));
                memmove(&reusableBuffer.sd.flags[i+1], &reusableBuffer.sd.flags[i], sizeof(reusableBuffer.sd.flags[i]) * (7-1-i));
              }
              strncpy(line, fn, SD_SCREEN_FILE_LENGTH);
              reusableBuffer.sd.flags[i] = isdir;
              break;
            }
          }
          else {
            char *line = reusableBuffer.sd.lines[6-i];
            if ((!hidden_line[0] || (isdir && !hidden_flag) || (strcmp(hidden_line, fn) > 0)) && (line[0] == '\0' || (!isdir&&reusableBuffer.sd.flags[6-i]) || strcmp(fn, line) > 0)) {
              if (i < 7-1) {
                memmove(reusableBuffer.sd.lines, reusableBuffer.sd.lines[1], sizeof(reusableBuffer.sd.lines[0]) * (6-i));
                memmove(&reusableBuffer.sd.flags, &reusableBuffer.sd.flags[1], sizeof(reusableBuffer.sd.flags[0]) * (6-i));
              }
              strncpy(line, fn, SD_SCREEN_FILE_LENGTH);
              reusableBuffer.sd.flags[6-i] = isdir;
              break;
            }
          }
        }
      }
    }
  }
#else
  if (reusableBuffer.sd.offset != s_pgOfs) {
    memset(reusableBuffer.sd.lines, 0, sizeof(reusableBuffer.sd.lines));
    memset(reusableBuffer.sd.flags, 0, sizeof(reusableBuffer.sd.flags));
    reusableBuffer.sd.offset = s_pgOfs;
    reusableBuffer.sd.count = 0;
    uint16_t offset = 0;
    uint8_t count = 0;
    FRESULT res = f_opendir(&dir, ".");        /* Open the directory */
    if (res == FR_OK) {
      for (;;) {
        res = f_readdir(&dir, &fno);                   /* Read a directory item */
        if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
        if (fno.fname[0] == '.' && fno.fname[1] == '\0') continue;             /* Ignore dot entry */
#if _USE_LFN
        fn = *fno.lfname ? fno.lfname : fno.fname;
#else
        fn = fno.fname;
#endif
        reusableBuffer.sd.count++;
        if (offset < s_pgOfs) {
          offset++;
          continue;
        }

        if (count < 7) {
          reusableBuffer.sd.flags[count] = (fno.fattrib & AM_DIR);
          strncpy(reusableBuffer.sd.lines[count], fn, SD_SCREEN_FILE_LENGTH);
          count++;
        }
      }
    }
  }
#endif

  reusableBuffer.sd.offset = s_pgOfs;

  for (uint8_t i=0; i<7; i++) {
    uint8_t y = FH+i*FH;
    uint8_t x = 0;
    uint8_t attr = (m_posVert-1-s_pgOfs == i ? BSS|INVERS : BSS);
    if (reusableBuffer.sd.flags[i]) { lcd_putcAtt(0, y, '[', attr); x += FW; }
    lcd_putsAtt(x, y, reusableBuffer.sd.lines[i], attr);
    if (reusableBuffer.sd.flags[i]) { lcd_putcAtt(lcdLastPos, y, ']', attr); }
  }

  if (s_menu_count) {
    const char * result = displayMenu(_event);
    if (result) {
      uint8_t index = m_posVert-1-s_pgOfs;
      if (result == STR_SD_INFO) {
        pushMenu(menuProcSdInfo);
      }
      else if (result == STR_SD_FORMAT) {
        displayPopup(PSTR("Formatting...")); // TODO translations
        if (f_mkfs(0, 1, 0) == FR_OK) {
#if defined(PCBSKY9X)
          sdAvailableAudioFiles = 0;
#endif
          f_chdir("/");
          reusableBuffer.sd.offset = -1;
        }
        else {
          s_warning = STR_SDCARD_ERROR;
        }
      }
      else if (result == STR_DELETE_FILE) {
        f_getcwd(lfn, SD_SCREEN_FILE_LENGTH);
        strcat_P(lfn, PSTR("/"));
        strcat(lfn, reusableBuffer.sd.lines[index]);
        f_unlink(lfn);
        strncpy(statusLineMsg, reusableBuffer.sd.lines[index], 13);
        strcpy_P(statusLineMsg+min((uint8_t)strlen(statusLineMsg), (uint8_t)13), STR_REMOVED);
        showStatusLine();
        if ((uint16_t)m_posVert == reusableBuffer.sd.count) m_posVert--;
        reusableBuffer.sd.offset = s_pgOfs-1;
      }
#if defined(PCBSKY9X)
      else if (result == STR_PLAY_FILE) {
        f_getcwd(lfn, SD_SCREEN_FILE_LENGTH);
        strcat(lfn, "/");
        strcat(lfn, reusableBuffer.sd.lines[index]);
        audioQueue.playFile(lfn, PLAY_NOW);
      }
#endif
    }
  }
}
#endif

void menuProcTrainer(uint8_t event)
{
  uint8_t y;
  bool slave = SLAVE_MODE;

  MENU(STR_MENUTRAINER, menuTabDiag, e_Trainer, slave ? 1 : 7, {0, 2, 2, 2, 2, 0/*, 0*/});

  if (slave) { // i am the slave
    lcd_puts(7*FW, 3*FH, STR_SLAVE);
  }
  else {
    uint8_t attr;
    uint8_t blink = ((s_editMode>0) ? BLINK|INVERS : INVERS);

    lcd_puts(3*FW, 1*FH, STR_MODESRC);

    y = 2*FH;

    for (uint8_t i=1; i<=NUM_STICKS; i++) {
      uint8_t chan = channel_order(i);
      volatile TrainerMix *td = &g_eeGeneral.trainer.mix[chan-1];

      putsChnRaw(0, y, chan, 0);

      for (uint8_t j=0; j<3; j++) {

        attr = ((m_posVert==i && m_posHorz==j) ? blink : 0);

        switch(j) {
          case 0:
            lcd_putsiAtt(4*FW, y, STR_TRNMODE, td->mode, attr);
            if (attr&BLINK) CHECK_INCDEC_GENVAR(event, td->mode, 0, 2);
            break;

          case 1:
            lcd_outdezAtt(11*FW, y, td->studWeight, attr);
            if (attr&BLINK) CHECK_INCDEC_GENVAR(event, td->studWeight, -100, 100);
            break;

          case 2:
            lcd_putsiAtt(12*FW, y, STR_TRNCHN, td->srcChn, attr);
            if (attr&BLINK) CHECK_INCDEC_GENVAR(event, td->srcChn, 0, 3);
            break;
        }
      }
      y += FH;
    }

    attr = (m_posVert==5) ? blink : 0;
    lcd_putsLeft(6*FH, STR_MULTIPLIER);
    lcd_outdezAtt(LEN_MULTIPLIER*FW+3*FW, 6*FH, g_eeGeneral.PPM_Multiplier+10, attr|PREC1);
    if (attr) CHECK_INCDEC_GENVAR(event, g_eeGeneral.PPM_Multiplier, -10, 40);

    attr = (m_posVert==6) ? blink : 0;
    lcd_putsAtt(0*FW, 7*FH, STR_CAL, attr);
    for (uint8_t i=0; i<4; i++) {
      uint8_t x = (i*8+16)*FW/2;
#if defined (DECIMALS_DISPLAYED)
      lcd_outdezAtt(x, 7*FH, (g_ppmIns[i]-g_eeGeneral.trainer.calib[i])*2, PREC1);
#else
      lcd_outdezAtt(x, 7*FH, (g_ppmIns[i]-g_eeGeneral.trainer.calib[i])/5, 0);
#endif
    }

    if (attr) {
      if (event==EVT_KEY_FIRST(KEY_MENU)){
        s_editMode = -1;
        memcpy(g_eeGeneral.trainer.calib, g_ppmIns, sizeof(g_eeGeneral.trainer.calib));
        eeDirty(EE_GENERAL);
        AUDIO_KEYPAD_UP();
      }
    }
  }
}

void menuProcDiagVers(uint8_t event)
{
  SIMPLE_MENU(STR_MENUVERSION, menuTabDiag, e_Vers, 1);

  lcd_putsLeft(2*FH, stamp1);
  lcd_putsLeft(3*FH, stamp2);
  lcd_putsLeft(4*FH, stamp3);
  lcd_putsLeft(5*FH, stamp4);
  lcd_putsLeft(7*FH, STR_EEPROMV);
  lcd_outdezAtt(8*FW, 7*FH, g_eeGeneral.myVers, LEFT);
}

void displayKeyState(uint8_t x, uint8_t y, EnumKeys key)
{
  uint8_t t = keyState(key);
  lcd_putcAtt(x, y, t+'0', t ? INVERS : 0);
}

void menuProcDiagKeys(uint8_t event)
{
  SIMPLE_MENU(STR_MENUDIAG, menuTabDiag, e_Keys, 1);

  lcd_puts(14*FW, 3*FH, STR_VTRIM);

  for(uint8_t i=0; i<9; i++) {
    uint8_t y = i*FH; //+FH;
    if(i>(SW_ID0-SW_BASE_DIAG)) y-=FH; //overwrite ID0
    putsSwitches(8*FW, y, i+1, 0); //ohne off,on
    displayKeyState(11*FW+2, y, (EnumKeys)(SW_BASE_DIAG+i));

    if (i<8) {
      y = i/2*FH+FH*4;
      lcd_img(14*FW, y, sticks, i/2, 0);
      displayKeyState(i&1? 20*FW : 18*FW, y, (EnumKeys)(TRM_BASE+i));
    }

    if (i<6) {
      y = (5-i)*FH+2*FH;
      lcd_putsiAtt(0, y, STR_VKEYS, i, 0);
      displayKeyState(5*FW+2, y, (EnumKeys)(KEY_MENU+i));
    }
  }

#if defined (ROTARY_ENCODERS)
  for(uint8_t i=0; i<ROTARY_ENCODERS; i++) {
    uint8_t y = i*FH + FH;
    lcd_putsiAtt(14*FW, y, STR_VRENCODERS, i, 0);
    lcd_outdezNAtt(18*FW, y, g_rotenc[i], LEFT|(keyState((EnumKeys)(BTN_REa+i)) ? INVERS : 0));
  }
#endif

}

void menuProcDiagAna(uint8_t event)
{
#if defined(PCBSKY9X) && defined(REVB)
#define ANAS_ITEMS_COUNT 4
#elif defined(PCBSKY9X)
#define ANAS_ITEMS_COUNT 3
#else
#define ANAS_ITEMS_COUNT 2
#endif

  SIMPLE_MENU(STR_MENUANA, menuTabDiag, e_Ana, ANAS_ITEMS_COUNT);

  for (uint8_t i=0; i<7; i++) {
    uint8_t y = 1+FH+(i/2)*FH;
    uint8_t x = i&1 ? 64+5 : 0;
    putsStrIdx(x, y, PSTR("A"), i+1);
    lcd_putc(x+2*FWNUM, y, ':');
    lcd_outhex4(x+3*FW-1, y, anaIn(i));
    lcd_outdez8(x+10*FW-1, y, (int16_t)calibratedStick[CONVERT_MODE(i+1)-1]*25/256);
  }

#if !defined(PCBSKY9X)
  // Display raw BandGap result (debug)
  lcd_puts(64+5, 1+4*FH, STR_BG);
  lcd_outdezAtt(64+5+6*FW-3, 1+4*FH, BandGap, 0);
#endif

#if defined(PCBSKY9X)
  lcd_putsLeft(5*FH, STR_BATT_CALIB);
  static uint32_t adcBatt;
  adcBatt = ((adcBatt * 7) + anaIn(7)) / 8; // running average, sourced directly (to avoid unending debate :P)
  uint32_t batCalV = ( adcBatt + adcBatt*(g_eeGeneral.vBatCalib)/128 ) * 4191 ;
  batCalV /= 55296  ;
  putsVolts(LEN_CALIB_FIELDS*FW+4*FW, 5*FH, batCalV, (m_posVert==1 ? INVERS : 0));
#elif defined(PCBGRUVIN9X)
  lcd_putsLeft(6*FH-2, STR_BATT_CALIB);
  // Gruvin wants 2 decimal places and instant update of volts calib field when button pressed
  static uint16_t adcBatt;
  adcBatt = ((adcBatt * 7) + anaIn(7)) / 8; // running average, sourced directly (to avoid unending debate :P)
  uint32_t batCalV = ((uint32_t)adcBatt*1390 + (10*(int32_t)adcBatt*g_eeGeneral.vBatCalib)/8) / BandGap;
  lcd_outdezNAtt(LEN_CALIB_FIELDS*FW+4*FW, 6*FH-2, batCalV, PREC2|(m_posVert==1 ? INVERS : 0));
#else
  lcd_putsLeft(6*FH-2, STR_BATT_CALIB);
  putsVolts(LEN_CALIB_FIELDS*FW+4*FW, 6*FH-2, g_vbat100mV, (m_posVert==1 ? INVERS : 0));
#endif
  if (m_posVert==1) CHECK_INCDEC_GENVAR(event, g_eeGeneral.vBatCalib, -127, 127);

#if defined(PCBSKY9X) && defined(REVB)
  lcd_putsLeft(6*FH, STR_CURRENT_CALIB);
  putsTelemetryValue(LEN_CALIB_FIELDS*FW+4*FW, 6*FH, getCurrent(), UNIT_MILLIAMPS, (m_posVert==2 ? INVERS : 0)) ;
  if (m_posVert==2) CHECK_INCDEC_GENVAR(event, g_eeGeneral.currentCalib, -49, 49);
#endif

#if defined(PCBSKY9X)
  lcd_putsLeft(7*FH, PSTR("Temp. Calib"));
  putsTelemetryValue(LEN_CALIB_FIELDS*FW+4*FW, 7*FH, getTemperature(), UNIT_DEGREES, (m_posVert==3 ? INVERS : 0)) ;
  if (m_posVert==3) CHECK_INCDEC_GENVAR(event, g_eeGeneral.temperatureCalib, -100, 100);
#endif
}

#if defined(PCBSKY9X)
enum menuProcHwItems {
  ITEM_SETUP_HW_COPROC,
  ITEM_SETUP_HW_OPTREX_DISPLAY,
  ITEM_SETUP_HW_MAX
};

#define GENERAL_HW_PARAM_OFS (2+(15*FW))
void menuProcHardware(uint8_t event)
{
  MENU(PSTR("Hardware"), menuTabDiag, e_Hardware, ITEM_SETUP_HW_MAX+1, {0, -1, 0});

  uint8_t sub = m_posVert - 1;

  for (uint8_t i=0; i<7; i++) {
    uint8_t y = 1*FH + i*FH;
    uint8_t k = i+s_pgOfs;
    uint8_t blink = ((s_editMode>0) ? BLINK|INVERS : INVERS);
    uint8_t attr = (sub == k ? blink : 0);

    switch(k) {
      case ITEM_SETUP_HW_COPROC:
        lcd_putsLeft(y, STR_COPROC);
        lcd_outhex4(GENERAL_HW_PARAM_OFS, y, (Coproc_valid << 8) + Coproc_read);
        break;

      case ITEM_SETUP_HW_OPTREX_DISPLAY:
        // TODO remove STR_OPTREX_DISPLAY + translations here
        g_eeGeneral.optrexDisplay = selectMenuItem(GENERAL_HW_PARAM_OFS, y, PSTR("LCD"), PSTR("\006NormalOptrex"), g_eeGeneral.optrexDisplay, 0, 1, attr, event);
        break;
    }
  }
}
#endif

void menuProcDiagCalib(uint8_t event)
{
  SIMPLE_MENU(STR_MENUCALIBRATION, menuTabDiag, e_Calib, 1);

  static uint8_t idxState;

  for (uint8_t i=0; i<7; i++) { //get low and high vals for sticks and trims
    int16_t vt = anaIn(i);
    reusableBuffer.calib.loVals[i] = min(vt, reusableBuffer.calib.loVals[i]);
    reusableBuffer.calib.hiVals[i] = max(vt, reusableBuffer.calib.hiVals[i]);
    //if(i>=4) midVals[i] = (loVals[i] + hiVals[i])/2;
  }

  s_noScroll = idxState; // make sure we don't scroll while calibrating

  switch(event)
  {
    case EVT_ENTRY:
      idxState = 0;
      break;

    case EVT_KEY_BREAK(KEY_MENU):
      idxState++;
      if (idxState==3) {
        STORE_GENERALVARS;
        idxState = 0;
      }
      break;
  }


  switch (idxState) {
    case 0:
      // START CALIBRATION
      lcd_puts(3*FW, 3*FH, STR_MENUTOSTART);
      break;

    case 1:
      // SET MIDPOINT
      lcd_putsAtt(5*FW, 2*FH, STR_SETMIDPOINT, s_noScroll ? INVERS : 0);
      lcd_puts(3*FW, 3*FH, STR_MENUWHENDONE);

      for (uint8_t i=0; i<7; i++) {
        reusableBuffer.calib.loVals[i] = 15000;
        reusableBuffer.calib.hiVals[i] = -15000;
        reusableBuffer.calib.midVals[i] = anaIn(i);
      }
      break;

    case 2:
      // MOVE STICKS/POTS
      lcd_putsAtt(3*FW, 2*FH, STR_MOVESTICKSPOTS, s_noScroll ? INVERS : 0);
      lcd_puts(3*FW, 3*FH, STR_MENUWHENDONE);

      for (uint8_t i=0; i<7; i++) {
        if (abs(reusableBuffer.calib.loVals[i]-reusableBuffer.calib.hiVals[i])>50) {
          g_eeGeneral.calibMid[i] = reusableBuffer.calib.midVals[i];
          int16_t v = reusableBuffer.calib.midVals[i] - reusableBuffer.calib.loVals[i];
          g_eeGeneral.calibSpanNeg[i] = v - v/64;
          v = reusableBuffer.calib.hiVals[i] - reusableBuffer.calib.midVals[i];
          g_eeGeneral.calibSpanPos[i] = v - v/64;
        }
      }

      g_eeGeneral.chkSum = evalChkSum();
      break;
  }

  doMainScreenGrphics();
}
