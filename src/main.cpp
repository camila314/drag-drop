#include "DragDrop.hpp"

auto getZip = handleDragDrop({ "zip" }, [](auto path) {
	log::info("Dropped at {}", path);
}, [](auto) {
	log::info("Dragging file...");
});
