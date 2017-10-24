#pragma once

template < class ptr_type >
remote_ptr<ptr_type>::value_holder::value_holder(const value_holder& other): mAddr(other.mAddr)
{}

template < class ptr_type >
typename remote_ptr<ptr_type>::value_holder& remote_ptr<ptr_type>::value_holder::operator=(const ptr_type& v)
{
	if (!mpManager->write(mAddr, sizeof(ptr_type), (void*)(&v))) {
		throw write_error(mAddr, sizeof(ptr_type));
	}
	return *this;
}

template < class ptr_type >
remote_ptr<ptr_type>::value_holder::operator ptr_type()
{
	if (!mpManager->read(mAddr, sizeof(ptr_type), (void*)(&mData))) {
		throw read_error(mAddr, sizeof(ptr_type));
	}
	return mData;
}

template < class ptr_type >
ptr_type* remote_ptr<ptr_type>::value_holder::get_ptr()
{
	operator ptr_type();
	return &mData;
}

template < class ptr_type >
remote_ptr<ptr_type>::remote_ptr()
{
	mHolder.mAddr = 0;
}

template < class ptr_type >
remote_ptr<ptr_type>::remote_ptr(uintptr_t addr)
{
	mHolder.mAddr = addr;
}

template < class ptr_type >
remote_ptr<ptr_type>::remote_ptr(const remote_ptr<ptr_type>& other): mHolder(other.mHolder)
{}

template < class ptr_type >
template < class other_t >
remote_ptr<ptr_type>::remote_ptr(remote_ptr<other_t>& other)
{
	mHolder.mAddr = other.mHolder.mAddr;
}

template < class ptr_type >
remote_ptr<ptr_type> remote_ptr<ptr_type>::operator+(const int offset)
{
	remote_ptr<ptr_type> tmp(*this);
	tmp.mHolder.mAddr += offset;
	return tmp;
}

template < class ptr_type >
ptr_type* remote_ptr<ptr_type>::operator->()
{
	return mHolder.get_ptr();
}

template < class ptr_type >
typename remote_ptr<ptr_type>::value_holder& remote_ptr<ptr_type>::operator*()
{
	return mHolder;
}

template < class ptr_type >
uintptr_t remote_ptr<ptr_type>::addr() const
{
	return mHolder.mAddr;
}