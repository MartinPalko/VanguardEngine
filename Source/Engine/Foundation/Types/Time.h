#pragma once
#include "juce_core.h"
#include "IntegerDefs.h"
#include "VanguardString.h"

namespace Vanguard
{
	// Represents a span of time.
	// Wrapper for juce::RelativeTime
	struct Timespan
	{
		friend struct Time;
	private:
		juce::RelativeTime data;

	public:
		explicit Timespan(double aSeconds = 0.0) : data (aSeconds) {}		
		Timespan(const juce::RelativeTime& aOther) : data(aOther) {}
		Timespan(const Timespan& aOther) : data(aOther.data) {}

		Timespan& operator= (const Timespan& aOther) 
		{
			data = aOther.data;
			return *this;
		}

		//~Timespan() {}

		inline static Timespan FromMilliseconds(int aMilliseconds) { return Timespan(aMilliseconds * 0.001); }
		inline static Timespan FromMilliseconds(int64 aMilliseconds) { return Timespan(aMilliseconds * 0.001); }
		inline static Timespan FromSeconds(double aSeconds) { return Timespan(aSeconds); }
		inline static Timespan FromMinutes(double aNumberOfMinutes) { return Timespan(aNumberOfMinutes * 60.0); }
		inline static Timespan FromHours(double aNumberOfHours) { return Timespan(aNumberOfHours * 3600.0); }
		inline static Timespan FromDays(double aNumberOfDays) { return Timespan(aNumberOfDays  * 86400.0); }
		inline static Timespan FromWeeks(double aNumberOfWeeks) { return Timespan(aNumberOfWeeks * 604800.0); }

		inline int64 InMilliseconds() const { return (int64)(InSeconds() * 1000); }

		inline double InSeconds() const { return data.inSeconds(); }

		inline double InMinutes() const { return data.inMinutes(); }
		inline double InHours() const { return data.inHours(); }
		inline double InDays() const { return data.inDays(); }
		inline double InWeeks() const { return data.inWeeks(); }

		// Returns a readable textual description of the time.
		// Eg. "1 min 4 secs", "1 hr 45 mins", "2 weeks 5 days", "140 ms"
		String GetDescription(const String& aReturnValueForZeroTime = "0") { return data.getDescription(aReturnValueForZeroTime); }

		// ----Operators----

		bool operator== (Timespan aOther) { return data == aOther.data; }
		bool operator!= (Timespan aOther) { return data != aOther.data; }
		bool operator>  (Timespan aOther) { return data >  aOther.data; }
		bool operator<  (Timespan aOther) { return data <  aOther.data; }
		bool operator>= (Timespan aOther) { return data >= aOther.data; }
		bool operator<= (Timespan aOther) { return data <= aOther.data; }

		Timespan operator+ (Timespan aOther) { return data + aOther.data; }
		Timespan operator- (Timespan aOther) { return data + aOther.data; }
	};

	// Represents a specific moment in time.
	// Wrapper for juce::Time
	struct Time
	{
		friend struct Timespan;
	private:
		juce::Time data;

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
		inline Time(int32 aYear, int32 aMonth, int32 aDay, int32 aHours, int32 aMinutes, int32 aSeconds = 0, int32 aMilliseconds = 0, bool aUseLocalTime = true)
			: data(aYear, aMonth, aDay, aHours, aMinutes, aSeconds, aMilliseconds, aUseLocalTime){}

		inline Time(int64 aMiliseconds) : data(aMiliseconds) {}

		inline Time(const Time& aOther) : data(aOther.ToMilliseconds()) {}

		inline Time(const juce::Time& aOther) : data(aOther.toMilliseconds()){}

		~Time() {}

		inline Time& operator= (const Time& aOther)
		{
			data = aOther.data;
			return *this;
		}

		// Returns a Time object that is set to the current system time.
		inline static Time GetCurrentTime() { return juce::Time::getCurrentTime(); }

		// Returns the time as a number of milliseconds since Jan 1st 1970.
		inline int64 ToMilliseconds() const { return data.toMilliseconds(); }
							
		//Returns the year.
		inline int32 GetYear() const { return data.getYear(); }

		// Returns the number of the month.
		// Note: The value returned is in the range 0 to 11.
		inline int32 GetMonth() const { return data.getMonth(); }

		//// Returns the name of the month.
		inline String GetMonthName() const { return data.getMonthName(false); }

		// Returns a 3 letter abbreviation of the month name.
		inline String GetMonthAbbreviation() const { return data.getMonthName(true); }

		// Returns the day of the month. Range is 1 to 31.
		inline int32 GetDayOfMonth() const { return data.getDayOfMonth(); }

		// Returns the number of the day of the week. Range is 0 to 6, day 0 is Sunday.
		inline int32 GetDayOfWeek() const { return data.getDayOfWeek(); }

