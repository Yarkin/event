// Copyright (c) 2017-2018 Telos Foundation & contributors
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#pragma once
#include <set>
#include <map>
#include <mutex>
#include <queue>
#include <atomic>
#include "event.h"

ns2_begin(top, event)

class xbroker : public broker {
 protected:
    // subscribe
    using subscriber_ptr_t = std::shared_ptr<subscriber>;
    using subscribers = std::map<uint32_t, subscriber_ptr_t>;

    // event process
    using event_ptr_t = std::shared_ptr<xevent_t>;
    struct aysnc_event {
        event_ptr_t event_ptr;
        pub_cb cb;
    };
    using aysnc_event_t = aysnc_event;

 public:
  xbroker();

 public:
    virtual uint32_t subscribe(e_major_event_type e_type, subscriber* pclient);
    virtual bool unsubscribe(e_major_event_type e_type, uint32_t client_id);
    virtual int32_t publish(xevent_t* event, pub_cb cb);

 protected:
    virtual int32_t notify_all(const aysnc_event_t & event);
    virtual int32_t aysnc_pub(const aysnc_event_t & ev);

 protected:
    std::map<e_major_event_type, subscribers> listeners;
    std::mutex listeners_lock;
    std::atomic_uint client_id;

    std::queue<aysnc_event_t> aysnc_pool;
    std::mutex aysnc_lock;
    std::atomic_uint pub_id;
};

ns2_end()
