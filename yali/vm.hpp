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
	uint32_t* local_frame;
	address_type return_address;

	callframe(uint32_t* p_local_frame, uint64_t p_return_address) :
		local_frame{p_local_frame},
		return_address{address_type(p_return_address)}
	{}
};

class vm
{
	std::vector<callframe> frames;
	std::vector<uint32_t> _locals;
	uint32_t* _locals_top;

public:
	bool jump_flag = false;

	const callframe& current_callframe() const;
	uint32_t local_pop();
	void local_push(uint32_t value, uint8_t typeinfo = 0);
	uint32_t& local_ref_stack(size_t depth);
	uint32_t& local_ref_id(size_t id);
	uint32_t& local_top();

	vm();
	void run(const std::vector<bc::opcode>& program);
};
}

#endif // VM_HPP
