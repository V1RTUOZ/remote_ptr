#include "remote_ptr.h"
#include <sstream>
#include <iomanip>

IIOManager* value_holder_base::mpManager = nullptr;

void remote_ptr_base::set_io_manager(IIOManager* p)
{
	value_holder_base::mpManager = p;
}

char const* access_error::what() const
{
	std::stringstream ss;
	ss << "Can not ";
	if (mErrType == read_error)
		ss << "read ";
	else 
		ss << "write ";
	ss << mSize << " bytes, address = 0x" << std::hex << std::setfill('0') << std::setw(8) << mAddr;
	mText = ss.str();
	return mText.c_str();
}
