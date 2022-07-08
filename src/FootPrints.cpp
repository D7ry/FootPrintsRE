#include "FootPrints.h"
#include "Utils.h"
void FootPrints::loadData() {
	logger::info("Loading data...");
	formLoader loader("Footprints.esp");
	loader.load(DefaultFootstepWalkR, 0x69bc);
	loader.load(DefaultFootstepWalkL, 0x69bd);
	loader.load(DefaultFootstepSprintR, 0x69be);
	loader.load(DefaultFootstepSprintL, 0x69bf);
	loader.load(DefaultFootstepSneakR, 0x69c0);
	loader.load(DefaultFootstepSneakL, 0x69c1);
	loader.printReport();
	logger::info("...done");
}


inline void FootPrints::getimpactDataSet(RE::BGSImpactDataSet*& a_impactDataSet, RE::Actor* a_actor, footPos a_footPos)
{
	switch (a_footPos) {
	case footPos::left:
		if (a_actor->IsSneaking()) {
			a_impactDataSet = DefaultFootstepSneakL;
		} else if (a_actor->IsSprinting()) {
			a_impactDataSet = DefaultFootstepSprintL;
		} else {
			a_impactDataSet = DefaultFootstepWalkL;
		}
		break;
	case footPos::right:
		if (a_actor->IsSneaking()) {
			a_impactDataSet = DefaultFootstepSneakR;
		} else if (a_actor->IsSprinting()) {
			a_impactDataSet = DefaultFootstepSprintR;
		} else {
			a_impactDataSet = DefaultFootstepWalkR;
		}
		break;
	}
}

inline bool isJumping(RE::Actor* a_actor)
{
	bool result = false;
	return a_actor->GetGraphVariableBool("bInJumpState", result) && result;
}

inline void FootPrints::getBodyNode(std::string& a_node, RE::Actor* a_actor, footPos a_footPos) {
	switch (a_footPos) {
	case footPos::left:
		if (isJumping(a_actor)) {
			a_node = "NPC L Thigh [LThg]";
		} else {
			a_node = "NPC L Calf [LClf]";
		}
		break;
	case footPos::right:
		if (isJumping(a_actor)) {
			a_node = "NPC R Thigh [RThg]";
		} else {
			a_node = "NPC R Calf [RClf]";
		}
		break;
	}
}

void FootPrints::playFootPrint(RE::Actor* a_actor, footPos a_foot)
{
	RE::NiPoint3 pos = RE::NiPoint3(0.0f, 0.0f, -1.0f);
	std::string node = "";
	RE::BGSImpactDataSet* impactData;
	getimpactDataSet(impactData, a_actor, a_foot);
	if (!impactData) {
		return;
	}
	getBodyNode(node, a_actor, a_foot);
	if (node == "") {
		return;
	}
	RE::BGSImpactManager::GetSingleton()->PlayImpactEffect(a_actor, impactData, node, pos, 128.f, false, false);
}

void FootPrints::printJump(RE::Actor* a_actor) {
	playFootPrint(a_actor, footPos::left);
	playFootPrint(a_actor, footPos::right);
}
