#pragma once

class StyleManager
{
	StyleManager() {}

public:
	static StyleManager* GetInstance();

	void LoadStyle();
};