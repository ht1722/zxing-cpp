/*
 * Copyright 2022 Axel Waggershauser
 */
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "DecodeStatus.h"

#include <string>

namespace ZXing {

class Error
{
public:
	enum class Type { None, Format, Checksum, Unsupported };
	Type type() const noexcept { return _type; }
	const std::string& msg() const noexcept { return _msg; }
	operator bool() const noexcept { return _type != Type::None; }

	Error() = default;

	static constexpr auto Format = Type::Format;
	static constexpr auto Checksum = Type::Checksum;
	static constexpr auto Unsupported = Type::Unsupported;

protected:
	Type _type = Type::None;
	std::string _msg;

	Error(Type type, std::string msg) : _type(type), _msg(std::move(msg)) {}
};

inline bool operator==(const Error& e, Error::Type t) noexcept { return e.type() == t; }
inline bool operator!=(const Error& e, Error::Type t) noexcept { return !(e == t); }
inline bool operator==(Error::Type t, const Error& e) noexcept { return e.type() == t; }
inline bool operator!=(Error::Type t, const Error& e) noexcept { return !(t == e); }

class FormatError : public Error
{
public:
	FormatError(std::string msg = {}) : Error(Format, std::move(msg)) {}
};

class ChecksumError : public Error
{
public:
	ChecksumError(std::string msg = {}) : Error(Checksum, std::move(msg)) {}
};

inline std::string ToString(const Error& e)
{
	const char* name[] = {"", "FormatError", "ChecksumError", "Unsupported"};
	std::string ret = name[static_cast<int>(e.type())];
	if (!e.msg().empty())
		ret += " (" + e.msg() + ")";
	return ret;
}

// transitional helper function
inline Error Status2Error(DecodeStatus s)
{
	switch (s) {
	case DecodeStatus::FormatError: return FormatError();
	case DecodeStatus::ChecksumError: return ChecksumError();
	default: return {};
	}
}

}