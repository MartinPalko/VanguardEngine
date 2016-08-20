#include "juce_core.h"
#include "VanguardTime.h"

namespace Vanguard
{
	Timespan::Timespan(double aSeconds) 
	{
		data = new juce::RelativeTime(aSeconds);
	}

	Timespan::Timespan(const Timespan& aOther)
	{
		data = new juce::RelativeTime(aOther.InSeconds());
	}

	Timespan& Timespan::operator = (const Timespan& aOther)
	{
		*data = juce::RelativeTime(aOther.InSeconds());
		return *this;
	}

	Timespan::~Timespan()
	{
		delete data;
	}

	Timespan Timespan::GetElapsedSystemTime()
	{
		double d = juce::Time::getMillisecondCounterHiRes();
		double n = d / 1000.0;
		return Timespan(n);
	}

	double Timespan::InSeconds() const { return data->inSeconds(); }

	double Timespan::InMinutes() const { return data->inMinutes(); }
	double Timespan::InHours() const { return data->inHours(); }
	double Timespan::InDays() const { return data->inDays(); }
	double Timespan::InWeeks() const { return data->inWeeks(); }

	String Timespan::GetDescription(const String& aReturnValueForZeroTime)
	{
		return String(data->getDescription(aReturnValueForZeroTime.GetCharPointer()).getCharPointer());
	}


	// ----Time----

	Time::Time(int32 aYear, int32 aMonth, int32 aDay, int32 aHours, int32 aMinutes, int32 aSeconds, int32 aMilliseconds, bool aUseLocalTime)
	{
		data = new juce::Time(aYear, aMonth, aDay, aHours, aMinutes, aSeconds, aMilliseconds, aUseLocalTime);
	}

	Time::Time(int64 aMiliseconds)
	{
		data = new juce::Time(aMiliseconds);
	}

	Time::Time(const Time& aOther)
	{
		data = new juce::Time(aOther.ToMilliseconds());
	}

	Time& Time::operator = (const Time& aOther)
	{
		*data = juce::Time(aOther.ToMilliseconds());
		return *this;
	}

	Time::~Time()
	{
		delete data;
	}

	// Returns a Time object that is set to the current system time.
	Time Time::CurrentTime() { return juce::Time::getCurrentTime().toMilliseconds(); }

	// Returns the time as a number of milliseconds since Jan 1st 1970.
	int64 Time::ToMilliseconds() const { return data->toMilliseconds(); }

	//Returns the year.
	int32 Time::GetYear() const { return data->getYear(); }

	// Returns the number of the month.
	// Note: The value returned is in the range 0 to 11.
	int32 Time::GetMonth() const { return data->getMonth(); }

	//// Returns the name of the month.
	String Time::GetMonthName() const { return String(data->getMonthName(false).getCharPointer()); }

	// Returns a 3 letter abbreviation of the month name.
	String Time::GetMonthAbbreviation() const { return String(data->getMonthName(true).getCharPointer()); }

	// Returns the day of the month. Range is 1 to 31.
	int32 Time::GetDayOfMonth() const { return data->getDayOfMonth(); }

	// Returns the number of the day of the week. Range is 0 to 6, day 0 is Sunday.
	int32 Time::GetDayOfWeek() const { return data->getDayOfWeek(); }

	// Returns the number of the day of the year. Range is 0 to 365
	int32 Time::GetDayOfYear() const { return data->getDayOfYear(); }

	//Returns the name of the weekday.
	String Time::GetWeekdayName() const { return String(data->getWeekdayName(false).getCharPointer()); }

	//Returns the 3 letter abbreviation of the weekday name.
	String Time::GetWeekdayAbbreviation() const { return String(data->getWeekdayName(true).getCharPointer()); }

	// Get the number of hours since midnight. Range is 0-23
	int32 Time::GetHour() const { return data->getHours(); }

	// Get the number of minutes. Range is 0 to 59.
	int32 Time::GetMinutes() const { return data->getMinutes(); }

	// Returns the number of seconds. Range is 0 to 59.
	int32 Time::GetSeconds() const { return data->getSeconds(); }

	//Returns the number of milliseconds. Range is 0 to 999.
	// Note: Unlike ToMilliseconds(), this just returns the position within the current second rather than the total number since the epoch.
	int32 Time::GetMilliseconds() const { return data->getMilliseconds(); }

	// Returns true if the local timezone uses daylight savings.
	bool Time::IsDaylightSavingTime() const { return data->isDaylightSavingTime(); }

	// Returns a 3-character string to indicate the local timezone.
	String Time::GetTimeZone() const { return String(data->getTimeZone().getCharPointer()); }

	// Quick way of getting a string version of a date and time.
	String Time::ToString(bool aIncludeDate, bool aIncludeTime, bool aIncludeSeconds, bool aUse24HourClock) const
	{
		return String(data->toString(aIncludeDate, aIncludeTime, aIncludeSeconds, aUse24HourClock).getCharPointer());
	}

	// Returns the name of a day of the week. Range is 0 to 11, day 0 is Sunday.
	String Time::GetWeekdayName(int aDayNumber) { return String(juce::Time::getWeekdayName(aDayNumber, false).getCharPointer()); }

	// Returns the 3-letter abbreviation of a day of the week. Range is 0 to 11, day 0 is Sunday.
	String Time::GetWeekdayAbbreviation(int aDayNumber) { return String(juce::Time::getWeekdayName(aDayNumber, true).getCharPointer()); }

	// Returns the name of one of the months. Range is 0 to 11
	String Time::GetMonthName(int aMonthNumber) { return String(juce::Time::getMonthName(aMonthNumber, false).getCharPointer()); }

	// Returns the 3-letter abbreviation of one of the months. Range is 0 to 11
	String Time::GetMonthAbbreviation(int aMonthNumber) { return String(juce::Time::getMonthName(aMonthNumber, true).getCharPointer()); }

	//Returns the current system time. (Number of milliseconds since midnight jan 1st, 1970.
	int64 Time::CurrentTimeMillis() { return juce::Time::currentTimeMillis(); }

	// Returns the number of millisecs since a fixed event (usually system startup).
	// Note: This counter is unaffected by changes to the system clock.
	uint32 Time::GetMillisecondCounter() { return juce::Time::getMillisecondCounter(); }

	// Returns the current high-resolution counter's tick-count.
	int64 Time::GetHighResolutionTicks() { return juce::Time::getHighResolutionTicks(); }

	// Returns the resolution of the high-resolution counter in ticks per second.
	int64 Time::GetHighResolutionTicksPerSecond() { return juce::Time::getHighResolutionTicksPerSecond(); }

	// Converts a number of high-resolution ticks into seconds.
	double Time::HighResolutionTicksToSeconds(int64 aTicks) { return juce::Time::highResolutionTicksToSeconds(aTicks); }

	// Converts a number seconds into high-resolution ticks.
	int64 Time::SecondsToHighResolutionTicks(double aSeconds) { return juce::Time::secondsToHighResolutionTicks(aSeconds); }

	// Returns a Time based on the value of the __DATE__ macro when this module was compiled
	Time Time::GetCompilationDate() { return juce::Time::getCompilationDate().toMilliseconds(); }
}