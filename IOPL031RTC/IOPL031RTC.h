#ifndef _IOPL031RTC_H
#define _IOPL031RTC_H

#include <IOKit/IOService.h>
#include <IOKit/IOMemoryDescriptor.h>

class IOPL031RTC : public IOService
{
	OSDeclareDefaultStructors(IOPL031RTC);

public:
	virtual bool start(IOService * provider) APPLE_KEXT_OVERRIDE;
	virtual void stop(IOService * provider) APPLE_KEXT_OVERRIDE;

private:
	OSPtr<IOMemoryDescriptor> _regDesc;
	OSPtr<IOMemoryMap> _regMap;
};

#endif /* _IOPL031RTC_H */
