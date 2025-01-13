#include "DragDrop.hpp"

using namespace geode::prelude;

DragDropEvent::DragDropEvent(std::filesystem::path const& path, DragDropType type) : m_path(path), m_type(type) {}
std::filesystem::path const& DragDropEvent::getPath() const { return m_path; }
DragDropType DragDropEvent::getType() const { return m_type; }


EventHandler<DragDropEvent> handleDragDrop(
	std::unordered_set<std::string> const& exts,
	std::function<void(std::filesystem::path const&)> drop,
	std::function<void(std::filesystem::path const&)> drag
) {
    auto handler = EventHandler<DragDropEvent>([=](auto event) {
        auto ext = event->getPath().extension().string();
        return ext.size() > 1 && exts.count(&ext[1]);
    });

    handler.bind([=](DragDropEvent* ev) {
    	if (ev->getType() == DragDropType::Drag && drag)
    		drag(ev->getPath());
    	else if (ev->getType() == DragDropType::Drop)
    		drop(ev->getPath());

    	return ListenerResult::Stop;
    });

    return handler;
}
