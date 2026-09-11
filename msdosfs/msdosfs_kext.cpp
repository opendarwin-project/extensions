/*
 * msdosfs_kext.cpp - modern IOKit lifecycle wrapper for the msdosfs VFS plugin.
 *
 * msdosfs_module_start()/msdosfs_module_stop() (msdosfs_vfsops.c) already
 * do exactly what we need - vfs_fsadd()/vfs_fsremove() the "msdos" VFS
 * table entry - and never actually use their (kmod_info_t *, void *)
 * arguments. We just call them from a plain IOService::start()/stop(),
 * so kext load/unload is driven by ordinary OSKext/IOKit machinery
 * instead of the legacy kmod_info start/stop ABI.
 */
#include "msdosfs_kext.h"

#include <mach/kmod.h>

extern "C" {
kern_return_t msdosfs_module_start(kmod_info_t *ki, void *data);
kern_return_t msdosfs_module_stop(kmod_info_t *ki, void *data);
}

OSDefineMetaClassAndStructors(MsdosFS, IOService)

bool
MsdosFS::start(IOService *provider)
{
	if (!IOService::start(provider)) {
		return false;
	}

	if (msdosfs_module_start(NULL, NULL) != KERN_SUCCESS) {
		IOService::stop(provider);
		return false;
	}

	registerService();
	return true;
}

void
MsdosFS::stop(IOService *provider)
{
	msdosfs_module_stop(NULL, NULL);
	IOService::stop(provider);
}
