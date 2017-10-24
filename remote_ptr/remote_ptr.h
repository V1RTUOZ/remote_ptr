#pragma once
#include "iiomanager.h"
#include <stdexcept>

class value_holder_base {
	friend class remote_ptr_base;
protected:
	static IIOManager* mpManager;
};

class remote_ptr_base {
public:
	static void set_io_manager(IIOManager* p);
};


template < class ptr_type >
class remote_ptr : public remote_ptr_base {
	class value_holder : public value_holder_base {
		friend class remote_ptr;

		ptr_type mData;
		uintptr_t mAddr;

		value_holder() = default;

	public:
		value_holder(const value_holder& other);
		value_holder& operator=(const ptr_type& v);
		operator ptr_type();
		ptr_type* get_ptr();
		~value_holder() = default;
	};

	value_holder mHolder;

public:	
	remote_ptr();
	remote_ptr(uintptr_t addr);
	remote_ptr(const remote_ptr<ptr_type>& other);
	uintptr_t addr() const;
	template < class other_t >
	explicit remote_ptr(remote_ptr<other_t>& other);
	remote_ptr<ptr_type> operator+(const int offset);
	ptr_type* operator->();
	value_holder& operator*();
	virtual ~remote_ptr() = default;
};


class access_error : public std::runtime_error {
public:
	typedef enum {
		read_error,
		write_error
	} error_type_e;
	error_type_e mErrType;
	size_t mSize;
	uintptr_t mAddr;
	mutable std::string mText;
	typedef std::runtime_error _base;
	access_error(uintptr_t addr, size_t size, error_type_e type) : _base("remote_ptr access error"), mErrType(type), mSize(size), mAddr(addr) {}
	virtual char const* what() const override;
	virtual ~access_error() = default;
};
class read_error : public access_error {
public:
	typedef access_error _base;
	read_error(uintptr_t addr, size_t size) : _base(addr, size, access_error::read_error) {}
	virtual ~read_error() = default;
};
class write_error : public access_error {
public:
	typedef access_error _base;
	write_error(uintptr_t addr, size_t size) : _base(addr, size, access_error::write_error) {}
	virtual ~write_error() = default;
};

#include "remote_ptr_tpldef.h"