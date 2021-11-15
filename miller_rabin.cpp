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
	n += 17;
	const u128 n_minus_1 = n - 1;
	u128 q = n_minus_1;

	for (powers_of_2 = 0; powers_of_2 < 89; ++powers_of_2) {
        if (bit_test(q, 0) == 0) {
            q /= 2;
            continue;
        } else {
            odd_factor = q;
            break;
        }
    }
    if (powers_of_2 == 89) odd_factor = 1;

	bool composite = true;
	
	for (; base < 52; ++base)
	{
		u128 first_test = powm(base, odd_factor, n);
		if (first_test == 1 || first_test == n_minus_1)
		{
            // continue;  // passed first pseudo prime test, don't need second test
		}
		else
		{
            u128 next_power_2 = 2;
            int ix = 1;
			for (; ix < powers_of_2; ++ix)
			{
				u128 second_test = powm(base, odd_factor*next_power_2, n);
				if (second_test == n_minus_1)
				{
					break; // found pseudo prime requirement so get out of inner loop
				}
                next_power_2 *= 2;
			}
            if (ix == powers_of_2) { // this is key to detecting composite property
                // composite = true; // the inner loop did NOT find pseudo prime property
                break; // we break out of outer loop BEFORE base gets to 52 (in this case)
            }
		}
	}
    if (base == 52)
        composite = false;
		
	std::cout << powers_of_2 << "  " << odd_factor << "\n";
	std::cout << "composite = " << composite << '\n';
	
	std::default_random_engine dre;
	
	bool prime = miller_rabin_test<u128, std::default_random_engine>(n, 40, dre);
	std::cout << "boost miller_rabin says prime = " << prime << '\n';
}
