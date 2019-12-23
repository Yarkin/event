#include <gtest/gtest.h>
#include "mock_subscriber.h"

ns3_begin(top, event, test)

int32_t xmock_xx_subscriber::on_xx1_event(xevent_t * ev) {
    EXPECT_NE(ev->type(), TO_EV_TYPE(e_xx, e_xx2));
    EXPECT_EQ(ev->type(), TO_EV_TYPE(e_xx, e_xx1));
    return 1;
}

int32_t xmock_xx_subscriber::on_xx2_event(xevent_t * ev) {
    EXPECT_NE(ev->type(), TO_EV_TYPE(e_xx, e_xx1));
    EXPECT_EQ(ev->type(), TO_EV_TYPE(e_xx, e_xx2));
    return 1;
}

SUBSCRIBE_BEGIN(xmock_xx_subscriber)
    SUBSCRIBE(xmock_xx_subscriber, TO_EV_TYPE(e_xx, e_xx1), on_xx1_event)
    SUBSCRIBE(xmock_xx_subscriber, TO_EV_TYPE(e_xx, e_xx2), on_xx2_event)
SUBSCRIBE_END()

int32_t xmock_yy_subscriber::on_yy1_event(xevent_t * ev) {
    EXPECT_NE(ev->type(), TO_EV_TYPE(e_yy, e_yy2));
    EXPECT_NE(ev->type(), TO_EV_TYPE(e_xx, e_xx2));
    EXPECT_NE(ev->type(), TO_EV_TYPE(e_xx, e_xx1));
    EXPECT_EQ(ev->type(), TO_EV_TYPE(e_yy, e_yy1));
    return 1;
}

SUBSCRIBE_BEGIN(xmock_yy_subscriber)
    SUBSCRIBE(xmock_yy_subscriber, TO_EV_TYPE(e_yy, e_yy1), on_yy1_event)
SUBSCRIBE_END()


ns3_end()