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



inline void FootPrints::getBodyNode(std::string& a_node, RE::Actor* a_actor, footPos a_footPos) {
	switch (a_footPos) {
	case footPos::left:
		a_node = "NPC L Calf [LClf]";
		break;
	case footPos::right:
		a_node = "NPC R Calf [RClf]";
		break;
	}
}

void FootPrints::playFootPrint(RE::Actor* a_actor, footPos a_foot)
{
	logger::debug("Playing footprint for actor {}", a_actor->GetName());
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

	RE::NiPoint3 pos = RE::NiPoint3(0.0f, 0.0f, -1.0f);
	RE::BGSImpactManager::GetSingleton()->PlayImpactEffect(a_actor, impactData, node, pos, 128.f, false, false);
}

void FootPrints::printJump(RE::Actor* a_actor) {
	RE::NiPoint3 pos = RE::NiPoint3(0.0f, 0.0f, -1.0f);
	std::string nodeL = "NPC L Thigh [LThg]";
	std::string nodeR = "NPC R Thigh [RThg]";
	RE::BGSImpactDataSet* impactDataL;
	RE::BGSImpactDataSet* impactDataR;
	getimpactDataSet(impactDataL, a_actor, footPos::left);
	getimpactDataSet(impactDataR, a_actor, footPos::right);
	if (!impactDataL || !impactDataR) {
		return;
	}
	auto ptr = RE::BGSImpactManager::GetSingleton();
	
	ptr->PlayImpactEffect(a_actor, impactDataL, nodeL, pos, 128.f, false, false);
	ptr->PlayImpactEffect(a_actor, impactDataR, nodeR, pos, 128.f, false, false);
}
