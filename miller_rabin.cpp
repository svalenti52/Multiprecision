//
// Created by svale on 11/13/2021.
//

#include <iostream>
#include <random>
#include <boost/multiprecision/cpp_bin_float.hpp>
#include <boost/multiprecision/miller_rabin.hpp>

using u128 = boost::multiprecision::uint128_t;

int main(int argc, char** argv)
{
    using namespace boost::multiprecision;

    // let n be the number to be determined if prime
    // the only chance of being prime, (other than n = 2) is if n is odd
    // therefore, n can be represented as n = 2^r * s + 1, with s being odd
    // Since the problem asks specifically for n = 2^89-1, the inputs will be
    // 1. the power of two and 2. the integer to offset by (thus we will pass 89 -1 for the problem)
    //

    if (argc != 3) {std::cout << "usage: <program> <power of two> <offset>\n"; return 1;}
    const uint32_t power_of_two = strtoul(argv[1], nullptr, 10);
    const int32_t offset = strtol(argv[2], nullptr, 10);

    // Compute n from the inputs
    u128 n = 1;
    for (int ix = 1; ix <= power_of_two; ++ix)
        n *= 2;
    n += offset;

    if (bit_test(n, 0) == 0)
    {
        std::cout << "n is even, obviously not prime\n";
        return 0;
    }

	u128 powers_of_2_limit;  // r from n = 2^r * s + 1
	u128 odd_factor = 1;  // s from above equation
	u128 base = 5;  // not using randomly selected bases but a sequence for now

	const u128 n_minus_1 = n - 1;
	u128 q = n_minus_1;

    // Determining r and s, i.e.,
    // powers_of_2_limit (should be at least one since n-1 is even)
    // odd_factor
	for (powers_of_2_limit = 0; powers_of_2_limit < power_of_two; ++powers_of_2_limit) {
        if (bit_test(q, 0) == 0) {
            q /= 2;
            continue; // while divisible by 2
        } else {
            odd_factor = q;
            break;  // q has been reduced to the odd_factor, break out of the loop
        }
    }

    // Assume the number is composite unless we test all the bases
    // and they all pass in which case it is a strong pseudo prime
	bool composite = true;
    const u128 base_limit = 55;
	for (; base < base_limit; ++base)
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
			for (; ix < powers_of_2_limit; ++ix)
			{
				u128 second_test = powm(base, odd_factor*next_power_2, n);
				if (second_test == n_minus_1)
				{
					break; // found pseudo prime requirement so get out of inner loop
				}
                next_power_2 *= 2;
			}
            if (ix == powers_of_2_limit) { // this is key to detecting composite property
                // composite = true; // the inner loop did NOT find pseudo prime property
                break; // we break out of outer loop BEFORE base gets to 52 (in this case)
            }
		}
	}
    if (base == base_limit)  // traversed all the bases without detecting composite
        composite = false;  // so it is a strong pseudo prime with an excellent chance of being prime
		
	std::cout << powers_of_2_limit << "  " << odd_factor << "\n";
	std::cout << "composite = " << composite << '\n';
	
	std::default_random_engine dre;
	
	bool prime = miller_rabin_test<u128, std::default_random_engine>(n, 40, dre);
	std::cout << "boost miller_rabin says prime = " << prime << '\n';
}
