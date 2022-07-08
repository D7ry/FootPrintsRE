#pragma once
#include <memory>

class FootPrints
{
public:
	static FootPrints* GetSingleton() {
		static FootPrints singleton;
		return std::addressof(singleton);
	}

	enum class footPos
	{
		left,
		right,
		frontLeft,
		frontRight,
		backLeft,
		backRight
	};

	void loadData();

	void playFootPrint(RE::Actor* a_actor, footPos a_foot);

	void printJump(RE::Actor* a_actor);

private:

	RE::BGSImpactDataSet* DefaultFootstepWalkR;
	RE::BGSImpactDataSet* DefaultFootstepWalkL;

	RE::BGSImpactDataSet* DefaultFootstepSprintR;
	RE::BGSImpactDataSet* DefaultFootstepSprintL;

	RE::BGSImpactDataSet* DefaultFootstepSneakR;
	RE::BGSImpactDataSet* DefaultFootstepSneakL;

	inline void getimpactDataSet(RE::BGSImpactDataSet*& a_impactDataSet, RE::Actor* a_actor, footPos a_footPos);
	inline void getBodyNode(std::string& a_node, RE::Actor* a_actor, footPos a_footPos);
};

using namespace RE;
class Hook_playimpactEffect  
{
public:
	static void install()
	{
		auto& trampoline = SKSE::GetTrampoline();

		REL::Relocation<uintptr_t> hook{ REL::ID(55677) }; 

		_playImpactEffect = trampoline.write_call<5>(hook.address() + 0xb5, playImpactEffect);
		
		logger::info("Attack stamina hook installed.");
	}

private:
	//this, a_ref, a_impactEffect, a_nodeName
	static bool playImpactEffect(BGSImpactManager* a_manager, TESObjectREFR* a_ref, BGSImpactDataSet* a_impactEffect, const BSFixedString& a_nodeName, NiPoint3& a_pickDirection, float a_pickLength, bool a_applyNodeRotation, bool a_useNodeLocalRotation)
	{
		if (a_ref && a_ref->IsPlayerRef()) {
			logger::info("hooked playimpacteffect for {}", a_ref->GetName());
			logger::info("length: {}", a_pickLength);
			logger::info("coords: {}, {}, {}", a_pickDirection.x, a_pickDirection.y, a_pickDirection.z);
		}
		
		return _playImpactEffect(a_manager, a_ref, a_impactEffect, a_nodeName, a_pickDirection, a_pickLength, a_applyNodeRotation, a_useNodeLocalRotation);
	}
	static inline REL::Relocation<decltype(playImpactEffect)> _playImpactEffect;
};
