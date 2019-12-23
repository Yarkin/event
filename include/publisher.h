// Copyright (c) 2017-2018 Telos Foundation & contributors
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#pragma once
#include "event.h"

ns2_begin(top, event)

class xpublisher : public publisher {
 public:
    virtual int32_t pub(broker* broker, xevent_t* e, pub_cb cb = nullptr);
};

ns2_end()
