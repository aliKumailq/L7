#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <cstddef>  // size_t

#include "pack.h"  // Pack from prep

#include "registry.h"

#include <limits>
#include <array>
#include <tuple>
#include <cassert>

using EntityID = size_t;

template <typename... ComponentTypes>
class EntityManager {

    public:


    EntityID createEntity() {
        std::array<size_t,pack_detail::packSize<ComponentTypes...>> indicies;
        indicies.fill(pack_detail::limit);
        return mEntities.store(indicies);
        
    }

    bool validEntity(EntityID id) {
        return mEntities.valid(id);
    }

    template <typename ComponentType>
    bool hasComponent(EntityID id) {
        assert(validEntity(id));
        static_assert(contains<ComponentType>);
        return mEntities[id][idx<ComponentType>] != pack_detail::limit;
    }

    template <typename ComponentType>
    decltype(auto) getComponent(EntityID id) {
        assert(hasComponent<ComponentType>(id));
        static_assert(contains<ComponentType>);

        constexpr auto index = idx<ComponentType>;
        return std::get<index>(mRegistries)[mEntities[id][index]];
    }

    template <typename ComponentType> 
    void addComponent(EntityID id, ComponentType&& component) {
        assert(validEntity(id));
        static_assert(contains<ComponentType>);
        constexpr auto index = idx<ComponentType>;
        const auto ticket = std::get<index>(mRegistries).store(component);
        mEntities[id][index] = ticket;
    }






    private:
    std::tuple<Registry<ComponentTypes>...> mRegistries;
    Registry<std::array<size_t,pack_detail::packSize<ComponentTypes...>>> mEntities; // 
    struct Pack<ComponentTypes...> mPack;

    template <typename Component>
    static constexpr bool contains = Pack<ComponentTypes...>::template contains<Component>;

    template <typename Component>
    static constexpr size_t idx = Pack<ComponentTypes...>::template index<Component>;

    template<size_t t>
    using type = Pack<ComponentTypes...>::template type<t>;


};

#endif  // ENTITY_MANAGER_H
