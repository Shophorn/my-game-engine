#include "MatrixBase.hpp"

namespace ng
{

	/*
	Concrete type definitions.
	Following members need to be explicitly defined:
		identity
		determinant
	*/
	using f22 = maths::MatrixBase<float2, float2>;

	template<> constexpr f22 f22::identity() noexcept
	{ 
		return f22
		{ 
			float2 {1, 0},
			float2 {0, 1} 
		}; 
	}

	using f33 = maths::MatrixBase<float3, float3>;
	
	template<> constexpr f33 f33::identity() noexcept
	{ 
		return f33
		{ 
			float3 {1, 0, 0},
			float3 {0, 1, 0},
			float3 {0, 1, 0}
		}; 
	}


	using f44 = maths::MatrixBase<float4, float4>;
	
	template<> constexpr f44 f44::identity() noexcept
	{
		return f44
		{
			float4 {1, 0, 0, 0},
			float4 {0, 1, 0, 0},
			float4 {0, 0, 1, 0},
			float4 {0, 0, 0, 1}
		};
	}

	using f34 = maths::MatrixBase<float3, float4>;


	void testMatrices()
	{
		debug::log("f22::identity = {}", f22::identity());
		debug::log("f33::identity = {}", f33::identity());
		debug::log("f44::identity = {}", f44::identity());

		f33 M {
			float3 (11, 12, 13),
			float3 (21, 22, 23),
			float3 (31, 32, 33)
		};
		debug::log("M {}", M);
		debug::log("transpose of M {}", M.transpose());
		debug::log("transpose of transpose of M {}", M.transpose().transpose());

		debug::log("M {}", M);
		debug::log("determinant of M {}", M.determinant());

		M.setColumn(1, float3(0,0,0));
		M.setRow(1, float3(1, 1, 1));

		debug::log("M {}", M);
		debug::log("determinant of M {}", M.determinant());

		f22 M2 {
			float2 {1, 1.5}, float2 {-2.763, 1}
		};
		debug::log("M2 {}, determinant {}", M2, M2.determinant());
		debug::log("f22 identity {}, determinant {}", f22::identity(), f22::identity().determinant());

		f44 M4 {
			float4(2, 0, 0, 0),
			float4(0, 1, 0, 0),
			float4(0, 0, 1, 0),
			float4(0, 0, 0, 1),
		};
		debug::log("M4 {}, determinant {}", M4, M4.determinant());
		debug::log("f44 identity {}, determinant {}", f44::identity(), f44::identity().determinant());
		debug::log("f44 identity {}, determinant {}", f44::identity(), f44::identity().determinant());

		f44::columnGenerator<4> gen(0, 1, 2, 3);
		for (int i = 0; i < 12; i ++)
		{
			debug::log("{}", gen.next());				
		}

		return;
	}
}