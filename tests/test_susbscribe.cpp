#include <gtest/gtest.h>
#include "mock_subscriber.h"

using top::event::test::xmock_xx_subscriber;
using top::event::test::xmock_yy_subscriber;
using top::event::e_major_event_type;
using top::event::test::xx1_data_event_t;
using top::event::test::xx2_data_event_t;
using top::event::test::xx1_data;
using top::event::test::xx2_data;
using top::event::test::yy_data;
using top::event::test::yy_data_event_t;
class test_subscribe : public testing::Test {
 protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

TEST_F(test_subscribe, subscribe_macro) {
    xmock_xx_subscriber mock_sub;
    mock_sub.init();
    xx1_data_event_t xx1_event(new xx1_data());
    xx2_data_event_t xx2_event(new xx2_data());

    EXPECT_TRUE(xx1_event.type() == TO_EV_TYPE(top::event::e_xx, top::event::e_xx1));
    EXPECT_TRUE(xx1_event.major_type() == top::event::e_xx);
    EXPECT_TRUE(xx1_event.minor_type() == top::event::e_xx1);
}

TEST_F(test_subscribe, sub_process) {
    xmock_xx_subscriber mock_sub;
    mock_sub.init();
    xx1_data_event_t xx1_event(new xx1_data());
    xx2_data_event_t xx2_event(new xx2_data());
    auto ret = mock_sub.on_event(&xx1_event);
    auto ret2 = mock_sub.on_event(&xx2_event);
    yy_data_event_t yy_event(new yy_data());
    auto rety = mock_sub.on_event(&yy_event);

    EXPECT_EQ(ret, 1);
    EXPECT_EQ(ret2, 1);
    EXPECT_EQ(rety, -1);

    xmock_yy_subscriber yy_mock_sub;
    yy_mock_sub.init();
    auto retyy = yy_mock_sub.on_event(&yy_event);
    EXPECT_EQ(retyy, 1);

}