//
// Created by svale on 11/13/2021.
//

#include <iostream>
#include <random>
#include <boost/multiprecision/cpp_bin_float.hpp>
#include <boost/multiprecision/miller_rabin.hpp>

using u128 = boost::multiprecision::uint128_t;

int main()
{
	using namespace boost::multiprecision;
	u128 powers_of_2;
	u128 odd_factor = 0;
	// u128 n_minus_1 = 0;
	u128 base = 5;

	u128 n = 1;
	for (int ix = 1; ix <=89; ++ix)
		n *= 2;
	n -= 1;
	const u128 n_minus_1 = n - 1;
	u128 q = n_minus_1;

	for (powers_of_2 = 0; powers_of_2 < 89; ++powers_of_2)
		if (bit_test(q, 0) == 0)
		{
			q /= 2;
			continue;
		}
		else
		{
			odd_factor = q;
			break;
		}

	// n_minus_1 = powm(x, y);
	
	bool composite = true;
	
	for (; base < 52; ++base)
	{
		u128 first_test = powm(base, odd_factor, n);
		if (first_test == 1 || first_test == n_minus_1)
		{
			composite = false;
			break;
		}
		else
		{
			for (int ix = 0; ix < powers_of_2; ++ix)
			{
				u128 second_test = powm(first_test, 2, n);
				if (second_test == n_minus_1)
				{
					composite = false;
					break;
				}
			}
			if (composite)
				break;
		}
	}
		
	std::cout << powers_of_2 << "  " << odd_factor << '\n';
	std::cout << "composite = " << composite << '\n';
	
	std::default_random_engine dre;
	
	bool prime = miller_rabin_test<u128, std::default_random_engine>(n, 40, dre);
	std::cout << "boost miller_rabin says prime = " << prime << '\n';
}
