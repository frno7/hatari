/*
  Hatari - rtc.c

  This file is distributed under the GNU General Public License, version 2
  or at your option any later version. Read the file gpl.txt for details.

  Mega-ST / Mega-STE real time clock.

  There is probably a more efficient way to do it, such as using directly a
  timer in ram instead of calling localtime for each function. For now it will
  show that it works, at least...

  In fact these mappings seems to force the gem to ask the IKBD for the real
  time (seconds units). See ikbd.c for the time returned by the IKBD.

  NOTE : we only emulate the case where BANK=0 in MODE register, as TOS doesn't
  use the BANK=1 setting for alarm
*/


/*
  RP5C15 Real Time Clock

  References :
   - RP/RF/RJ5C15 datasheet by Ricoh (EK-086-9908, June 1995)

                                  -----------
                         CS(INV) -| 1    18 |- VCC
                              CS -| 2    17 |- OSCOUT
       CLKOUT : connected to TPI -| 3    16 |- OSCIN
                              A0 -| 4    15 |- ALARM(INV) : not connected
                              A1 -| 5    14 |- D3
                              A2 -| 6    13 |- D2
                              A3 -| 7    12 |- D1
                         RD(INV) -| 8    11 |- D0
                             GND -| 9    10 |- WR(INV)
                                  -----------

  Registers (when BANK=0) :
    0xfffc21.b	Seconds, units
    0xfffc23.b	Seconds, tens
    0xfffc25.b	Minutes, units
    0xfffc27.b	Minutes, tens
    0xfffc29.b	Hours, units
    0xfffc2b.b	Hours, tens
    0xfffc2d.b	Weekday
    0xfffc2f.b	Day, units
    0xfffc31.b	Day, tens
    0xfffc33.b	Month, units
    0xfffc35.b	Month, tens
    0xfffc37.b	Year, units
    0xfffc39.b	Year, tens
    0xfffc3b.b	Mode register
    0xfffc3d.b	Test register
    0xfffc3f.b	Reset register

*/


const char Rtc_fileid[] = "Hatari rtc.c : " __DATE__ " " __TIME__;

#include <time.h>

#include "main.h"
#include "ioMem.h"
#include "rtc.h"


static bool rtc_bank;           /* RTC bank select (0=normal, 1=configuration(?)) */
static Sint8 fake_am, fake_amz;


/*-----------------------------------------------------------------------*/
/**
 * Read seconds units.
 */
void Rtc_SecondsUnits_ReadByte(void)
{
	struct tm *SystemTime;
	time_t nTimeTicks;

	/* Get system time */
	nTimeTicks = time(NULL);
	SystemTime = localtime(&nTimeTicks);
	IoMem[0xfffc21] = SystemTime->tm_sec % 10;
}


/*-----------------------------------------------------------------------*/
/**
 * Read seconds tens.
 */
void Rtc_SecondsTens_ReadByte(void)
{
	struct tm *SystemTime;
	time_t nTimeTicks;

	/* Get system time */
	nTimeTicks = time(NULL);
	SystemTime = localtime(&nTimeTicks);
	IoMem[0xfffc23] = SystemTime->tm_sec / 10;
}


/*-----------------------------------------------------------------------*/
/**
 * Read minutes units.
 */
void Rtc_MinutesUnits_ReadByte(void)
{
	if (rtc_bank)
	{
		IoMem[0xfffc25] = fake_am;
	}
	else
	{
		struct tm *SystemTime;
		time_t nTimeTicks;

		/* Get system time */
		nTimeTicks = time(NULL);
		SystemTime = localtime(&nTimeTicks);
		IoMem[0xfffc25] = SystemTime->tm_min % 10;
	}
}


/*-----------------------------------------------------------------------*/
/**
 * Write minutes units.
 */
void Rtc_MinutesUnits_WriteByte(void)
{
	/* TOS 1.0x uses this... */
	if (rtc_bank)
		fake_am = ((IoMem[0xfffc25] & 0x0f) | 0xf0);
	/* else ignore */
}


/*-----------------------------------------------------------------------*/
/**
 * Read minutes tens.
 */
void Rtc_MinutesTens_ReadByte(void)
{
	if (rtc_bank)
	{
		IoMem[0xfffc27] = fake_amz;
	}
	else
	{
		struct tm *SystemTime;
		time_t nTimeTicks;

		/* Get system time */
		nTimeTicks = time(NULL);
		SystemTime = localtime(&nTimeTicks);
		IoMem[0xfffc27] = SystemTime->tm_min / 10;
	}
}


