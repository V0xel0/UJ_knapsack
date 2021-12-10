#include <vector>
#include <random>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <iostream>
#include <bitset>

#include "Utils.hpp"

constexpr s32 g_generation_size = 100;
constexpr s32 g_items_count = 100;
constexpr s8 g_max_value = 99;
constexpr s8 g_min_value = 10;
constexpr f32 g_knapsack_size_modifier = 0.66f;

[[nodiscard]]
inline b32 random_bool(const f32 probability, auto&& rng)
{
	const std::bernoulli_distribution distr(probability);
	return distr(rng);
}

[[nodiscard]]
inline s32 random_number(const s32 begin, const s32 end, auto&& rng)
{
	std::uniform_int_distribution<s32>distribution(begin, end);
	return distribution(rng);
}

int main()
{
	std::vector<s8>items;
	items.reserve(g_items_count);

	std::mt19937 rng{ std::random_device{}() };
	std::generate_n(std::back_inserter(items), g_items_count, [&rng] {
		return (s8)random_number(g_min_value, g_max_value, rng);
	});
	
	auto sum = std::accumulate(items.cbegin(), items.cend(), 0);
	auto knapsack_size = sum * g_knapsack_size_modifier;

	std::vector<std::bitset<g_items_count>>generation(g_items_count);
	// First initial population initalization
	for(auto&& chromosome : generation)
	{
		for(auto i = 0; i < g_items_count; ++i)
		{
			chromosome[i] = random_bool(0.5f, rng);
		}
	}

	std::vector<s16>fit_scores(g_items_count);

	auto sc = 0;
	for(auto&& chromosome : generation)
	{
		s16 score = 0;
		s16 total_weight = 0;

		for(auto i = 0; i < g_items_count; ++i)
		{
			if (chromosome[i] == true)
			{
				score += 1; // treat all items as having same value
				total_weight += items[i];
			}
		}

		if (total_weight > knapsack_size)
			score = 0;

		fit_scores[sc] = score;
		++sc;
	}

	for(auto&& el : fit_scores)
		std::cout << el << "\n";

}