// Sync
// Copyright © 2021 Sean Farrell <sean.farrell@rioki.org>
//
// This program is free software. It comes without any warranty, to
// the extent permitted by applicable law. You can redistribute it
// and/or modify it under the terms of the Do What The Fuck You Want
// To Public License, Version 2, as published by Sam Hocevar. See
// http://www.wtfpl.net/ for more details.

#include <rex/sync.h>
#include <gtest/gtest.h>

using namespace std::literals::chrono_literals;
TEST(sync, empty_tick)
{
    sync::sync_point();
}

TEST(sync, set_main_thread)
{
    sync::set_main_thread_id(std::this_thread::get_id());
    EXPECT_EQ(std::this_thread::get_id(), sync::get_main_thread_id());
}

TEST(sync, sync_different_thread)
{
    std::thread::id exec_id;

    std::thread thread_a([&] () {
        while (exec_id == std::thread::id())
        {
            std::this_thread::sleep_for(50ms);
            sync::sync_point();

        }
    });

    std::thread thread_b([&] () {
        sync::sync(thread_a.get_id(), [&] () {
            exec_id = std::this_thread::get_id();
        });
    });

    auto id_a = thread_a.get_id();
    thread_a.join();
    thread_b.join();

    EXPECT_EQ(id_a, exec_id);
}
TEST(sync, sync_main_thread)
{
    std::thread::id exec_id;
    sync::set_main_thread_id(std::this_thread::get_id());

    std::thread worker([&] () {
        sync::sync([&] () {
            exec_id = std::this_thread::get_id();
        });
    });

    while (exec_id == std::thread::id())
    {
        std::this_thread::sleep_for(50ms);
        sync::sync_point();

    }

    worker.join();

    EXPECT_EQ(std::this_thread::get_id(), exec_id);
}

TEST(sync, sync_different_thread_with_result)
{
    std::thread::id exec_id;
    unsigned int result = 0;

    std::thread thread_a([&] () {
        while (exec_id == std::thread::id())
        {
            std::this_thread::sleep_for(50ms);
            sync::sync_point();

        }
    });

    std::thread thread_b([&] () {
        auto f = sync::sync<unsigned int>(thread_a.get_id(), [&] () {
            exec_id = std::this_thread::get_id();
            return 42;
        });
        result = f.get();
    });

    auto id_a = thread_a.get_id();
    thread_a.join();
    thread_b.join();

    EXPECT_EQ(id_a, exec_id);
    EXPECT_EQ(42, result);
}
TEST(sync, sync_main_thread_with_result)
{
    std::thread::id exec_id;
    unsigned int result = 0;
    sync::set_main_thread_id(std::this_thread::get_id());

    std::thread worker([&] () {
        auto f = sync::sync<unsigned int>([&] () {
            exec_id = std::this_thread::get_id();
            return 42;
        });
        result = f.get();
    });

    while (exec_id == std::thread::id())
    {
        std::this_thread::sleep_for(50ms);
        sync::sync_point();
    }

    worker.join();

    EXPECT_EQ(std::this_thread::get_id(), exec_id);
    EXPECT_EQ(42, result);
}

TEST(sync, delay)
{
    unsigned int result = 0;

    sync::delay([&] () {
        result = 42;
    });

    while (result == 0)
    {
        std::this_thread::sleep_for(50ms);
        sync::sync_point();
    }

    EXPECT_EQ(42, result);
}

// NOTE: Don't get smart, death tests don't work over thread boundaries.

TEST(sync, sync_throw_terminates)
{
    sync::set_main_thread_id(std::this_thread::get_id());
    sync::sync(std::this_thread::get_id(), [&] () {
        throw std::runtime_error("YOLO");
    });
    EXPECT_DEATH(sync::sync_point(), "YOLO");
}

TEST(sync, sync_main_throw_terminates)
{
    sync::set_main_thread_id(std::this_thread::get_id());
    sync::sync([&] () {
        throw std::runtime_error("YOLO");
    });
    EXPECT_DEATH(sync::sync_point(), "YOLO");
}

TEST(sync, delay_throw_terminates)
{
    sync::delay([&] () {
        throw std::runtime_error("YOLO");
    });
    EXPECT_DEATH(sync::sync_point(), "YOLO");
}

