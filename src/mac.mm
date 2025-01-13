#include "../include/DragDrop.hpp"

#ifdef GEODE_IS_MACOS
#define CommentType CommentTypeDummy
#include <Cocoa/Cocoa.h>
#undef CommentType

@implementation NSWindow (DragDrop)
- (NSDragOperation)draggingEntered:(id<NSDraggingInfo>)sender {
    NSPasteboard *pboard = [sender draggingPasteboard];
    NSArray *files = [pboard propertyListForType:NSFilenamesPboardType];
    
    if (files.count != 1)
        return NSDragOperationNone;

    NSString *filename = files[0];
    if (DragDropEvent(filename.UTF8String, DragDropType::Drag).post() == ListenerResult::Propagate) {
        return NSDragOperationNone;
    }

    return NSDragOperationCopy;
}

- (BOOL)performDragOperation:(id<NSDraggingInfo>)sender {
    NSPasteboard *pboard = [sender draggingPasteboard];
    NSArray *files = [pboard propertyListForType:NSFilenamesPboardType];
    NSString *filename = files[0];
    DragDropEvent(filename.UTF8String, DragDropType::Drop).post();
    return YES;
}
@end

__attribute__((constructor)) void doThing2() {
    Loader::get()->queueInMainThread([]() {
        [NSApp.windows[0] registerForDraggedTypes:@[NSFilenamesPboardType]];
    });
}
#endif