// Copyright (c) 2017-2018 Telos Foundation & contributors
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#pragma once
#include <string>
#include <functional>
#include "common.h"

ns2_begin(top, event)

enum e_major_event_type {
  e_vc   = 0,
  e_cons = 1,
  e_sync = 2,
  e_xx   = 3,
  e_yy   = 4,
};

enum e_minor_event_type{
  e_vc_req  = 0,
  e_vc_resp = 1,

  e_cons_req  = 2,
  e_cons_resp = 3,

  e_sync_req = 4,
  e_sync_resp = 5,

  e_xx1      = 6,
  e_xx2      = 7,

  e_yy1      = 8,
  e_yy2      = 9
};

using event_type = uint32_t;

#define TO_EV_TYPE(major_type, minor_type) \
(top::event::event_type)((major_type) << 16 | ((minor_type) & 0xFFFF))

#define PARSE_MAJOR(ev_type) \
(e_major_event_type)((top::event::event_type)((ev_type) >> 16) & 0xFFFF)

#define PARSE_MINOR(ev_type) \
(e_minor_event_type)((top::event::event_type)(ev_type) & 0xFFFF)

class xevent_t {
 public:
    virtual event_type type() = 0;
    virtual e_major_event_type major_type() = 0;
    virtual e_minor_event_type minor_type() = 0;
    virtual bool sync() = 0;
};

template<class T, e_major_event_type major, e_minor_event_type minor>
class xevent : public xevent_t {
 public:
    typedef std::shared_ptr<T> TPtr;
 public:
    explicit xevent(T * ptr)
     : data(ptr), e_type(TO_EV_TYPE(major, minor)) {}
 public:
    virtual event_type type() { return e_type; }
    virtual e_major_event_type major_type() { return PARSE_MAJOR(type()); }
    virtual e_minor_event_type minor_type() { return PARSE_MINOR(type()); }
    virtual bool sync() { return true; }
    TPtr value() {return data;}
 private:
    event_type e_type;
    TPtr data;
};

#define DECL_EVENT_BEGIN(cls, major, minor) \
class cls##_event_t : public xevent<cls,  major, minor> { \
 public:\
  cls##_event_t(cls* ptr) \
    : xevent<cls, major, minor>(ptr) {}

#define DECL_EVENT_END() };

class subscriber {
 public:
    virtual int32_t on_event(xevent_t * event) = 0;
    virtual bool init() = 0;
};

using pub_cb = std::function<void(int32_t, xevent_t* event)>;
class broker {
 public:
    virtual uint32_t subscribe(e_major_event_type type, subscriber* psub) = 0;
    virtual bool unsubscribe(e_major_event_type type, uint32_t client_id) = 0;
    virtual int32_t publish(xevent_t* event, pub_cb cb) = 0;
};

class publisher {
 public:
    virtual int32_t pub(broker* broker, xevent_t* e, pub_cb cb = nullptr) = 0;
};

ns2_end()
