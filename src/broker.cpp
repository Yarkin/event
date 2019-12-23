// Copyright (c) 2017-2018 Telos Foundation & contributors
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "broker.h"
ns2_begin(top, event)

xbroker::xbroker() :client_id(0), pub_id(0) {}

uint32_t xbroker::subscribe(e_major_event_type e_type, subscriber* pclient) {
    uint32_t id = 0;
    if (pclient != nullptr) {
        std::lock_guard<std::mutex> lock(listeners_lock);
        id = ++client_id;
        subscriber_ptr_t client_ptr(pclient);
        auto iter = listeners.find(e_type);
        if ( iter != listeners.end() ) {
            auto typed_listeners = iter->second;
            typed_listeners[id] = client_ptr;
        } else {
            subscribers typed_listeners;
            typed_listeners[id] = client_ptr;
            listeners[e_type] = typed_listeners;
        }
    }
    return id;
}

bool xbroker::unsubscribe(e_major_event_type e_type, uint32_t client_id) {
    bool b_unsubed = false;
    {
        std::lock_guard<std::mutex> lock(listeners_lock);
        auto iter = listeners.find(e_type);
        if (iter != listeners.end()) {
            auto typed_listeners = iter->second;
            auto client_iter = typed_listeners.find(client_id);
            if (client_iter != typed_listeners.end()) {
                typed_listeners.erase(client_iter);
                b_unsubed = true;
            }
            if (typed_listeners.empty()) {
                listeners.erase(iter);
            }
        }
    }
    return b_unsubed;
}

int32_t xbroker::publish(xevent_t* e, pub_cb cb) {
    event_ptr_t event_ptr(e);
    const aysnc_event_t a_ev {event_ptr, cb};
    if (cb == nullptr) {
        return notify_all(a_ev);
    } else {
        return aysnc_pub(a_ev);
    }
}

int32_t xbroker::notify_all(const aysnc_event_t & ev) {
    int32_t ret = 0;
    {
        subscribers sub;
        {
            std::lock_guard<std::mutex> lock(listeners_lock);
            auto iter = listeners.find(ev.event_ptr->major_type());
            if (iter != listeners.end()) {
                sub = iter->second;
            } else {
                ret = -1;
            }
        }

        if (!sub.empty()) {
            for (auto iter = sub.begin(); iter != sub.end(); iter++) {
                auto ev_ptr = ev.event_ptr.get();
                ret = iter->second->on_event(ev_ptr);
                if (ev.cb != nullptr) {
                    ev.cb(ret, ev_ptr);
                }
            }
        }
    }
    return ret;
}

int32_t xbroker::aysnc_pub(const aysnc_event_t & ev) {
    int32_t ret = 0;
    {
        std::lock_guard<std::mutex> lock(aysnc_lock);
        aysnc_pool.push(ev);
        ret = aysnc_pool.size();
    }
    return ret;
}

ns2_end()
