#ifndef REGISTRY_H
#define REGISTRY_H
#include <stdio.h>
#include <stack>
#include <vector>
#include <queue>
#include <cassert>
#include <cstdint>


template <typename T>
class Registry {

    public:
    size_t store(T&& t) {

        if (mAvailableTags.empty()) { // t is gonna sit at the back of the vector
            mComponents.push_back((t)); 

            auto idx = mComponents.size() - 1;
            mTags.push_back(idx);
            mValidTags.push_back(1);
            return idx;

        } else {
            auto newTag = mAvailableTags.front();
            assert(!valid(newTag));
            mAvailableTags.pop();

            mValidTags[newTag] = 1;

            (*this)[newTag] = (t);
            return newTag;

        }

    }

        size_t store(const T& t) {

        if (mAvailableTags.empty()) { // t is gonna sit at the back of the vector
            mComponents.push_back((t)); 

            auto idx = mComponents.size() - 1;
            mTags.push_back(idx);
            mValidTags.push_back(1);
            return idx;

        } else {
            auto newTag = mAvailableTags.front();
            assert(!valid(newTag));
            mAvailableTags.pop();

            mValidTags[newTag] = 1;

            (*this)[newTag] = (t);
            return newTag;

        }

    }


    inline void remove(size_t t) {
        /*
        "Noooo you cant just inline everything! Your binary size will explode!!!"

        "Haha inline go brrrrr"
        */

        (*this)[t] = T{}; // calls destructor for OG value. assertion implicitly called.
        mValidTags[t] = false;
        mAvailableTags.push(t);






    }

    inline decltype(auto) operator[](size_t t) {
        // decl type to also account for rvalues that cannot be bound to lvalues like for a vector of booleans.
        assert(valid(t));
        return mComponents[mTags[t]];
    }

    inline decltype(auto) operator[](size_t t) const {
        return assert(valid(t));
        return mComponents[mTags[t]];
    }

    inline bool valid(size_t t) const {
        return t < size() ? mValidTags[t] : false;
    }

    inline size_t size() const {
        return mComponents.size();
    }

    private:

    std::vector<T> mComponents;
    std::vector<size_t> mTags; // maps an ID to its corresponding index in mComponents.
    std::vector<bool> mValidTags; // keeps track of all valid tags
    std::queue<size_t> mAvailableTags;

};



#endif  // REGISTRY_H
