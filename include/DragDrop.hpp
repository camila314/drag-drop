#include <Geode/Geode.hpp>
#include <filesystem>


#ifdef GEODE_IS_WINDOWS
#define C_DLL __declspec(dllexport)
#else
#define C_DLL
#endif


using namespace geode::prelude;
using namespace geode::event::v2;

enum class DragDropType {
    Drag,
    Drop,
    Cancel
};

class C_DLL DragDropEvent : public Event {
 protected:
    std::filesystem::path m_path;
    DragDropType m_type;
 public:
    DragDropEvent(std::filesystem::path const& path, DragDropType type);
    std::filesystem::path const& getPath() const;
    DragDropType getType() const;
};

EventHandler<DragDropEvent> C_DLL handleDragDrop(
    std::unordered_set<std::string> const& exts,
    std::function<void(std::filesystem::path const&)> drop,
    std::function<void(std::filesystem::path const&)> drag = {},
    std::function<void()> cancel = {}
);