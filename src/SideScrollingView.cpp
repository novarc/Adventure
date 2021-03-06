/*
 * SideScrollingView.cpp
 */

#include "stdinclude.hpp"
#include "GeometricPrimitives.hpp"
#include "Elements.hpp"
#include "PlatformInterface.hpp"
#include "EntityMap.hpp"
#include "PhysicsMap.hpp"
#include "SideScrollingView.hpp"

static inline xy calculateViewport(const xy &playerPosition, const xy &mapSize)
{
    xy v(0, 0);

    if(mapSize.x > sys->windowProperties.size.x)
    {
        int x_central_first = sys->windowProperties.size.x/2;
        int x_central_last = mapSize.x - x_central_first;

        if(playerPosition.x > x_central_last)
            v.x = mapSize.x - sys->windowProperties.size.x;
        else if(playerPosition.x > x_central_first)
            v.x = playerPosition.x - x_central_first;
    }

    if(mapSize.y > sys->windowProperties.size.y)
    {
        int y_central_first = sys->windowProperties.size.y/2;
        int y_central_last = mapSize.y - y_central_first;

        if(playerPosition.y > y_central_last)
            v.y = mapSize.y - sys->windowProperties.size.y;
        else if(playerPosition.y > y_central_first)
            v.y = playerPosition.y - y_central_first;
    }

    return v;
}

void SideScrollingView::render()
{
    if(physicsMap != nullptr) {
        if(player == nullptr) {
            // Invoke drawAt() naively on each entity:
            for(auto e : physicsMap->entityMap.getEntities())
                e->drawable->drawAt(e->pos);
        }
        else {
            xy viewport = calculateViewport(
                    player->pos + player->drawable->getSize() / 2,
                    physicsMap->entityMap.getMapSize());

            // Invoke drawAt(pos - viewport) on each entity:
            for(auto e : physicsMap->entityMap.getEntities())
                e->drawable->drawAt(e->pos - viewport);
        }
    }
}
