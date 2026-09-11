/*
 * msdosfs_kext.h - modern IOKit lifecycle wrapper for the msdosfs VFS plugin.
 *
 * Apple's own msdosfs.kext is a classic "kmod interface" kext: its
 * Xcode target sets MODULE_START/MODULE_STOP to msdosfs_module_start()/
 * msdosfs_module_stop() (see msdosfs_vfsops.c), and relies on Apple's
 * CreateKModInfo.perl build step to synthesize a kmod_info_t + _realmain/
 * _antimain trampoline that the kernel bootstrap looks up by symbol. Our
 * toolchain has no equivalent of that proprietary post-link step, so
 * instead we wrap the same start/stop logic in a tiny IOService subclass,
 * exactly like every other kext in opendarwin-extensions (IOPL031RTC,
 * OSSARMCPU): OSMetaClass-driven registration is all static C++
 * constructors + IOKitPersonalities matching, and needs no kmod_info at
 * all.
 */
#ifndef _MSDOSFS_KEXT_H
#define _MSDOSFS_KEXT_H

#include <IOKit/IOService.h>

class MsdosFS : public IOService
{
	OSDeclareDefaultStructors(MsdosFS);

public:
	virtual bool start(IOService *provider) APPLE_KEXT_OVERRIDE;
	virtual void stop(IOService *provider) APPLE_KEXT_OVERRIDE;
};

#endif /* _MSDOSFS_KEXT_H */
