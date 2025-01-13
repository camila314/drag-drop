#include <Geode/loader/Event.hpp>
#include <Geode/loader/EventV2.hpp>
#include <filesystem>

#ifdef DRAG_TL
    #ifdef GEODE_IS_MACOS
        #define __declspec(x) __attribute__((visibility("default")))
    #endif

    #define DRAG_DLL __declspec(dllexport)
#else
    #ifdef GEODE_IS_MACOS
        #define __declspec(x)
    #endif

    #define DRAG_DLL __declspec(dllimport)
#endif

enum class DragDropType {
    Drag,
    Drop,
    Cancel
};

class DRAG_DLL DragDropEvent : public geode::Event {
 protected:
    std::filesystem::path m_path;
    DragDropType m_type;
 public:
    DragDropEvent(std::filesystem::path const& path, DragDropType type);
    std::filesystem::path const& getPath() const;
    DragDropType getType() const;
};

geode::event::v2::EventHandler<DragDropEvent> DRAG_DLL handleDragDrop(
    std::unordered_set<std::string> const& exts,
    std::function<void(std::filesystem::path const&)> drop,
    std::function<void(std::filesystem::path const&)> drag = {},
    std::function<void()> cancel = {}
);