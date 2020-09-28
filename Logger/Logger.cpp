#include "Logger.h"

Logger::Logger()
{
	m_bCommandLineLogging = false;
	m_bFileLogging = false;
	m_bTimeStamping = false;
	file = std::ofstream();
}

Logger::Logger(const std::string &sFilename, bool bFileLogging, bool bCommandLineLogging, bool bTimeStamping)
{
	m_bFileLogging = bFileLogging;
	m_bCommandLineLogging = bCommandLineLogging;
	m_bTimeStamping = bTimeStamping;
	m_tStart = clock_t::now();

	if (m_bFileLogging)
		file = std::ofstream(sFilename);
}

Logger::~Logger()
{
	file.close();
}

void Logger::Log(const std::string &string, const std::string &branch)
{
	auto paddString = [](const std::string & string, int len)
	{
		std::string str = "";
		for (int i = 0; i < len; i++)
		{
			if (i < len - string.length())
			{
				str += " ";
			}
			else
			{
				str += string;
				return str;
			}
		}

		return string;
	};

	std::string log = "";

	if (m_bTimeStamping)
	{
		float fTimeSinceStart = (float)std::chrono::duration_cast<std::chrono::milliseconds>(clock_t::now() - m_tStart).count() / 1e3f;
		log += "[" + paddString(std::to_string((size_t)fTimeSinceStart), 7) + "]";
	}
	if (branch != "")
		log += "[" + branch + "] ";
	log += string;

	if (m_bCommandLineLogging)
		std::cout << log << std::endl;
	if (file.is_open() && m_bFileLogging)
		file << log << std::endl;
}
