#pragma once
#include <string>
#include <map>
#include <functional>

#include "xjson.hpp"
namespace xjson_rpc
{
	
	class dispatcher
	{
	public:
		typedef std::function<xjson::obj_t(const xjson::obj_t&)> decorate_function;

		dispatcher()
		{

		}

		template<typename FuncType>
		typename function_traits<FuncType>::stl_function_type
			to_stl_function(FuncType func)
		{
			return function_traits<FuncType>::stl_function_type{ func };
		}

		template<typename FuncType>
		void regist(std::string &&funcname, FuncType func)
		{
			add_stl_function(std::move(funcname), to_stl_function(func));
		}
		template<typename Ret, typename Class, typename ...Params>
		void regist(std::string &&funcname, Ret(Class::*func)(Params...), Class &inst)
		{
			add_stl_function(std::move(funcname), 
							 std::function<Ret(Params...)>{[&inst, func](Params&&...params)->Ret{
				return (inst.*func)(std::forward<Params>(params)...);
			}});
		}
	//private:
		template<typename Ret, typename ...Params>
		void add_stl_function(std::string &&funcname, std::function<Ret(Params...)> &&func)
		{
			return add_stl_function(std::move(funcname), std::move(func), make_index_sequence<sizeof ...(Params)>{});
		}

		template<typename ...Params>
		void add_stl_function(std::string &&funcname, std::function<void(Params...)> &&func)
		{
			std::function<xjson::obj_t(Params...)> tmp = [func](Params&&... params)->xjson::obj_t {
				func(std::forward<Params>(params)...);
				xjson::obj_t ret;
				return std::move(ret);
			};
			return add_stl_function(std::move(funcname), std::move(tmp),  make_index_sequence<sizeof ...(Params)>{});
		}

		template<typename Ret, typename ...Params, std::size_t ...Is>
		void add_stl_function(std::string &&funcname, std::function<Ret(Params...)> &&func, index_sequence<Is...> )
		{
			add_method(std::move(funcname), [func](const xjson::obj_t& vec) ->xjson::obj_t{
				Ret ret = func(vec.get<typename std::decay<Params>::type>(Is)...);
				xjson::obj_t o;
				o = std::move(ret);
				return std::move(o);
			});
		}

		void add_method(std::string &&funcname, decorate_function&&func)
		{
			auto ret = method_map_.emplace(std::piecewise_construct,
								std::forward_as_tuple(std::move (funcname)), 
								std::forward_as_tuple(std::move(func)));
			if(!ret.second)
			{
				//todo process error
			}
		}
		std::map<std::string, decorate_function> method_map_;
	};
}