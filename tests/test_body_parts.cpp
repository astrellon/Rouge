#include "test_body_parts.h"

#include <game/body_part.h>
#include <game/body_part_common.h>
#include <game/body_parts.h>
using namespace am::game;

#include <tests/asserts.h>

namespace am {
namespace tests {

	bool TestBodyParts::testSimple() {

		BodyParts parts;
		parts.addBodyPart(new BodyPart("left_hand", BodyPartType::HAND));
		assert(parts.getBodyPart("left_hand"));
		parts.getBodyPart("left_hand")->setWeaponPart(true);

		parts.addBodyPart(new BodyPart("right_hand", BodyPartType::HAND));
		assert(parts.getBodyPart("right_hand"));
		parts.getBodyPart("right_hand")->setWeaponPart(true);
		parts.getBodyPart("right_hand")->setCanHoldOnto(parts.getBodyPart("left_hand"));

		parts.addBodyPart(new BodyPart("torso", BodyPartType::TORSO));
		assert(parts.getBodyPart("torso"));

		parts.addBodyPart(new BodyPart("legs", BodyPartType::LEGS));
		assert(parts.getBodyPart("legs"));

		bool looped = false;
		base::Handle<BodyPart> attackPart(parts.getNextWeaponPart(looped));
		assert(attackPart.get());
		am_equals(false, looped);
		am_equalsStr("left_hand", attackPart->getName());

		attackPart = parts.getNextWeaponPart(looped);
		assert(attackPart.get());
		am_equals(false, looped);
		am_equalsStr("right_hand", attackPart->getName());

		attackPart = parts.getNextWeaponPart(looped);
		assert(attackPart.get());
		am_equals(true, looped);
		am_equalsStr("left_hand", attackPart->getName());

		BodyParts::PartList linked;
		assert(parts.getLinkedParts("left_hand", linked));
		am_equals(1u, linked.size());
		am_equalsStr("right_hand", linked[0]->getName());

		linked.clear();
		assert(parts.getLinkedParts("right_hand", linked));
		am_equals(0u, linked.size());
		
		return true;
	}

}
}
