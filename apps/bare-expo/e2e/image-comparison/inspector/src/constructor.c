#include <dispatch/dispatch.h>

// Forward declare the Swift function
extern void screen_inspector_dylib_init(void);

__attribute__((constructor))
static void customConstructor(int argc, const char **argv)
{
    // Defer Swift initialization off the dyld init thread. dyld runs constructors
    // synchronously and single-threaded, so any blocking work here (mkfifo, file
    // I/O, DispatchQueue creation) delays every subsequent dylib load and the
    // simulator's XCUITest runner attachment.
    dispatch_async(dispatch_get_global_queue(QOS_CLASS_USER_INITIATED, 0), ^{
        screen_inspector_dylib_init();
    });
}
