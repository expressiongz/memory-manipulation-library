#include "../main/mem_manip.hpp"

std::string mem_manip_lib::mem_read_string(const std::size_t string_sz) const 
{
    std::string read_string{};
    for (auto idx = 0u; idx < string_sz; idx++) 
    {
        read_string.push_back(static_cast<std::uint8_t*>(this->memory_address)[idx]);
    }
    return read_string;
}


std::vector<std::uint8_t> mem_manip_lib::mem_read_func_bytes() const 
{
	auto byte = static_cast< std::uint8_t* >( address );
	std::vector< std::uint8_t > function_bytes{ };

	static const std::unordered_map< std::uint8_t, std::uint8_t > ret_bytes_map
	{
		{
			0xC2,
			0x03
		},
		{
			0xC3,
			0x01
		}
	};

	static const auto alignment_bytes = std::to_array< std::uint8_t >
	( 
		{ 0xCC, 0x90 } 
	);

	while( true )
	{
		function_bytes.push_back( *byte );

		for( const auto& ret_byte : ret_bytes_map )
		{
			const auto& [ opcode, opcode_sz ] = ret_byte;
			if( *byte == opcode )
			{
				for( const auto& alignment_byte : alignment_bytes )
				{
					if( *( byte + opcode_sz ) == alignment_byte )
						return function_bytes;
				}
			}
			
		}
		++byte;
	}
}
