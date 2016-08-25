#pragma once
#include "VanguardString.h"
#include "Foundation_Common.h"

namespace juce
{
	class RelativeTime;
	class Time;
}

namespace Vanguard
{
	// Represents a span of time.
	// Wrapper for juce::RelativeTime
	struct FOUNDATION_API Timespan
	{
		friend struct Time;
	private:
		juce::RelativeTime* data;

	public:
		explicit Timespan(double aSeconds = 0.0);
		Timespan(const Timespan& aOther);
		Timespan& operator = (const Timespan& aOther);
		~Timespan();

		static Timespan GetElapsedSystemTime();

		inline static Timespan FromMilliseconds(int aMilliseconds) { return Timespan(aMilliseconds * 0.001); }
		inline static Timespan FromMilliseconds(int64 aMilliseconds) { return Timespan(aMilliseconds * 0.001); }
		inline static Timespan FromSeconds(double aSeconds) { return Timespan(aSeconds); }
		inline static Timespan FromMinutes(double aNumberOfMinutes) { return Timespan(aNumberOfMinutes * 60.0); }
		inline static Timespan FromHours(double aNumberOfHours) { return Timespan(aNumberOfHours * 3600.0); }
		inline static Timespan FromDays(double aNumberOfDays) { return Timespan(aNumberOfDays  * 86400.0); }
		inline static Timespan FromWeeks(double aNumberOfWeeks) { return Timespan(aNumberOfWeeks * 604800.0); }

		inline int64 InMilliseconds() const { return (int64)(InSeconds() * 1000); }

		double InSeconds() const;
		double InMinutes() const;
		double InHours() const;
		double InDays() const;
		double InWeeks() const;

		// Returns a readable textual description of the time.
		// Eg. "1 min 4 secs", "1 hr 45 mins", "2 weeks 5 days", "140 ms"
		String GetDescription(const String& aReturnValueForZeroTime = "0");

		// ----Operators----

		bool operator== (const Timespan& aOther) const { return InSeconds() == aOther.InSeconds(); }
		bool operator!= (const Timespan& aOther) const { return InSeconds() != aOther.InSeconds(); }
		bool operator>  (const Timespan& aOther) const { return InSeconds() >  aOther.InSeconds(); }
		bool operator<  (const Timespan& aOther) const { return InSeconds() <  aOther.InSeconds(); }
		bool operator>= (const Timespan& aOther) const { return InSeconds() >= aOther.InSeconds(); }
		bool operator<= (const Timespan& aOther) const { return InSeconds() <= aOther.InSeconds(); }

		Timespan operator+ (const Timespan& aOther) const { return Timespan(InSeconds() + aOther.InSeconds()); }
		Timespan operator- (const Timespan& aOther) const { return Timespan(InSeconds() - aOther.InSeconds()); }
	};

	// Represents a specific moment in time. Only accurate down to the millisecond. For more accurate timing, see Timespan.
	// Wrapper for juce::Time
	struct FOUNDATION_API Time
	{
		friend struct Timespan;
	private:
		juce::Time* data;

	public:

		/** Creates a time from a set of date components.

		The timezone is assumed to be whatever the system is using as its locale.

		@param year             the year, in 4-digit format, e.g. 2004
		@param month            the month, in the range 0 to 11
		@param day              the day of the month, in the range 1 to 31
		@param hours            hours in 24-hour clock format, 0 to 23
		@param minutes          minutes 0 to 59
		@param seconds          seconds 0 to 59
		@param milliseconds     milliseconds 0 to 999
		@param useLocalTime     if true, encode using the current machine's local time; if
		false, it will always work in GMT.
		*/
		Time(int32 aYear, int32 aMonth, int32 aDay, int32 aHours, int32 aMinutes, int32 aSeconds = 0, int32 aMilliseconds = 0, bool aUseLocalTime = true);

		Time(int64 aMiliseconds);
		Time(const Time& aOther);
		Time& operator = (const Time& aOther);

		~Time();

		// Returns a Time object that is set to the current system time.
		static Time CurrentTime();

		// Returns the time as a number of milliseconds since Jan 1st 1970.
		int64 ToMilliseconds() const;

		//Returns the year.
		int32 GetYear() const;

		// Returns the number of the month.
		// Note: The value returned is in the range 0 to 11.
		int32 GetMonth() const;

