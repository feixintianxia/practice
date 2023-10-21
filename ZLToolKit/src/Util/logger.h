#ifndef UTIL_LOGGER_H_
#define UTIL_LOGGER_H_

#include <map>
#include <memory>

namespace toolkit {

class LogContext;
class LogChannel;
class LogWriter;
class Logger;

using LogContextPtr = std::shared_ptr<LogContext>;

typedef enum { LTrace = 0, LDebug, LInfo, LWarn, LError } LogLevel;

Logger *getLogger();
void setLogger(Logger *logger);

class Logger : public std::enable_shared_from<Logger>, public noncopyable {
  public:
    friend class AsyncLogWriter;
    using Ptr = std::shared_ptr<Logger>;

    /**
     * 获取日志单例
     * @return
     **/
    static Logger &Instance();

    explicit Logger(const std::string &loggerName);
    ~Logger();

    /**
     * 添加日志通道， 非线程安全
     * @param channel log通道
     **/
    void add(const std::shared_ptr<LogChannel> &channel);

    /**
     * 删除日志通道， 非线程安全
     * @
     **/
    void del(const std::string &name);

    /**
     * 获取日志通道， 非线程安全
     * @param name 线程通道
     **/
    std::shared_ptr<LogChannel> get(const std::string &name);

    /**
     * 设置写log器，非线程安全
     * @param writer 写log器
     **/
    void setWriter(const std::shared_ptr<LogWriter> &writer);

    /**
     * 设置所有日志通道的log等级
     * @param  level log等级
     **/
    void setLevel(LogLevel level);

    /**
     * 获取logger名
     * @return logger名
     **/
    const std::string &getName() const;

    /**
     * 写日志
     * @param ctx 日志信息
     **/
    void write(const LogContextPtr &ctx);

  private:
    void writeChannels(const LogContextPtr &ctx);
    void writeChannels_l(const LogContextPtr &ctx);

  private:
    LogContextPtr _last__log;
    std::string _logger_name;
    std::shared_ptr<LogWriter> _writer;
    std::map<std::string, std::shared_ptr<LogChannel>> _channels;
};

////////////////////////////////LogContext/////////////////////////////////
/**
 * 日志上下文
 **/
class LogContext : public std::ostringstream {
  public:
    LogContext() = default;
    LogContext(LogLevel level, const char *file, const char *function, int line,
               const char *module_name, const char *flag);
    ~LogContext() = default;

    LogLevel _level;
    int _line;
    int _repeat = 0;
    std::string _file;
    std::string _function;
    std::string _thread_name;
    std::string _module_name;
    std::string _flag;
    struct timeval _tv;

    const std::string &str();

  private:
    bool _got_content = false;
    std::string _content;
};

} /*namespace toolkit*/
#endif /*UTIL_LOGGER_H*/