// Sync
// Copyright © 2021 Sean Farrell <sean.farrell@rioki.org>
//
// This program is free software. It comes without any warranty, to
// the extent permitted by applicable law. You can redistribute it
// and/or modify it under the terms of the Do What The Fuck You Want
// To Public License, Version 2, as published by Sam Hocevar. See
// http://www.wtfpl.net/ for more details.

#include "sync.h"

#include <stdexcept>
#include <mutex>
#include <map>
#include <iostream>

namespace sync
{
    std::mutex tasks_mutex;
    std::map<std::thread::id, std::vector<std::function<void ()>>> tasks;
    std::atomic<std::thread::id> main_thread_id;

    void set_main_thread_id(const std::thread::id& id) noexcept
    {
        main_thread_id = id;
    }

    std::thread::id get_main_thread_id() noexcept
    {
        return main_thread_id;
    }

    void delay(const std::function<void()>& func) noexcept
    {
        sync(std::this_thread::get_id(), func);
    }

    void sync(const std::function<void()>& func) noexcept
    {
        assert(main_thread_id != std::thread::id());
        sync(main_thread_id, func);
    }

    void sync(const std::thread::id& thread, const std::function<void()>& func) noexcept
    {
        assert(func);
        std::scoped_lock<std::mutex> sl(tasks_mutex);
        tasks[thread].push_back(func);
    }

    std::vector<std::function<void()>> get_this_threads_tasks() noexcept
    {
        std::scoped_lock<std::mutex> sl(tasks_mutex);
        auto this_threads_tasks = tasks[std::this_thread::get_id()];
        tasks[std::this_thread::get_id()] = std::vector<std::function<void ()>>();
        return this_threads_tasks;
    }

    void sync_point() noexcept
    {
        auto tasks = get_this_threads_tasks();
        for (const auto& task : tasks)
        {
            try
            {
                task();
            }
            catch (const std::exception& ex)
            {
                std::cerr << ex.what() << std::endl;
                std::terminate();
            }
            catch (...)
            {
                std::terminate();
            }
        }
    }
}
