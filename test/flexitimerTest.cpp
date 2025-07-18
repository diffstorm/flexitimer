#include <gtest/gtest.h>
#include "flexitimer.h"

extern "C" {
    static int callback_count = 0;
    void test_callback(timer_id_t id)
    {
        callback_count++;
    }
}

class FlexiTimerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        flexitimer_init();
        callback_count = 0;
    }
};

TEST_F(FlexiTimerTest, InitResetsAllTimers)
{
    timer_state_t state;

    for(timer_id_t i = 0; i < FLEXITIMER_MAX_TIMERS; i++)
    {
        flexitimer_get_state(i, &state);
        EXPECT_EQ(state, TIMER_STATE_PASSIVE);
    }
}

TEST_F(FlexiTimerTest, StartValidTimer)
{
    auto result = flexitimer_start(0, TIMER_TYPE_SINGLESHOT, 100, test_callback);
    EXPECT_EQ(result, FLEXITIMER_OK);
}

TEST_F(FlexiTimerTest, StartInvalidTimerId)
{
    auto result = flexitimer_start(FLEXITIMER_MAX_TIMERS, TIMER_TYPE_SINGLESHOT, 100, test_callback);
    EXPECT_EQ(result, FLEXITIMER_ERROR_INVALID_ID);
}

TEST_F(FlexiTimerTest, PeriodicTimerZeroTimeout)
{
    auto result = flexitimer_start(0, TIMER_TYPE_PERIODIC, 0, test_callback);
    EXPECT_EQ(result, FLEXITIMER_ERROR_ZERO_TIMEOUT);
}

TEST_F(FlexiTimerTest, CallbackInvokedSingleShot)
{
    flexitimer_start(0, TIMER_TYPE_SINGLESHOT, 1, test_callback);
    flexitimer_handler(); // Triggers callback
    EXPECT_EQ(callback_count, 1);
    flexitimer_handler(); // No effect
    EXPECT_EQ(callback_count, 1);
}

TEST_F(FlexiTimerTest, CallbackInvokedPeriodic)
{
    flexitimer_start(0, TIMER_TYPE_PERIODIC, 1, test_callback);
    flexitimer_handler(); // Triggers callback
    EXPECT_EQ(callback_count, 1);
    flexitimer_handler(); // Triggers again
    EXPECT_EQ(callback_count, 2);
}

TEST_F(FlexiTimerTest, DelayActiveTimer)
{
    flexitimer_start(0, TIMER_TYPE_SINGLESHOT, 3, nullptr);
    EXPECT_EQ(flexitimer_delay(0, 2), FLEXITIMER_OK);
    timer_time_t remaining;
    flexitimer_get_elapsed(0, &remaining);
    EXPECT_EQ(remaining, 5); // 3 + 2
}

TEST_F(FlexiTimerTest, DelayInvalidState)
{
    EXPECT_EQ(flexitimer_delay(0, 2), FLEXITIMER_ERROR_INVALID_STATE);
}

TEST_F(FlexiTimerTest, PauseResumeTimer)
{
    flexitimer_start(0, TIMER_TYPE_SINGLESHOT, 2, nullptr);
    EXPECT_EQ(flexitimer_pause(0), FLEXITIMER_OK);
    timer_state_t state;
    flexitimer_get_state(0, &state);
    EXPECT_EQ(state, TIMER_STATE_PAUSED);
    EXPECT_EQ(flexitimer_resume(0), FLEXITIMER_OK);
    flexitimer_get_state(0, &state);
    EXPECT_EQ(state, TIMER_STATE_ACTIVE);
}

TEST_F(FlexiTimerTest, RestartTimer)
{
    flexitimer_start(0, TIMER_TYPE_SINGLESHOT, 2, test_callback);
    flexitimer_handler(); // remaining=1
    EXPECT_EQ(flexitimer_restart(0), FLEXITIMER_OK);
    timer_time_t remaining;
    flexitimer_get_elapsed(0, &remaining);
    EXPECT_EQ(remaining, 2);
}

TEST_F(FlexiTimerTest, CancelTimer)
{
    flexitimer_start(0, TIMER_TYPE_SINGLESHOT, 1, test_callback);
    flexitimer_cancel(0);
    timer_state_t state;
    flexitimer_get_state(0, &state);
    EXPECT_EQ(state, TIMER_STATE_PASSIVE);
    flexitimer_handler(); // Should not trigger callback
    EXPECT_EQ(callback_count, 0);
}

TEST_F(FlexiTimerTest, GetFunctionsWithNullArg)
{
    EXPECT_EQ(flexitimer_get_state(0, nullptr), FLEXITIMER_ERROR_INVALID_ARG);
    EXPECT_EQ(flexitimer_get_type(0, nullptr), FLEXITIMER_ERROR_INVALID_ARG);
    EXPECT_EQ(flexitimer_get_time(0, nullptr), FLEXITIMER_ERROR_INVALID_ARG);
    EXPECT_EQ(flexitimer_get_elapsed(0, nullptr), FLEXITIMER_ERROR_INVALID_ARG);
}

TEST_F(FlexiTimerTest, MultipleTimersIndependent)
{
    flexitimer_start(0, TIMER_TYPE_SINGLESHOT, 1, test_callback);
    flexitimer_start(1, TIMER_TYPE_PERIODIC, 2, test_callback);
    flexitimer_handler(); // Timer 0 triggers, Timer 1 decrements to 1
    EXPECT_EQ(callback_count, 1);
    flexitimer_handler(); // Timer 1 triggers, count=2
    EXPECT_EQ(callback_count, 2);
    flexitimer_handler(); // Timer 1 decrements to 1
    flexitimer_handler(); // Timer 1 triggers, count=3
    EXPECT_EQ(callback_count, 3);
}

TEST_F(FlexiTimerTest, RestartPassiveSingleShotTimer)
{
    flexitimer_start(0, TIMER_TYPE_SINGLESHOT, 1, test_callback);
    flexitimer_handler(); // Timer expires
    EXPECT_EQ(callback_count, 1);
    timer_state_t state;
    flexitimer_get_state(0, &state);
    EXPECT_EQ(state, TIMER_STATE_PASSIVE);

    EXPECT_EQ(flexitimer_restart(0), FLEXITIMER_OK);
    flexitimer_get_state(0, &state);
    EXPECT_EQ(state, TIMER_STATE_ACTIVE);
    timer_time_t remaining;
    flexitimer_get_elapsed(0, &remaining);
    EXPECT_EQ(remaining, 1); // Should be reset to original timeout

    flexitimer_handler(); // Should trigger callback again
    EXPECT_EQ(callback_count, 2);
}