		//// Returns the name of the month.
		String GetMonthName() const;

		// Returns a 3 letter abbreviation of the month name.
		String GetMonthAbbreviation() const;

		// Returns the day of the month. Range is 1 to 31.
		int32 GetDayOfMonth() const;

		// Returns the number of the day of the week. Range is 0 to 6, day 0 is Sunday.
		int32 GetDayOfWeek() const;

		// Returns the number of the day of the year. Range is 0 to 365
		int32 GetDayOfYear() const;

		//Returns the name of the weekday.
		String GetWeekdayName() const;

		//Returns the 3 letter abbreviation of the weekday name.
		String GetWeekdayAbbreviation() const;

		// Get the number of hours since midnight. Range is 0-23
		int32 GetHour() const;

		// Get the number of minutes. Range is 0 to 59.
		int32 GetMinutes() const;

		// Returns the number of seconds. Range is 0 to 59.
		int32 GetSeconds() const;

		//Returns the number of milliseconds. Range is 0 to 999.
		// Note: Unlike ToMilliseconds(), this just returns the position within the current second rather than the total number since the epoch.
		int32 GetMilliseconds() const;

		// Returns true if the local timezone uses daylight savings.
		bool IsDaylightSavingTime() const;

		// Returns a 3-character string to indicate the local timezone.
		String GetTimeZone() const;

		// Quick way of getting a string version of a date and time.
		String ToString(bool aIncludeDate, bool aIncludeTime, bool aIncludeSeconds = true, bool aUse24HourClock = false) const;

		// Returns the name of a day of the week. Range is 0 to 11, day 0 is Sunday.
		static String GetWeekdayName(int aDayNumber);

		// Returns the 3-letter abbreviation of a day of the week. Range is 0 to 11, day 0 is Sunday.
		static String GetWeekdayAbbreviation(int aDayNumber);

		// Returns the name of one of the months. Range is 0 to 11
		static String GetMonthName(int aMonthNumber);

		// Returns the 3-letter abbreviation of one of the months. Range is 0 to 11
		static String GetMonthAbbreviation(int aMonthNumber);

		//Returns the current system time. (Number of milliseconds since midnight jan 1st, 1970.
		static int64 CurrentTimeMillis();

		// Returns the number of millisecs since a fixed event (usually system startup).
		// Note: This counter is unaffected by changes to the system clock.
		static uint32 GetMillisecondCounter();

		// Returns the current high-resolution counter's tick-count.
		static int64 GetHighResolutionTicks();

		// Returns the resolution of the high-resolution counter in ticks per second.
		static int64 GetHighResolutionTicksPerSecond();

		// Converts a number of high-resolution ticks into seconds.
		static double HighResolutionTicksToSeconds(int64 aTicks);

		// Converts a number seconds into high-resolution ticks.
		static int64 SecondsToHighResolutionTicks(double aSeconds);

		// Returns a Time based on the value of the __DATE__ macro when this module was compiled
		static Time GetCompilationDate();

		// ----Operators----

		inline Time operator+ (const Timespan& aDeltaTime) { return Time(ToMilliseconds() + aDeltaTime.InMilliseconds()); }
		inline Time operator- (const Timespan& aDeltaTime) { return Time(ToMilliseconds() - aDeltaTime.InMilliseconds()); }

		inline Timespan operator- (const Time& aOther) { return Timespan::FromMilliseconds(ToMilliseconds() - aOther.ToMilliseconds()); }

		//inline Time& operator+= (const Timespan& aDeltaTime)
		//{
		//	this->data += aDeltaTime.data;
		//	return *this;
		//}

		//inline Time& operator-= (const Timespan& aDeltaTime)
		//{
		//	this->data -= aDeltaTime.data;
		//	return *this;
		//}

		inline bool operator== (const Time& aOther) { return data == aOther.data;}
		inline bool operator!= (const Time& aOther) { return data != aOther.data;}
		inline bool operator<  (const Time& aOther) { return data < aOther.data;}
		inline bool operator<= (const Time& aOther) { return data <= aOther.data;}
		inline bool operator>  (const Time& aOther) { return data > aOther.data;}
		inline bool operator>= (const Time& aOther) { return data >= aOther.data;}
	};
}
