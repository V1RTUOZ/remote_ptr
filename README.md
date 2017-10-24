# remote_ptr
### remote_ptr pattern for external hacks


Pattern for help you access remote process virtual memory or file data using pointer-like syntax.

You just need to realize [IIOManager](https://github.com/V1RTUOZ/remote_ptr/blob/master/remote_ptr/iiomanager.h) interface (here is [example](https://github.com/V1RTUOZ/remote_ptr/blob/master/memory_manager.cpp)):

```cpp
class IIOManager {
public:
	virtual ~IIOManager() = default;
	virtual bool read(uintptr_t addr, size_t size, void* data) = 0;
	virtual bool write(uintptr_t addr, size_t size, void* data) = 0;
};
```

First you need to set instance of IIOManager:
```cpp
// static pointer to IIOManager inherited class instance for ALL remote_ptrs
remote_ptr_base::set_io_manager(pIIOManagerInstance);
```
```cpp
remote_ptr<int> remotePointer(address);
int value = *remotePointer; // read value
*remotePointer = value; // write value
```

On error it will throw [read_error](https://github.com/V1RTUOZ/remote_ptr/blob/81c9ad168b7c795dba3cd60f1fe86a5265f163f4/remote_ptr/remote_ptr.h#L66) or [write_error](https://github.com/V1RTUOZ/remote_ptr/blob/81c9ad168b7c795dba3cd60f1fe86a5265f163f4/remote_ptr/remote_ptr.h#L72). Both inherited from [access_error](https://github.com/V1RTUOZ/remote_ptr/blob/81c9ad168b7c795dba3cd60f1fe86a5265f163f4/remote_ptr/remote_ptr.h#L51).

In this repo you can find example application source (simple CS:GO radar hack).
Welcome to my [UC profile](https://www.unknowncheats.me/forum/members/669275.html).
