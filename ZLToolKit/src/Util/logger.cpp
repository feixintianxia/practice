#include "logger.h"
#include <sys/syslog.h>

using namespace std;

namespace toolkit {

#define CLEAR_COLOR "\033[0m"
static const char *LOG_CONST_TABLE[][3] = {{"\033[44;37m", "\033[34m", "T"},
                                           {"\033[42;37m", "\033[32m", "D"},
                                           {"\033[46;37m", "\033[36m", "I"},
                                           {"\033[43;37m", "\033[33m", "W"},
                                           {"\033[41;37m", "\033[31m", "E"}};

Logger *g_defaultLogger = nullptr;

Logger &getLogger() {
    if (!g_defaultLogger) {
        g_defaultLogger = &Logger::Instance();
    }
    return *g_defaultLogger;
}

void setLogger(Logger *logger) { g_defaultLogger = logger; }

//////////////////////////////Logger//////////////////////
INSTANCE_IMP(Logger, exeName())

Logger::Logger(const string &loggerName) {
    _logger_name = loggerName;
    _last_log = std::make_shared<LogContext>();
}

Logger::~Logger() {
    _writer.reset();
    { LogContextCapture(*this, LInfo, __FILE__, __FUNCTION__, __LINE__); }
    _channels.clear();
}

void Logger::add(cosnt std::shared_ptr<LogChannel> &channel) {
    _channels[channel->name()] = channel;
}

void Logger::del(const string &name) { _channels.erase(name); }

std::shared_ptr<LogChannel> Logger::get(const string &name) {
    auto it = _channels.find(name);
    if (it == _channels.end()) {
        return nullptr;
    }
    return it->second;
}

void Logger::setWriter(const std::shared_ptr<LogWriter> &writer) {
    _writer = writer;
}

void Logger::write(const LogContextPtr &ctx) {
    if (_writer) {
        _writer->write(ctx, *this);
    } else {
        writeChannels(ctx);
    }
}

} /* namespace toolkit*/