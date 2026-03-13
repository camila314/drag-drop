#include "../include/DragDrop.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

#ifdef GEODE_IS_MACOS
#define CommentType CommentTypeDummy
#include <Cocoa/Cocoa.h>
#undef CommentType

#define mainThread(x) dispatch_async(dispatch_get_main_queue(), ^{ x; })

@implementation NSWindow (DragDrop)
- (NSDragOperation)draggingEntered:(id<NSDraggingInfo>)sender {
    NSPasteboard *pboard = [sender draggingPasteboard];
    NSArray *files = [pboard propertyListForType:NSFilenamesPboardType];
    
    if (files.count != 1)
        return NSDragOperationNone;

    NSString *filename = files[0];
    if (DragDropEvent(DragDropType::Drag).send(filename.UTF8String) == false) {
        return NSDragOperationNone;
    }

    return NSDragOperationCopy;
}

- (BOOL)performDragOperation:(id<NSDraggingInfo>)sender {
    NSPasteboard *pboard = [sender draggingPasteboard];
    NSArray *files = [pboard propertyListForType:NSFilenamesPboardType];
    NSString *filename = files[0];
    DragDropEvent(DragDropType::Drop).send(filename.UTF8String);
    return YES;
}

- (void)draggingExited:(id<NSDraggingInfo>)sender {
    NSPasteboard *pboard = [sender draggingPasteboard];
    NSArray *files = [pboard propertyListForType:NSFilenamesPboardType];
    NSString *filename = files[0];

    DragDropEvent(DragDropType::Cancel).send(filename.UTF8String);
}

@end

$execute {
    [NSApp.windows[0] registerForDraggedTypes:@[NSFilenamesPboardType]];
}
#endif