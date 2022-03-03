#include "Events.h"
#include <Injuries.h>

EventHandler* EventHandler::GetSingleton()
{
	static EventHandler singleton;
	return std::addressof(singleton);
}

void EventHandler::Register()
{
	auto scriptEventSourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
	scriptEventSourceHolder->GetEventSource<RE::TESHitEvent>()->AddEventSink(EventHandler::GetSingleton());
	scriptEventSourceHolder->GetEventSource<RE::TESEquipEvent>()->AddEventSink(EventHandler::GetSingleton());
}

EventResult EventHandler::ProcessEvent(const RE::TESHitEvent* a_event, RE::BSTEventSource<RE::TESHitEvent>*)
{
	if (a_event && a_event->cause.get() && a_event->target.get()) {
		auto victim = a_event->target.get()->As<RE::Actor>();
		auto aggressor = a_event->cause.get()->As<RE::Actor>();
		if (victim && aggressor)
			Injuries::on_attacked(victim, aggressor, a_event->flags);
	}
	return EventResult::kContinue;
}

EventResult EventHandler::ProcessEvent(const RE::TESEquipEvent* a_event, RE::BSTEventSource<RE::TESEquipEvent>*)
{
	if (a_event && a_event->actor.get()) {
		auto a = a_event->actor.get()->As<RE::Actor>();
		if (a)
			Injuries::on_equipped(a, a_event->equipped);
	}
	return EventResult::kContinue;
}
