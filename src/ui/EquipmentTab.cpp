#include "EquipmentTab.h"
#include "EquipSlot.h"
#include "game/components/Character.h"
#include "translate.h"
namespace UI
{

    EquipmentTab::EquipmentTab(Object *parent, core::ecs::Entity playerEntity) :
        UI::Tab(parent, _("Character")), m_playerEntity(playerEntity)
    {
        auto &character = m_playerEntity.findComponent<Character>();

        for (auto &slot: character.getEquipment())
        {
            auto itemSlot = std::make_shared<UI::EquipSlot>(this, slot, m_playerEntity);

            addObject(itemSlot);
        }
        constexpr int SIZE = 60;
        constexpr int BASE_OFFSET_X = 50;
        constexpr int BASE_OFFSET_Y = 10;

        objects[0]->setPos(BASE_OFFSET_X + SIZE, BASE_OFFSET_Y);
        objects[1]->setPos(BASE_OFFSET_X, BASE_OFFSET_Y + SIZE);
        objects[2]->setPos(BASE_OFFSET_X + (SIZE * 2), BASE_OFFSET_Y + SIZE);
        objects[3]->setPos(BASE_OFFSET_X + SIZE, BASE_OFFSET_Y + SIZE);
        objects[4]->setPos(BASE_OFFSET_X + SIZE, BASE_OFFSET_Y + (SIZE * 2));
        objects[5]->setPos(BASE_OFFSET_X + SIZE, BASE_OFFSET_Y + (SIZE * 3));
    }

    EquipmentTab::~EquipmentTab() {}
} // namespace UI
