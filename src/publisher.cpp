// Copyright (c) 2017-2018 Telos Foundation & contributors
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "publisher.h"

ns2_begin(top, event)

int32_t xpublisher::pub(broker* broker, xevent_t* e, pub_cb cb) {
    return broker->publish(e, cb);
}

ns2_end()