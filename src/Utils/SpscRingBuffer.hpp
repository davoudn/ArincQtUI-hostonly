#pragma once

#include <atomic>
#include <cstddef>
#include <optional>

template <typename T, size_t N>
class SpscRingBuffer {
    static_assert(N > 0, "N must be positive");

public:
    SpscRingBuffer() : rc_(0), wc_(0) {}

    // Called only by the single producer thread.
    bool push(const T& item) {
        size_t w = wc_.load(std::memory_order_relaxed);
        size_t r = rc_.load(std::memory_order_acquire); // see consumer progress

        if (w - r == N) {
            return false; // full
        }

        buffer_[w % N] = item;
        wc_.store(w + 1, std::memory_order_release); // publish data
        return true;
    }

    // Called only by the single consumer thread.
    std::optional<T> pop() {
        size_t r = rc_.load(std::memory_order_relaxed);
        size_t w = wc_.load(std::memory_order_acquire); // see producer's data

        if (w - r == 0) {
            return std::nullopt; // empty
        }

        T item = buffer_[r % N];
        rc_.store(r + 1, std::memory_order_release); // free the slot
        return item;
    }

    static constexpr size_t capacity() { return N; }

private:
    alignas(64) std::atomic<size_t> rc_;
    alignas(64) std::atomic<size_t> wc_;

    T buffer_[N];
};

