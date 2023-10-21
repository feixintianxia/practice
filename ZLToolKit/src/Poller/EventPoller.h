#ifndef EventPoller_h
#define EventPoller_h

#include <mutex>
#include <thread>
#include <string>
#include <functional>
#include <memory>
#include <unordered_map>
#include "PipeWrap.h"
#include "Util/logger.h"
#include "Util/List.h"
#include "Thread/TaskExecutor.h"
#include "Thread/TreadPool.h"
#include "Network/Buffer.h"

#if defined(__linux__) || defined(__linux)
#define HAS_EPOLL
#endif //__linux__

namespace toolkit {

class EventPoller : public TaskExecutor, public AnyStorage, public std::enable_shared_from_this<EventPoller> {
  public:
    friend class TaskExecutorGetterImp;

    using Ptr = std::shared_ptr<EventPoller>;
    using Poll

};
}


#endif
