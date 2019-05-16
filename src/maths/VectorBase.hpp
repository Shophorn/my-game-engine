/*
NG engine project
Leo Tamminen

Base Vector class for all types and sizes.
This is my own road to learn about template metaprogramming
among other things.

This does not belong here, but it will help later when trying to connect classes to objects 
from serialized text files
https://stackoverflow.com/questions/1055452/c-get-name-of-type-in-template
*/

#pragma once

#include <type_traits>
#include <string>
#include <sstream>

namespace ng::maths
{
	/*
	Base Vector class for all vector types.

	Supported now are 1 - 4 dimensions.

	Derived classes must declare Dimension number of variables.
	They must be named 'x', 'y', 'z' and 'w' in that order.
	Also constructors must be declared in derived class :(

	eg VectorBase<int, 2, [derivedtypename]> must declare int x and int y;
	*/

	// :TODO: Add final, const etc modifiers
	template<typename ValueType, int Dimension, typename Derived>
	struct VectorBase
	{
		// using this_type 	= VectorBase<ValueType, Dimension, Derived>;
		using value_type 	= ValueType;
		using derived_type 	= Derived;

		static constexpr int dimension = Dimension;

		value_type & operator[] (int index)
		{
			// :TODO: do we need bounds checking here?
			// Is this trustworthy?
			// This assumes that there is no padding etc. between member fields, correct?
			return *(reinterpret_cast<value_type*>(this) + index);
		}	

		const value_type & operator [] (int index) const
		{
			return *(reinterpret_cast<const value_type*>(this) + index);
		}

		derived_type & operator += (derived_type other)
		{
			/*
			This is essentially 'this' pointer, but with desired derived type.
			Right?
			*/
			derived_type * thisAsDerived = static_cast<derived_type*> (this);
			
			/*
			These are compiled away as Dimension is compile-time constant
			Check for dimensions 1 and 2 too, although it wouldn't make sense
			to use vectors of 0 or 1 dimension.
			
			Should this be macroed?
			*/
			if constexpr (Dimension >= 1) 	thisAsDerived->x += other.x;
			if constexpr (Dimension >= 2)	thisAsDerived->y += other.y;
			if constexpr (Dimension >= 3)	thisAsDerived->z += other.z;
			if constexpr (Dimension >= 4) 	thisAsDerived->w += other.w;
			
			// :TODO: implement further dimensions as loop?

			return *thisAsDerived;
		}


		derived_type & operator -= (derived_type other)
		{
			// Look comments from 'operator +='

			derived_type * thisAsDerived = static_cast<derived_type*> (this);

			if constexpr (Dimension >= 1) 	thisAsDerived->x -= other.x;
			if constexpr (Dimension >= 2)	thisAsDerived->y -= other.y;
			if constexpr (Dimension >= 3)	thisAsDerived->z -= other.z;
			if constexpr (Dimension >= 4) 	thisAsDerived->w -= other.w;

			return *thisAsDerived;
		}

		derived_type operator + (derived_type other)
		{	
			// Copy this to derived type. 
			derived_type value = *static_cast<derived_type*> (this);
			return value += other;
		}

		derived_type operator - (derived_type other)
		{	
			// Copy 'this' to derived type. 
			derived_type value = *static_cast<derived_type*> (this);
			return value -= other;
		}

		derived_type & operator *= (value_type value)
		{
			// Look comments from 'operator +='

			derived_type * thisAsDerived = static_cast<derived_type*> (this);

			if constexpr (Dimension >= 1) 	thisAsDerived->x *= value;
			if constexpr (Dimension >= 2)	thisAsDerived->y *= value;
			if constexpr (Dimension >= 3)	thisAsDerived->z *= value;
			if constexpr (Dimension >= 4) 	thisAsDerived->w *= value;

			return *thisAsDerived;	
		}

