#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <type_traits>
#include <ctime>
#include <vector>


class LogDest
{
public:
	virtual bool log(const std::string &) noexcept = 0;
};


class StdOutDest : public LogDest
{
public:
	StdOutDest() noexcept = default;

public:
	bool log(const std::string &msg) noexcept override
	{
		std::cout << msg << std::endl;
		return true;
	}
};


class FileDest : public LogDest
{
private:
	std::ofstream m_file;

public:
	FileDest() noexcept = default;

	FileDest(const std::string &sFilename) noexcept
	{
		m_file = std::ofstream(sFilename);
	}

public:
	bool log(const std::string &msg) noexcept override
	{
		if (m_file.is_open())
		{
			m_file << msg;
			return true;
		}

		return false;
	}
};


class LogFormat
{
public:
	virtual std::string format(const std::string &) noexcept = 0;
};


class PlainFormat : public LogFormat
{
public:
	PlainFormat() noexcept = default;

public:
	std::string format(const std::string &msg) noexcept override
	{
		return msg;
	}
};


enum class Timestamp
{
	DATE = 0,
	TIME = 1,
	BOTH = 2
};


template <Timestamp timestamp>
class TimestampFormat : public LogFormat
{
private:
	constexpr static int s_nBuflen = 80;

public:
	TimestampFormat() noexcept = default;

private:
	static std::string strformat(const char *format) noexcept
	{
		time_t now = time(0);
		char buf[s_nBuflen];
		strftime(buf, s_nBuflen, format, localtime(&now));

		return std::string(buf);
	}

	static std::string getDateTimeString() noexcept
	{
		if constexpr (timestamp == Timestamp::DATE)
		{
			return strformat("[%F] ");
		}
		else if (timestamp == Timestamp::TIME)
		{
			return strformat("[%T] ");
		}
		else // Timestamp::BOTH
		{
			return strformat("[%F %T] ");
		}
	}

public:
	std::string format(const std::string &msg) noexcept override
	{
		return getDateTimeString() + msg;
	}
};


class BaseLogger
{
public:
	virtual bool log(const std::string &msg) noexcept = 0;
};


class Logger : public BaseLogger
{
private:
	std::shared_ptr<LogDest>   m_logDest{};
	std::shared_ptr<LogFormat> m_logFmt{};

public:
	Logger() noexcept = default;

	Logger(LogDest *logDest, LogFormat *format) noexcept
	{
		m_logDest = std::shared_ptr<LogDest>(logDest);
		m_logFmt = std::shared_ptr<LogFormat>(format);
	}

public:
	bool log(const std::string &msg) noexcept override
	{
		return m_logDest->log(m_logFmt->format(msg));
	}
};


class MDLogger : public BaseLogger
{
private:
	std::vector<std::shared_ptr<LogDest>> m_vLogDest{};
	std::shared_ptr<LogFormat> m_logFormat{};

public:
	MDLogger() noexcept = default;

	MDLogger(std::vector<LogDest *> vLogDest, LogFormat *format) noexcept
	{
		for (LogDest *ptr : vLogDest)
			m_vLogDest.emplace_back(ptr);

		m_logFormat = std::shared_ptr<LogFormat>(format);
	}

public:
	bool log(const std::string &msg) noexcept override
	{
		std::string text = m_logFormat->format(msg);

		bool bRet = true;
		for (const std::shared_ptr<LogDest> &pLogDest : m_vLogDest)
			bRet = bRet && pLogDest->log(text);

		return bRet;
	}
};
