// Sync
// Copyright © 2021 Sean Farrell <sean.farrell@rioki.org>
//
// This program is free software. It comes without any warranty, to
// the extent permitted by applicable law. You can redistribute it
// and/or modify it under the terms of the Do What The Fuck You Want
// To Public License, Version 2, as published by Sam Hocevar. See
// http://www.wtfpl.net/ for more details.

#ifndef _RIOKI_SYNC_H_
#define _RIOKI_SYNC_H_

#include <cassert>
#include <memory>
#include <thread>
#include <future>
#include <functional>

#ifdef SYNC_DLL
#define SYNC_EXPORT __declspec(dllexport)
#else
#define SYNC_EXPORT
#endif

namespace sync
{
    //! Set the main thead id.
    //!
    //! Before using sync without the thread id, you should set the main thread id.
    //! This can be any thread, as long as the thread calls sync_point
    //! regularly.
    //!
    //! @param id the thread id of the main thread.
    SYNC_EXPORT void set_main_thread_id(const std::thread::id& id) noexcept;

    //! Get the main thread id.
    //!
    //! @returns the main thread id set by set_main_thread_id
    //!
    //! @see set_main_thread_id
    SYNC_EXPORT std::thread::id get_main_thread_id() noexcept;

    //! Queue action to be exectued by the given thread.
    //!
    //! @param thread the thread id of the thread to call on
    //! @param func the fuction to call in the give thread
    //!
    //! @warning If any exception is thrown out of the function
    //! the sync_point will call terminate. Use sync<T> to get a
    //! future that can handle the exception.
    //!
    //! @see sync_point
    SYNC_EXPORT void sync(const std::thread::id& thread, const std::function<void ()>& func) noexcept;

    //! Queue action to be executed by the main thread.
    //!
    //! @func the function to call in the main thread
    //!
    //! @warning If any exception is thrown out of the function
    //! the sync_point will call terminate. Use sync<T> to get a
    //! future that can handle the exception.
    //!
    //! @see set_main_thread_id
    //! @see sync_point
    SYNC_EXPORT void sync(const std::function<void ()>& func) noexcept;

    //! Queue action to be exectued by the given thread with result.
    //!
    //! @param thread the thread id of the thread to call on
    //! @param func the fuction to call in the give thread
    //!
    //! @see sync_point
    template <typename T>
    std::future<T> sync(const std::thread::id& thread, const std::function<T()>& func) noexcept
    {
        auto task = std::make_shared<std::packaged_task<T()>>(func);
        auto future = task->get_future();
        sync(thread, [task] () mutable {
            (*task)();
        });
        return future;
    }

    //! Queue task to be executed on the main thread with result.
    //!
    //! @func the function to call in the main thread
    //!
    //! @see set_main_thread_id
    //! @see sync_point
    template <typename T>
    std::future<T> sync(const std::function<T()>& func) noexcept
    {
        return sync<T>(get_main_thread_id(), func);
    }

    //! Delay action until next time tick is called on this thread.
    //!
    //! @param func the function to call later
    //!
    //! @warning If any exception is thrown out of the function
    //! the sync_point will call terminate. Use sync<T> to get a
    //! future that can handle the exception.
    //!
    //! @see sync_point
    SYNC_EXPORT void delay(const std::function<void ()>& func) noexcept;

    //! Execute alle queueed tasks for this thread.
    //!
    //! This function actually calls the tasks queued by sync and delay.
    SYNC_EXPORT void sync_point() noexcept;
}

#endif