		derived_type & operator /= (value_type value)
		{
			// Look comments from 'operator +='

			derived_type * thisAsDerived = static_cast<derived_type*>(this);

			if constexpr (Dimension >= 1)	thisAsDerived->x /= value;
			if constexpr (Dimension >= 2)	thisAsDerived->y /= value;
			if constexpr (Dimension >= 3)	thisAsDerived->z /= value;
			if constexpr (Dimension >= 4)	thisAsDerived->w /= value;
		
			return *thisAsDerived;
		}

		derived_type operator * (value_type value)
		{
			// Copy this to derived type. 
			derived_type vec = *static_cast<derived_type *>(this);
			return vec *= value;
		}

		derived_type operator / (value_type value)
		{
			// Copy this to derived type. 
			derived_type vec = *static_cast<derived_type *>(this);
			return vec /= value;
		}

		/*
		Stuff to do:
			scalar cast
			type cast
			size cast

			Assert is_basevector
		*/

		static value_type dot (derived_type a, derived_type b)
		{
			value_type product = 0;

			if constexpr (dimension >= 1) product += a.x * b.x;
			if constexpr (dimension >= 2) product += a.y * b.y;
			if constexpr (dimension >= 3) product += a.z * b.z;
			if constexpr (dimension >= 4) product += a.w * b.w;

			return product;
		}

		static value_type cross2d (derived_type lhs, derived_type rhs)
		{
			static_assert (dimension == 2, "2d cross product is only defined for vectors with 2 components");

			return lhs.x * rhs.y - lhs.y * rhs.x;
		}

		static derived_type cross (derived_type lhs, derived_type rhs)
		{
			static_assert(dimension == 3, "Cross product is only defined for vectors with 3 components");

			return derived_type
			(
				lhs.y * rhs.z - lhs.z * rhs.y,
				lhs.z * rhs.x - lhs.x * rhs.z,
				lhs.x * rhs.y - lhs.y * rhs.x
			);
		}

		/*
		Same as this[0].
		*/
		value_type * valuePtr()
		{
			return reinterpret_cast<value_type*>(this);
		}
	};
}

/*
Insert casts into global namespace

Cast derived vector class from one scalar type to another
*/
template<typename NewDerivedType, typename OldDerivedType>
auto type_cast(OldDerivedType a)
{
	static_assert(
		NewDerivedType::dimension == OldDerivedType::dimension,
		"Cannot type_cast vectors of different dimensions"
	);

	NewDerivedType b;

	if constexpr (OldDerivedType::dimension >= 1) b.x = static_cast<typename NewDerivedType::value_type> (a.x);
	if constexpr (OldDerivedType::dimension >= 2) b.y = static_cast<typename NewDerivedType::value_type> (a.y);
	if constexpr (OldDerivedType::dimension >= 3) b.z = static_cast<typename NewDerivedType::value_type> (a.z);
	if constexpr (OldDerivedType::dimension >= 4) b.x = static_cast<typename NewDerivedType::value_type> (a.x);

	return b;
}

/*
Cast derived vector to new derived type with different number of dimensions/components
*/
template<typename NewDerivedType, typename OldDerivedType>
auto dimension_cast(OldDerivedType a)
{
	static_assert(
		std::is_same<typename NewDerivedType::value_type, typename OldDerivedType::value_type>::value,
		"Cannot dimension_cast vectors of diffent value type"
	);

	/*
	Copy values from old type up to old dimensions. Use default values for new type
	when new has more dimensions.
	*/

	NewDerivedType b;

	if constexpr (NewDerivedType::dimension >= 1 && OldDerivedType::dimension >= 1) b.x = a.x;
	if constexpr (NewDerivedType::dimension >= 2 && OldDerivedType::dimension >= 2) b.y = a.y;
	if constexpr (NewDerivedType::dimension >= 3 && OldDerivedType::dimension >= 3) b.z = a.z;
	if constexpr (NewDerivedType::dimension >= 4 && OldDerivedType::dimension >= 4) b.w = a.w;


	return b;
}