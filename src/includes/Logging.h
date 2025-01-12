#pragma once

#include <iostream>

enum class LoggingLevel {
  LogLevelInfo = 0, LogLevelWarning, LogLevelError
};

class Logging {
public:
  Logging();
	Logging(const std::string &className);
  Logging(LoggingLevel loggingLevel, const std::string &className);
  ~Logging();
  LoggingLevel getLoggingLevel() const;
  void setLoggingLevel(LoggingLevel loggingLevel);
  void log(const std::string &message);
  void log(const std::string &message, LoggingLevel loggingLevel);
  void info(const std::string &message);
  void warn(const std::string &message);
  void error(const std::string &message);
private:
  LoggingLevel m_LoggingLevel;
  const std::string m_ClassName;
};
