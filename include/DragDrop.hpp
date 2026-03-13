#include <Geode/loader/Event.hpp>
#include <filesystem>

enum class DragDropType : char {
    Drag,
    Drop,
    Cancel
};

class DragDropEvent : public geode::Event<DragDropEvent, void(std::filesystem::path), DragDropType> {
    using Event::Event;
};