		// Returns the number of the day of the year. Range is 0 to 365
		inline int32 GetDayOfYear() const { return data.getDayOfYear(); }

		//Returns the name of the weekday.
		inline String GetWeekdayName() { return data.getWeekdayName(false); }

		//Returns the 3 letter abbreviation of the weekday name.
		inline String GetWeekdayAbbreviation() { return data.getWeekdayName(true); }

		// Get the number of hours since midnight. Range is 0-23
		inline int32 GetHour() { return data.getHours(); }

		// Get the number of minutes. Range is 0 to 59.
		inline int32 GetMinutes() const { return data.getMinutes(); }

		// Returns the number of seconds. Range is 0 to 59.
		inline int32 GetSeconds() const { return data.getSeconds(); }

		//Returns the number of milliseconds. Range is 0 to 999.
		// Note: Unlike ToMilliseconds(), this just returns the position within the current second rather than the total number since the epoch.
		inline int32 GetMilliseconds() const { return data.getMilliseconds(); }

		// Returns true if the local timezone uses daylight savings.
		inline bool IsDaylightSavingTime() const { return data.isDaylightSavingTime(); }

		// Returns a 3-character string to indicate the local timezone.
		inline String GetTimeZone() const { return data.getTimeZone(); }

		// Quick way of getting a string version of a date and time.
		inline String ToString(bool aIncludeDate, bool aIncludeTime, bool aIncludeSeconds = true, bool aUse24HourClock = false) const
		{
			return data.toString(aIncludeDate, aIncludeTime, aIncludeSeconds, aUse24HourClock);
		}

		// Returns the name of a day of the week. Range is 0 to 11, day 0 is Sunday.
		inline static String GetWeekdayName(int aDayNumber) { return juce::Time::getWeekdayName(aDayNumber, false); }

		// Returns the 3-letter abbreviation of a day of the week. Range is 0 to 11, day 0 is Sunday.
		inline static String GetWeekdayAbbreviation(int aDayNumber) { return juce::Time::getWeekdayName(aDayNumber, true); }

		// Returns the name of one of the months. Range is 0 to 11
		inline static String GetMonthName(int aMonthNumber) { return juce::Time::getMonthName(aMonthNumber, false); }

		// Returns the 3-letter abbreviation of one of the months. Range is 0 to 11
		inline static String GetMonthAbbreviation(int aMonthNumber) { return juce::Time::getMonthName(aMonthNumber, true); }

		//Returns the current system time. (Number of milliseconds since midnight jan 1st, 1970.
		inline static int64 CurrentTimeMillis() { return juce::Time::currentTimeMillis(); }

		// Returns the number of millisecs since a fixed event (usually system startup).
		// Note: This counter is unaffected by changes to the system clock.
		inline static uint32 GetMillisecondCounter() { return juce::Time::getMillisecondCounter(); }

		// Returns the current high-resolution counter's tick-count.
		inline static int64 GetHighResolutionTicks() { return juce::Time::getHighResolutionTicks(); }

		// Returns the resolution of the high-resolution counter in ticks per second.
		inline static int64 GetHighResolutionTicksPerSecond() { return juce::Time::getHighResolutionTicksPerSecond(); }

		// Converts a number of high-resolution ticks into seconds.
		inline static double HighResolutionTicksToSeconds(int64 aTicks) { return juce::Time::highResolutionTicksToSeconds(aTicks); }

		// Converts a number seconds into high-resolution ticks.
		inline static int64 SecondsToHighResolutionTicks(double aSeconds) { return juce::Time::secondsToHighResolutionTicks(aSeconds); }

		// Returns a Time based on the value of the __DATE__ macro when this module was compiled
		inline static Time GetCompilationDate() { return juce::Time::getCompilationDate(); }

		// ----Operators----

		inline Time operator+ (const Timespan& aDeltaTime) { return Time(ToMilliseconds() + aDeltaTime.InMilliseconds()); }
		inline Time operator- (const Timespan& aDeltaTime) { return Time(ToMilliseconds() - aDeltaTime.InMilliseconds()); }

		inline Timespan operator- (const Time& aOther) { return Timespan::FromMilliseconds(ToMilliseconds() - aOther.ToMilliseconds()); }

		inline Time& operator+= (const Timespan& aDeltaTime) 
		{ 
			this->data += aDeltaTime.data; 
			return *this;
		}

		inline Time& operator-= (const Timespan& aDeltaTime)
		{
			this->data -= aDeltaTime.data;
			return *this;
		}

		inline bool operator== (const Time& aOther) { return data == aOther.data;}
		inline bool operator!= (const Time& aOther) { return data != aOther.data;}
		inline bool operator<  (const Time& aOther) { return data < aOther.data;}
		inline bool operator<= (const Time& aOther) { return data <= aOther.data;}
		inline bool operator>  (const Time& aOther) { return data > aOther.data;}
		inline bool operator>= (const Time& aOther) { return data >= aOther.data;}
	};
}