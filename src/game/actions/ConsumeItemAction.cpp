#include "ConsumeItemAction.h"
#include <engine/core/ecs/Entity.h>
#include "game/Inventory.h"
#include "game/components/Character.h"
namespace actions
{
    ConsumeItemAction::ConsumeItemAction()
    {
    }
    ConsumeItemAction::~ConsumeItemAction()
    {
    }

    bool ConsumeItemAction::execute(std::shared_ptr<Item> &item, core::ecs::Entity &entity)
    {
        if (item->getType() != ItemType::FOOD)
            return false;

        auto hunger = std::stoi(item->getProperty("hunger"));
        auto thirst = std::stoi(item->getProperty("thirst"));

        auto &inventory = entity.findComponent<Inventory>();
        auto &character = entity.findComponent<Character>();
        character.getThirst().addValue(thirst);
        character.getHunger().addValue(hunger);

        inventory.removeItemById(item->getId(), 1);
        return true;
    }
} // namespace actions
