#include "../include/DragDrop.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;
using namespace geode::event::v2;

DragDropEvent::DragDropEvent(std::filesystem::path const& path, DragDropType type) : m_path(path), m_type(type) {}
std::filesystem::path const& DragDropEvent::getPath() const { return m_path; }
DragDropType DragDropEvent::getType() const { return m_type; }


EventHandler<DragDropEvent> handleDragDrop(
    std::unordered_set<std::string> const& exts,
    std::function<void(std::filesystem::path const&)> drop,
    std::function<void(std::filesystem::path const&)> drag,
    std::function<void()> cancel
) {
    auto handler = EventHandler<DragDropEvent>([=](auto event) {
        auto ext = event->getPath().extension().string();
        return ext.size() > 1 && exts.count(&ext[1]);
    });

    handler.bind([=](DragDropEvent* ev) {
        Loader::get()->queueInMainThread([=]() {
            if (ev->getType() == DragDropType::Drag && drag)
                drag(ev->getPath());
            else if (ev->getType() == DragDropType::Drop)
                drop(ev->getPath());
            else if (ev->getType() == DragDropType::Cancel && cancel)
                cancel();
        });

        return ListenerResult::Stop;
    });

    return handler;
}
