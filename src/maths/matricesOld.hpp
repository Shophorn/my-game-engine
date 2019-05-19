// #include "MatrixBase.hpp"

// namespace ng
// {

// 	/*
// 	Concrete type definitions.
// 	Following members need to be explicitly defined:
// 		identity
// 	*/

// 	using f22 = maths::MatrixBase<float2, float2>;

// 	template<> constexpr f22 f22::identity() noexcept
// 	{ 
// 		// static constexpr f22
// 		// {

// 		// }
// 		return f22
// 		{ 
// 			float2 {1, 0},
// 			float2 {0, 1} 
// 		}; 
// 	}

// 	using f33 = maths::MatrixBase<float3, float3>;
	
// 	template<> constexpr f33 f33::identity() noexcept
// 	{ 
// 		return f33
// 		{ 
// 			float3 {1, 0, 0},
// 			float3 {0, 1, 0},
// 			float3 {0, 1, 0}
// 		}; 
// 	}


// 	using f44 = maths::MatrixBase<float4, float4>;
	
// 	template<> constexpr f44 f44::identity() noexcept
// 	{
// 		return f44
// 		{
// 			float4 {1, 0, 0, 0},
// 			float4 {0, 1, 0, 0},
// 			float4 {0, 0, 1, 0},
// 			float4 {0, 0, 0, 1}
// 		};
// 	}

// 	using f34 = maths::MatrixBase<float3, float4>;


// 	struct f5 : maths::VectorBase<float, 5, f5>
// 	{
// 		float x {};
// 		float y {};
// 		float z {};
// 		float w {};
// 		float n {};

// 		constexpr f5 () = default;
// 		constexpr f5 (float x, float y, float z, float w, float n)
// 			: x(x), y(y), z(z), w(w), n(n) {}
// 	};


// 	using f55 = maths::MatrixBase<f5, f5>;

// 	template <> constexpr f55 f55::identity() noexcept
// 	{
// 		return f55
// 		{
// 			f5 {1, 0, 0, 0, 0},
// 			f5 {0, 1, 0, 0, 0},
// 			f5 {0, 0, 1, 0, 0},
// 			f5 {0, 0, 0, 1, 0},
// 			f5 {0, 0, 0, 0, 1}
// 		};
// 	}

// 	using f23 = maths::MatrixBase<float2, float3>;
// 	using f24 = maths::MatrixBase<float2, float4>;
// 	using f32 = maths::MatrixBase<float3, float2>;

// 	void testMatrices()
// 	{

// 		f32 A {
// 			float2(2, 2),
// 			float2(2, 2),
// 			float2(2, 2)
// 		};

// 		f24 B {
// 			float2(3, 2, 5, 2),
// 			float2(2, 4, 2, 6)
// 		};

// 		// auto product = A * B;
// 		// debug::log("f32 * f24 = {}", product);
// 		// debug::log("f32 * f32 = {}", B * A);



// 		f33 MM;
// 		debug::log("default initialized f33: {}", MM);


// 		f33 M {
// 			float3 (11, 12, 13),
// 			float3 (21, 22, 23),
// 			float3 (31, 32, 33)
// 		};

// 		f55 M5 = f55::identity();
// 		debug::log("f55 det {}", M5.determinant());

// 		M5 = f55 {

// 			f5 {1, 31, 0, 0, 0},
// 			f5 {0, 1, 0, 0, 0},
// 			f5 {0, 0.5, 1, 0, 0},
// 			f5 {0, 0.67, 0, 1, 0},
// 			f5 {0, -1.4, 0, 0, 1}
// 		};

// 		debug::log("f55 det {}", M5.determinant());
// 		return;

// 		// return;
// 		debug::log("f22::identity = {}", f22::identity());
// 		debug::log("f33::identity = {}", f33::identity());
// 		debug::log("f44::identity = {}", f44::identity());

// 		debug::log("M {}", M);
// 		debug::log("transpose of M {}", M.transpose());
// 		debug::log("transpose of transpose of M {}", M.transpose().transpose());

// 		debug::log("M {}", M);
// 		debug::log("determinant of M {}", M.determinant());

// 		M.setColumn(1, float3(0,0,0));
// 		M.setRow(1, float3(1, 1, 1));

// 		debug::log("M {}", M);
// 		debug::log("determinant of M {}", M.determinant());

// 		f22 M2 {
// 			float2 {1, 1.5}, float2 {-2.763, 1}
// 		};
// 		debug::log("M2 {}, determinant {}", M2, M2.determinant());
// 		debug::log("f22 identity {}, determinant {}", f22::identity(), f22::identity().determinant());

// 		f44 M4 {
// 			float4(2, 0, 0, 0),
// 			float4(0, 1, 0, 2.1),
// 			float4(0, 0, 2, 0),
// 			float4(-9, 1, 3, 0.2),
// 		};
// 		debug::log("M4 {}, determinant {}", M4, M4.determinant());
// 		debug::log("f44 identity {}, determinant {}", f44::identity(), f44::identity().determinant());
// 		debug::log("f44 identity {}, determinant {}", f44::identity(), f44::identity().determinant());

// 		return;
// 	}
	
// }