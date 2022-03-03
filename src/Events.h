#pragma once

using EventResult = RE::BSEventNotifyControl;
class EventHandler : public 
	RE::BSTEventSink<RE::TESHitEvent>,
	RE::BSTEventSink<RE::TESEquipEvent>
{
public:
	static EventHandler* GetSingleton();
	static void Register();

	virtual EventResult ProcessEvent(const RE::TESHitEvent* a_event, RE::BSTEventSource<RE::TESHitEvent>* a_eventSource) override;
	virtual EventResult ProcessEvent(const RE::TESEquipEvent* a_event, RE::BSTEventSource<RE::TESEquipEvent>* a_eventSource) override;

private:
	EventHandler() = default;
	EventHandler(const EventHandler&) = delete;
	EventHandler(EventHandler&&) = delete;
	virtual ~EventHandler() = default;

	EventHandler& operator=(const EventHandler&) = delete;
	EventHandler& operator=(EventHandler&&) = delete;
};
