// Copyright (c) 2017-2018 Telos Foundation & contributors
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <gtest/gtest.h>
#include "mock_subscriber.h"
#include "broker.h"
#include "publisher.h"

using top::event::test::xmock_xx_subscriber;
using top::event::test::xmock_yy_subscriber;
using top::event::e_major_event_type;
using top::event::xbroker;
using top::event::test::xx1_data_event_t;
using top::event::test::xx2_data_event_t;
using top::event::test::xx1_data;
using top::event::test::xx2_data;
using top::event::test::yy_data;
using top::event::test::yy_data_event_t;
using top::event::xpublisher;
class test_broker : public testing::Test {
 protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

TEST_F(test_broker, sub_notify) {
    xmock_xx_subscriber *mock_sub = new xmock_xx_subscriber();
    mock_sub->init();
    xbroker broker;
    auto ret = broker.subscribe(top::event::e_xx, mock_sub);
    EXPECT_GT(ret, 0);
    xpublisher publisher;

    auto ret1 = publisher.pub(&broker, new xx1_data_event_t(new xx1_data()));
    auto ret2 = publisher.pub(&broker, new xx2_data_event_t(new xx2_data()));

    EXPECT_EQ(ret, 1);
    EXPECT_EQ(ret2, 1);

    auto rety = publisher.pub(&broker, new yy_data_event_t(new yy_data()));
    EXPECT_EQ(rety, -1);
}

TEST_F(test_broker, bench) {
    xmock_xx_subscriber *mock_sub = new xmock_xx_subscriber();
    mock_sub->init();
    xbroker broker;
    auto ret = broker.subscribe(top::event::e_xx, mock_sub);
    EXPECT_GT(ret, 0);
    xpublisher publisher;

    for (auto index =0; index < 99999; index++) {
        auto ret1 = publisher.pub(&broker,
                        new xx1_data_event_t(new xx1_data()));
        auto ret2 = publisher.pub(&broker,
                        new xx2_data_event_t(new xx2_data()));

        EXPECT_EQ(ret, 1);
        EXPECT_EQ(ret2, 1);

        auto rety = publisher.pub(&broker, new yy_data_event_t(new yy_data()));
        EXPECT_EQ(rety, -1);
    }
}
