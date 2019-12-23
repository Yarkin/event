// Copyright (c) 2017-2018 Telos Foundation & contributors
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#include "subscriber.h"
ns2_begin(top, event)

int32_t xsubscriber::on_event(xevent_t * ev) {
    auto iter = handlers.find(ev->type());
    if (iter != handlers.end()) {
        auto handle = iter->second;
        return handle(ev);
    }
    return -1;
}

bool xsubscriber::init() {
    sub_imp();
    return true;
}

ns2_end()