#include "IOPL031RTC.h"

#include <IOKit/IOLib.h>
#include <IOKit/IOService.h>
#include <IOKit/IOMemoryDescriptor.h>
#include <IOKit/IOTypes.h>
#include <IOKit/IOMapTypes.h>

#define super IOService
OSDefineMetaClassAndStructors(IOPL031RTC, IOService);

/*
 * PL031 base address and register layout on QEMU's `virt` machine
 * (see QEMU's hw/arm/virt.c: VIRT_RTC @ 0x0901_0000, size 0x1000).
 * RTCDR (offset 0x000) holds the current time as seconds since the POSIX
 * epoch.
 */
#define PL031_BASE 0x0901'0000ULL
#define PL031_SIZE 0x1000ULL
#define PL031_RTCDR 0x000

bool
IOPL031RTC::start(IOService * provider)
{
	kprintf("IOPL031RTC: start\n");
	if (!super::start(provider)) {
		kprintf("IOPL031RTC: super::start failed\n");
		return false;
	}

	_regDesc = IOMemoryDescriptor::withAddressRange(
		PL031_BASE, PL031_SIZE, kIODirectionInOut, /* task */ NULL);
	if (!_regDesc) {
		IOLog("IOPL031RTC: failed to create memory descriptor for PL031\n");
		return false;
	}

	_regMap = _regDesc->map(kIOMapInhibitCache);
	if (!_regMap) {
		IOLog("IOPL031RTC: failed to map PL031 registers\n");
		return false;
	}

	IOVirtualAddress regBase = _regMap->getVirtualAddress();
	uint32_t secs = *(volatile uint32_t *)(regBase + PL031_RTCDR);
	IOLog("IOPL031RTC: mapped PL031 @ 0x%llx, RTCDR=%u\n", PL031_BASE, secs);

	publishResource("IORTC", this);

	registerService();
	return true;
}

void
IOPL031RTC::stop(IOService * provider)
{
	if (_regMap) {
		_regMap->release();
		_regMap = NULL;
	}
	if (_regDesc) {
		_regDesc->release();
		_regDesc = NULL;
	}
	super::stop(provider);
}
