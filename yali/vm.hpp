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
};

class vm
{
	uint32_t* _locals_top;
	callframe* _frames_top;

	std::array<callframe, 256> _frames;
	std::array<uint32_t, 1024 * 1024 / 4> _locals;

public:
	bool jump_flag = false;

	const callframe& current_callframe() const;
	void frame_push(callframe frame);
	callframe::address_type frame_pop();

	uint32_t local_pop();
	void local_push(uint32_t value, uint8_t typeinfo = 0);
	uint32_t& local_ref_stack(size_t depth);
	uint32_t& local_ref_id(size_t id);
	uint32_t& local_top();

	template<size_t N>
	auto local_popn()
	{
		std::array<uint32_t, N> ret;

		for (size_t i = 0; i < N; ++i)
		{
			ret[i] = local_pop();
		}

		return ret;
	}

	vm();
	int run(const std::vector<bc::opcode>& program);
};
}

#endif // VM_HPP
