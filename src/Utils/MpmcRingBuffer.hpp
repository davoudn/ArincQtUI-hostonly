template <typename T, size_t N>
class MpmcRingBuffer {
    static_assert((N & (N - 1)) == 0, "N must be power of two");

    struct Cell {
        std::atomic<size_t> sequence;
        T data;
    };

    alignas(64) std::atomic<size_t> enqueue_pos_{0};
    alignas(64) std::atomic<size_t> dequeue_pos_{0};
    std::array<Cell, N> buffer_;

public:
    MpmcRingBuffer() {
        for (size_t i = 0; i < N; ++i)
            buffer_[i].sequence.store(i, std::memory_order_relaxed);
    }

    bool push(const T& item) {
        size_t pos = enqueue_pos_.load(std::memory_order_relaxed);
        for (;;) {
            Cell& cell = buffer_[pos & (N - 1)];
            size_t seq = cell.sequence.load(std::memory_order_acquire);
            intptr_t diff = (intptr_t)seq - (intptr_t)pos;

            if (diff == 0) {
                if (enqueue_pos_.compare_exchange_weak(
                        pos, pos + 1, std::memory_order_relaxed))
                    break;
            } else if (diff < 0) {
                return false; // full
            } else {
                pos = enqueue_pos_.load(std::memory_order_relaxed);
            }
        }
        Cell& cell = buffer_[pos & (N - 1)];
        cell.data = item;
        cell.sequence.store(pos + 1, std::memory_order_release);
        return true;
    }

    bool pop(T& out) {
        size_t pos = dequeue_pos_.load(std::memory_order_relaxed);
        for (;;) {
            Cell& cell = buffer_[pos & (N - 1)];
            size_t seq = cell.sequence.load(std::memory_order_acquire);
            intptr_t diff = (intptr_t)seq - (intptr_t)(pos + 1);

            if (diff == 0) {
                if (dequeue_pos_.compare_exchange_weak(
                        pos, pos + 1, std::memory_order_relaxed))
                    break;
            } else if (diff < 0) {
                return false; // empty
            } else {
                pos = dequeue_pos_.load(std::memory_order_relaxed);
            }
        }
        Cell& cell = buffer_[pos & (N - 1)];
        out = cell.data;
        cell.sequence.store(pos + N, std::memory_order_release);
        return true;
    }
};

