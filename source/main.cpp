#include <vector>
#include <array>
#include <random>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <iostream>
#include <fstream>
#include <bitset>
#include <cassert>

#include "Utils.hpp"

constexpr s32 g_generation_size = 100;
constexpr s32 g_items_count = 100;
constexpr s16 g_max_value = 99;
constexpr s16 g_min_value = 10;
constexpr f32 g_knapsack_size_modifier = 0.66f;
constexpr size_t g_max_iterations = 1001;

using chromosome = std::bitset<g_items_count>;
using generation = std::vector<chromosome>;

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

struct Fit_score
{
	s16 fit;
	s16 chrom_pos;
};

bool operator<(const Fit_score &first, const Fit_score &sec)
{
    return first.fit < sec.fit;
}

bool operator>(const Fit_score &first, const Fit_score &sec)
{
    return first.fit > sec.fit;
}

int main()
{
	assert(sizeof(void*) == 8 && "Not x64!");

	std::vector<s8>items;
	items.reserve(g_items_count);

	std::mt19937 rng{ std::random_device{}() };
	std::generate_n(std::back_inserter(items), g_items_count, [&rng] {
		return (s8)random_number(g_min_value, g_max_value, rng);
	});
	
	auto sum = std::accumulate(items.cbegin(), items.cend(), 0);
	s16 knapsack_size = (s16)(sum * g_knapsack_size_modifier);

	generation prev_generation(g_items_count);
	generation new_generation(g_items_count);

	// First initial population initalization
	for(auto&& chromosome : prev_generation)
	{
		for(auto i = 0; i < g_items_count; ++i)
		{
			chromosome[i] = random_bool(0.5f, rng);
		}
	}

	std::vector<Fit_score>fit_scores(g_items_count);

	// Genetic Algorithm start
	for (size_t iterations = 0; iterations < g_max_iterations; iterations++)
	{
		// Calculate fitness
		s16 sc = 0;
		for(auto&& chromosome : prev_generation)
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

			fit_scores[sc].fit = score;
			fit_scores[sc].chrom_pos = sc;
			++sc;
		}

		// Ranking selection
		std::sort(fit_scores.begin(), fit_scores.end(), std::greater<Fit_score>());

		// Elitism - top 10%
		for (s16 i = 0; i < 10; i++)
		{
			new_generation[i] = prev_generation[ fit_scores[i].chrom_pos ];
		}
		
		// Crossover top 50%
		for (s16 i = 10; i < 100; i+=2)
		{
			s16 chrom_to_change = (s16)random_number(0, 50, rng);
			auto parent_1 = prev_generation[chrom_to_change];
			chrom_to_change = (s16)random_number(0, 50, rng);
			auto parent_2 = prev_generation[chrom_to_change];

			for (s16 chr = 0; chr < parent_1.size() / 2; chr++)
			{
				// chr and chr 2 is first and second "halves"
				auto chr2 = chr + (parent_1.size() / 2) - 1;
				// child 1
				new_generation[i][chr] = parent_1[chr];
				new_generation[i][chr2] = parent_2[chr2];
				// child 2
				new_generation[i+1][chr] = parent_1[chr2];
				new_generation[i+1][chr2] = parent_2[chr];
			}
			
		}

		// Mutation - 10%
		for(auto&& chromosome : new_generation)
		{
			s16 bit_to_flip = (s16)random_number(0, (s32)chromosome.size() - 1, rng);
			if (random_bool(0.1f, rng))
				chromosome.flip(bit_to_flip);
		}

		prev_generation.swap(new_generation);
	}

	// Solution gather
	s16 total_weight = 0;
	auto final_chromosome = new_generation[fit_scores[0].chrom_pos];
	std::array<s16, g_items_count>final_items{};
	s16 to_print = 0;

	for(auto i = 0; i < final_chromosome.size(); ++i)
	{
		if (final_chromosome[i] == true)
		{
			total_weight += items[i];
			final_items[to_print] = items[i];
			to_print++;
		}
	}

	// Simple redirect to file, can comment if not needed
	std::ofstream out("output.txt", std::ofstream::app);
    std::streambuf *coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(out.rdbuf());

	// Solution print - the solution is in new generation, even after swap cause mutations are only "handled" in next iterations (score = 0 if too big)
	std::cout << "\nKnapsack size: " << knapsack_size;
	std::cout << "\nBest fit score " << fit_scores[0].fit << "\nChromosome: " << fit_scores[0].chrom_pos << "  " << final_chromosome;
	std::cout <<"\nTotal weight: "   << total_weight;
	std::cout <<"\nItems in solution:\n";

	for(auto i = 0; i < to_print; ++i	)
	{
		if (i % 10 == 0 && i != 0)
			std::cout << "\n";

		std::cout << final_items[i] << " ";
	}
	
	std::cout << "\n\n";
}