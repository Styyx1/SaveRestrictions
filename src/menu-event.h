#pragma once

namespace MenuEventHandler {
	class OpenCloseManager : public REX::Singleton<OpenCloseManager>,
		public RE::BSTEventSink<RE::MenuOpenCloseEvent> 
	{
	public:
		static void Register();
		void SetDifficultyForSaving();		

	protected:
		RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent* event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) override;
		int _storedDifficulty;		
	};
}