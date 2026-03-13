#include "../include/DragDrop.hpp"

void test() {
    DragDropEvent(DragDropType::Drag).send("hello.txt");
}