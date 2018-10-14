#ifndef BYTECODE_HPP
#define BYTECODE_HPP

#include <array>
#include <cstdint>
#include <string_view>

namespace yali::bc
{
enum class system_function
{
	print_arg = 0x00000000,

	arith_add = 0x00000010,
	arith_sub = 0x00000011,
	arith_mul = 0x00000012,
	arith_div = 0x00000013,

	cmp_lt    = 0x00000020,
	cmp_lte   = 0x00000021,
	cmp_eq    = 0x00000022,
	cmp_neq   = 0x00000023,
	cmp_gt    = 0x00000024,
	cmp_gte   = 0x00000025,
};

enum class instruction : std::uint8_t
{
	local_push,
	local_clone,
	local_pop,
	local_swap,
	invoke_user,
	invoke_system,
	func_return,
	jump_cond,
};

struct opcode
{
	uint64_t value = {};

	template<class T> T read(size_t off) const
	{
		return static_cast<T>(value >> off);
	}

	uint8_t instruction() const
	{
		return read<uint8_t>(0);
	}

	template<class T> void write(T new_value, size_t off)
	{
		// Reset bits we are going to OR with the new_value
		value &= (~uint64_t{}) ^ (T{} << off);

		value |= new_value << off;
	}

	void write(uint64_t new_value, size_t off, size_t bytes)
	{
		// Welcome to hell!
		unsigned shift_amount = (64 - bytes * 8);
		auto mask = (~uint64_t{} << shift_amount) >> shift_amount;
		value &= (~uint64_t{}) ^ (mask << off);
		value |= new_value << off;
	}
};
}

namespace yali::bc::def
{
struct param_info
{
	//! Mnemonic for the parameter
	std::string_view name = "";

	//! Offset of the given parameter within the opcode
	size_t offset = {};

	//! Size, in bytes, of the parameter
	size_t bytes = {};
};

struct instruction_info
{
	std::string_view name;
	uint8_t id; // TODO: less garbage
	std::array<param_info, 4> params = {};
};

constexpr instruction_info
	local_push {
		"local_push",
		0,
		{{
			{"value", 8, 4},
			{"typeinfo", 40, 1}
		}}
	},

	local_clone {
		"local_clone",
		1,
		{{
			{"localid", 8, 4}
		}}
	},

	local_pop {
		"local_pop",
		2
	},

	local_swap {
		"local_swap",
		3
	},

	invoke_user {
		"invoke_user",
		4,
		{{
			{"funcip", 8, 4},
			{"paramframe", 40, 2}
		}}
	},

	invoke_system {
		"invoke_system",
		5,
		{{
			{"sysfuncid", 8, 4},
			{"paramframe", 40, 2}
		}}
	},

	func_return {
		"func_return",
		6
	},

	jump_cond {
		"jump_cond",
		7,
		{{
			{"targetip", 8, 4}
		}}
	};

constexpr std::array<instruction_info, 8> infos {
	local_push,
	local_clone,
	local_pop,
	local_swap,
	invoke_user,
	invoke_system,
	func_return,
	jump_cond
};

constexpr auto mk(instruction_info info, std::array<uint64_t, 4> array = {})
{
	opcode ret{info.id};

	for (size_t i = 0; i < info.params.size(); ++i)
	{
		if (info.params[i].name.empty())
		{
			break;
		}

		ret.write(array[i], info.params[i].offset, info.params[i].bytes);
	}

	return ret;
}
}

#endif // BYTECODE_HPP
