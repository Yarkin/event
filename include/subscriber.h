// Copyright (c) 2017-2018 Telos Foundation & contributors
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#pragma once
#include <set>
#include <map>
#include <mutex>
#include <atomic>
#include <vector>
#include "event.h"

ns2_begin(top, event)

#ifndef SUBSCRIBE_DECL
#define SUBSCRIBE_DECL() \
protected: \
virtual void sub_imp();
#endif // CB_REG_DECL

#ifndef SUBSCRIBE_BEGIN
#define SUBSCRIBE_BEGIN(cls) \
void cls::sub_imp() {
#endif // SUB_REG_IMP_BEGIN

#ifndef SUBSCRIBE_END
#define SUBSCRIBE_END() \
}
#endif // CB_REG_IMP_END

#ifndef SUBSCRIBE
#define SUBSCRIBE(cls, ev_type, func) \
{ \
auto cb = std::bind(&cls::func, this, std::placeholders::_1);\
handlers[ev_type] = cb;\
}
#endif // SUBSCRIBE

class xsubscriber : public subscriber {
 protected:
    using event_cb = std::function<int32_t(xevent_t* event)>;
    using event_handlers = std::map<event_type, event_cb>;
 protected:
    virtual void sub_imp() = 0;
 public:
    virtual int32_t on_event(xevent_t * event);
    virtual bool init();
    virtual void id(uint32_t id) {client_id = id;}
    virtual uint32_t id() {return client_id;}
 private:
    uint32_t client_id;
 protected:
    event_handlers handlers;
};

ns2_end()