/*-----------------------------------------------------------------------*/
/**
 * Write minutes tens.
 */
void Rtc_MinutesTens_WriteByte(void)
{
	/* TOS 1.0x uses this... */
	if (rtc_bank)
		fake_amz = ((IoMem[0xfffc27] & 0x0f) | 0xf0);
	/* else ignore */
}


/*-----------------------------------------------------------------------*/
/**
 * Read hours units.
 */
void Rtc_HoursUnits_ReadByte(void)
{
	struct tm *SystemTime;
	time_t nTimeTicks;

	/* Get system time */
	nTimeTicks = time(NULL);
	SystemTime = localtime(&nTimeTicks);
	IoMem[0xfffc29] = SystemTime->tm_hour % 10;
}


/*-----------------------------------------------------------------------*/
/**
 * Read hours tens.
 */
void Rtc_HoursTens_ReadByte(void)
{
	struct tm *SystemTime;
	time_t nTimeTicks;

	/* Get system time */
	nTimeTicks = time(NULL);
	SystemTime = localtime(&nTimeTicks);
	IoMem[0xfffc2b] = SystemTime->tm_hour / 10;
}


/*-----------------------------------------------------------------------*/
/**
 * Read weekday.
 */
void Rtc_Weekday_ReadByte(void)
{
	struct tm *SystemTime;
	time_t nTimeTicks;

	/* Get system time */
	nTimeTicks = time(NULL);
	SystemTime = localtime(&nTimeTicks);
	IoMem[0xfffc2d] = SystemTime->tm_wday;
}


/*-----------------------------------------------------------------------*/
/**
 * Read day units.
 */
void Rtc_DayUnits_ReadByte(void)
{
	struct tm *SystemTime;
	time_t nTimeTicks;

	/* Get system time */
	nTimeTicks = time(NULL);
	SystemTime = localtime(&nTimeTicks);
	IoMem[0xfffc2f] = SystemTime->tm_mday % 10;
}


/*-----------------------------------------------------------------------*/
/**
 * Read day tens.
 */
void Rtc_DayTens_ReadByte(void)
{
	struct tm *SystemTime;
	time_t nTimeTicks;

	/* Get system time */
	nTimeTicks = time(NULL);
	SystemTime = localtime(&nTimeTicks);
	IoMem[0xfffc31] = SystemTime->tm_mday / 10;
}


/*-----------------------------------------------------------------------*/
/**
 * Read month units.
 */
void Rtc_MonthUnits_ReadByte(void)
{
	struct tm *SystemTime;
	time_t nTimeTicks;

	/* Get system time */
	nTimeTicks = time(NULL);
	SystemTime = localtime(&nTimeTicks);
	IoMem[0xfffc33] = (SystemTime->tm_mon + 1) % 10;
}


/*-----------------------------------------------------------------------*/
/**
 * Read month tens.
 */
void Rtc_MonthTens_ReadByte(void)
{
	struct tm *SystemTime;
	time_t nTimeTicks;

	/* Get system time */
	nTimeTicks = time(NULL);
	SystemTime = localtime(&nTimeTicks);
	IoMem[0xfffc35] = (SystemTime->tm_mon + 1) / 10;
}


/*-----------------------------------------------------------------------*/
/**
 * Read year units.
 */
void Rtc_YearUnits_ReadByte(void)
{
	struct tm *SystemTime;
	time_t nTimeTicks;

	/* Get system time */
	nTimeTicks = time(NULL);
	SystemTime = localtime(&nTimeTicks);
	IoMem[0xfffc37] = SystemTime->tm_year % 10;
}


/*-----------------------------------------------------------------------*/
/**
 * Read year tens.
 */
void Rtc_YearTens_ReadByte(void)
{
	struct tm *SystemTime;
	time_t nTimeTicks;

	/* Get system time */
	nTimeTicks = time(NULL);
	SystemTime = localtime(&nTimeTicks);
	IoMem[0xfffc39] = (SystemTime->tm_year - 80) / 10;
}


/*-----------------------------------------------------------------------*/
/**
 * Read clock mod.
 */
void Rtc_ClockMod_ReadByte(void)
{
	IoMem[0xfffc3b] = ((IoMem[0xfffc3b] & 0x0f) | 0xf0);
}


/*-----------------------------------------------------------------------*/
/**
 * Write clock mod.
 */
void Rtc_ClockMod_WriteByte(void)
{
	rtc_bank = IoMem[0xfffc3b] & 1;
}

