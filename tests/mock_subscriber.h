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
#include "subscriber.h"

ns3_begin(top, event, test)
using top::event::xevent_t;
class xmock_xx_subscriber : public xsubscriber {
 public:
    virtual int32_t on_xx1_event(xevent_t * ev);
    virtual int32_t on_xx2_event(xevent_t * ev);

    SUBSCRIBE_DECL()
};

class xmock_yy_subscriber : public xsubscriber {
 public:
    virtual int32_t on_yy1_event(xevent_t * ev);

    SUBSCRIBE_DECL()
};

class xx1_data {

};

class xx2_data {

};

class yy_data {

};

DECL_EVENT_BEGIN(xx1_data, e_xx, e_xx1)
DECL_EVENT_END()

DECL_EVENT_BEGIN(xx2_data, e_xx, e_xx2)
DECL_EVENT_END()

DECL_EVENT_BEGIN(yy_data, e_yy, e_yy1)
DECL_EVENT_END()

ns3_end()
