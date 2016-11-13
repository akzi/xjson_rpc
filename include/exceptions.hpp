#pragma once
namespace xjson_rpc
{
	class xjson_rpc_exception :std::exception
	{
	public:
		xjson_rpc_exception(const char *const error_str)
			:exception(error_str)
		{

		}
	};
	struct not_found_function: xjson_rpc_exception
	{
		not_found_function(const char *const error_str)
			:xjson_rpc_exception(error_str)
		{

		}
	};
}