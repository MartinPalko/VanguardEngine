#pragma once

#include "ConfigTable.h"
#include "ConfigVar.h"

#include "Int32ConfigVar.h"
#include "FloatConfigVar.h"
#include "BooleanConfigVar.h"
#include "StringConfigVar.h"

// Macros for easy declaration of config variables.
#define int32_Config(varName, configFile, defaultValue) const Vanguard::Int32ConfigVar varName = Vanguard::Int32ConfigVar(configFile,typeid(*this).name(),#varName,defaultValue);
#define float_Config(varName, configFile, defaultValue) const Vanguard::FloatConfigVar varName = Vanguard::FloatConfigVar(configFile,typeid(*this).name(),#varName,defaultValue);
#define bool_Config(varName, configFile, defaultValue) const Vanguard::BooleanConfigVar varName = Vanguard::BooleanConfigVar(configFile,typeid(*this).name(),#varName,defaultValue);
#define String_Config(varName, configFile, defaultValue) const Vanguard::StringConfigVar varName = Vanguard::StringConfigVar(configFile,typeid(*this).name(),#varName,defaultValue);