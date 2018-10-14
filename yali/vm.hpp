#ifndef VM_HPP
#define VM_HPP

#include "bytecode.hpp"
#include <array>
#include <vector>

namespace yali
{
struct callframe
{
	using address_type = uint64_t;
	address_type local_frame_offset = 0;
	address_type return_address;

	callframe(uint64_t p_local_frame_offset, uint64_t p_return_address) :
		local_frame_offset{address_type(p_local_frame_offset)},
		return_address{address_type(p_return_address)}
	{}
};

struct vm
{
	std::vector<callframe> frames = {{0, 0}};
	std::vector<uint32_t> locals;
	bool jump_flag = false;

	const callframe& current_callframe() const;
	uint32_t local_pop();

	vm();
	void run(const std::vector<bc::opcode>& program);
};
}

#endif // VM_HPP
