#ifndef PACK_H
#define PACK_H

#include <type_traits>
#include <limits>
#include <stdio.h>

namespace pack_detail {
    static constexpr size_t limit = std::numeric_limits<size_t>::max();

// (a) packSize



// base case, empty list
template <typename... Args>
constexpr size_t packSize = 0;

// recursion step, non-empty list
template <typename Arg, typename... Args>
constexpr size_t packSize<Arg, Args...> = 1 + packSize<Args...>;

// (b) packTypeToIndex
template <size_t QueryIndex, typename... Args>
struct _packIndexToType {
    // empty args and any index -- base case, type does not exist in pack
    static_assert(false);
};

template <typename Arg, typename... Args>
struct _packIndexToType<0, Arg, Args...> {
    // non-empty args and zero index -- base case, just return Arg
    using type = Arg;
};

template <size_t QueryIndex, typename Arg, typename... Args>
    requires(QueryIndex >= 0)
struct _packIndexToType<QueryIndex, Arg, Args...> {
    // non-empty args and non-zero index -- iterate over on list, decrementing index.
    using type = typename _packIndexToType<QueryIndex - 1, Args...>::type;
};

// copied over alias
template <size_t QueryIndex, typename... Args>
using packIndexToType = typename _packIndexToType<QueryIndex, Args...>::type;

// (c) packTypeToIndex

// helper:
// If no types left in list and we are still searching, it doesn't exist -- return -1
template <size_t Accumulated, typename QueryType, typename... Args>
constexpr size_t _packTypeToIndex = limit;

// Recursion case: check equality. If equal, return Accumulated, else recurse.
template <size_t Accumulated, typename QueryType, typename Arg, typename... Args>
constexpr size_t _packTypeToIndex<Accumulated, QueryType, Arg, Args...> =
    std::is_same_v<QueryType, Arg> ? Accumulated
                                   : _packTypeToIndex<Accumulated + 1, QueryType, Args...>;

template <typename QueryType, typename... Args>
constexpr size_t packTypeToIndex = _packTypeToIndex<0, QueryType, Args...>;

}  // namespace pack_detail

template <typename... Args>
struct Pack {
    // (d)
    static constexpr size_t size = pack_detail::packSize<Args...>;

    // (e)
    template <typename QueryType>
    static constexpr size_t index = pack_detail::packTypeToIndex<QueryType, Args...>;

    // (f)
    template <size_t QueryIndex>
        requires((QueryIndex >= 0) && (QueryIndex < size))
    using type = pack_detail::packIndexToType<QueryIndex, Args...>;

    // Trivial addition:
    template <typename QueryType>
    static constexpr bool contains = (index<QueryType> != pack_detail::limit);

    static constexpr size_t limit = pack_detail::limit;
};

#endif  // PACK_H
