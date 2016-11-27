#pragma once
#include "Foundation_Common.h"

namespace Vanguard
{
	namespace eVerticalAlignment
	{
		enum Type
		{
			Center = 0,
			Top = 1,
			Bottom = 2
		};
	}

	namespace eHorizontalAlignment
	{
		enum Type
		{
			Center = 0,
			Left = 1,
			Right = 2
		};
	}

	class Alignment
	{
		uint8 data;

	public:
		Alignment() : data(0) {}
		Alignment(eVerticalAlignment::Type aAlignmentVertical, eHorizontalAlignment::Type aAlignmentHorizontal)
		{
			SetVertical(aAlignmentVertical);
			SetHorizontal(aAlignmentHorizontal);
		}

		void SetVertical(eVerticalAlignment::Type aAlignment)
		{
			data = (data & 0xF0) | (uint8)aAlignment;
		}

		eVerticalAlignment::Type GetVertical() const
		{
			return eVerticalAlignment::Type(data & 0x0F);
		}

		void SetHorizontal(eHorizontalAlignment::Type aAlignment)
		{
			data = (data & 0x0F) | (uint8)aAlignment << 4;
		}

		eHorizontalAlignment::Type GetHorizontal() const
		{
			return eHorizontalAlignment::Type(data >> 4);
		}
	};
}