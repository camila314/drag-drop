# Drag/Drop API

Geode API mod that implements dragging and dropping. Here is an example:

```cpp
auto txtHandler = handleDragDrop({ "txt" }, [](auto path) {
    // When file is dropped
    auto str = file::readString(path).unwrapOrDefault();
    FLAlertLayer::create("Dragged Text", str, "Ok")->show();
}, [](auto) { // optional arg
    log::info("Initiated drag event");
}, []() { // optional arg
    log::info("Left drag event");
});
```

Requires nightly to build, but does not require nightly to work.