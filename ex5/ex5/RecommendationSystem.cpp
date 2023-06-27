#include <numeric>
#include <cmath>

#include "RecommendationSystem.h"

// See documentation at header file
RecommendationSystem::RecommendationSystem() :
	_movies(
		[](const sp_movie& movie1, const sp_movie& movie2)
		{
			return *movie1.get() < *movie2.get();
		}),
	_feature_count(0)
{}

// See documentation at header file
sp_movie RecommendationSystem::add_movie(
	const std::string& name, 
	int year, 
	const movie_features& features)
{
	auto movie = get_movie(name, year);
	if (nullptr != movie)
	{
		return movie;
	}

	movie = std::make_shared<Movie>(name, year);
	_movies[movie] = features;
	_feature_count = features.size();
	return movie;
}

// See documentation at header file
sp_movie RecommendationSystem::get_movie(
	const std::string& name, int year) const
{
	const sp_movie temp_movie = std::make_shared<Movie>(name, year);
	const auto found_movie = _movies.find(temp_movie);
	if (_movies.end() == found_movie)
	{
		return nullptr;
	}

	return found_movie->first;
}

// See documentation at header file
sp_movie RecommendationSystem::recommend_by_content(
	const User& user) const
{
	const up_rank_map normalized_ranks = 
		normalize_ranks(user.get_ranks());

	double rec_value = -1;
	sp_movie rec_movie = nullptr;

	for (const auto& movie : _movies)
	{
		// Only checking the movies which weren't watched by the user
		if (normalized_ranks->end() == 
			normalized_ranks->find(movie.first))
		{
			const double similarity = calculate_movie_similarity(
				calculate_preferences(
					*normalized_ranks.get()), movie.second);
			// Checking if the similarity is greater, if so, update
			// the recommendation
			if (similarity > rec_value)
			{
				rec_value = similarity;
				rec_movie = movie.first;
			}
		}
	}

	return rec_movie;
}

// See documentation at header file
sp_movie RecommendationSystem::recommend_by_cf(
	const User& user, int movie_count) const
{
	const rank_map& user_ranks = user.get_ranks();
	sp_movie current_movie = nullptr;
	double current_score = 0;

	for (const auto& movie : _movies)
	{
		// Only checking the movies which weren't watched by the user
		if (user_ranks.end() ==
			user_ranks.find(movie.first))
		{
			const double new_score = predict_movie_score(
				user, movie.first, movie_count);
			if (current_score < new_score)
			{
				current_score = new_score;
				current_movie = movie.first;
			}
		}
	}

	return current_movie;
}

// See documentation at header file
double RecommendationSystem::predict_movie_score(
	const User& user, 
	const sp_movie& movie, 
	int movie_count) const
{
	// Creating max heap to store all similarities within,
	// in the end we will take the k (movie_count) highest
	scores_max_heap highest_similarity(
				[](
					const movie_rank_pair& movie1, 
					const movie_rank_pair& movie2)
					{
						// Comparing the SCORES
						return movie1.second < movie2.second;
					});

	// Calculating the similarity of each movie the user watched
	// with the given movie
	const auto& user_ranks = user.get_ranks();
	for (const auto& user_movie : user_ranks)
	{
		double similarity = calculate_movie_similarity(
			_movies.at(user_movie.first), _movies.at(movie));

		highest_similarity.push(
			std::make_pair(user_movie.first, similarity));
	}

	// Now taking only the k-highest similarities, and calculating
	// the predicted score with the formula
	double similarity_sum = 0;
	double multiplicity_sum = 0;
	for (int index = 0; index < movie_count; index++)
	{
		const auto& current_max = highest_similarity.top();
		multiplicity_sum += 
			current_max.second * user_ranks.at(current_max.first);
		similarity_sum += current_max.second;
		// Removing the top element
		highest_similarity.pop();
	}

	return multiplicity_sum / similarity_sum;
}

// See documentation at header file
up_rank_map RecommendationSystem::normalize_ranks(const rank_map& user_ranks)
{
	double ranks_sum = 0;
	for (const auto& rank : user_ranks)
	{
		ranks_sum += rank.second;
	}
	// Calculating the average, as the sum has no use for us
	ranks_sum = ranks_sum / static_cast<double>(user_ranks.size());

	up_rank_map normalized = std::make_unique<rank_map>(user_ranks);

	for (auto& rank : *normalized)
	{
		rank.second -= ranks_sum;
	}

	return normalized;
}

// See documentation at header file
double RecommendationSystem::get_norm(const movie_features& vec)
{
	double quadratic_sum = 0;

	for (const auto& value : vec)
	{
		quadratic_sum += std::pow(value, 2);
	}

	return std::sqrt(quadratic_sum);
}

// See documentation at header file
double RecommendationSystem::dot_product(
	const movie_features& vec1, const movie_features& vec2)
{
	double product = 0;

	for (uint32_t index = 0; index < vec1.size(); index++)
	{
		product += vec1[index] * vec2[index];
	}

	return product;
}

// See documentation at header file
double RecommendationSystem::calculate_movie_similarity(
	const movie_features& preferences,
	const movie_features& features)
{
	double norm = get_norm(preferences) * get_norm(features);
	return dot_product(preferences, features) / norm;
}

// See documentation at header file
movie_features RecommendationSystem::calculate_preferences(
	const rank_map& normalized_ranks) const
{
	movie_features preferences(_feature_count);

	for (const auto& rank : normalized_ranks)
	{
		// fetching the features of the current movie in the ranks
		// and multiplying it by the normalized rank
		for (uint32_t index = 0; index < _feature_count; index++)
		{
			preferences[index] += 
				rank.second * _movies.at(rank.first)[index];
		}
	}

	return preferences;
}

// See documentation at header file
std::ostream& operator<<(
	std::ostream& os, const RecommendationSystem& rs)
{
	for (const auto& movie : rs._movies)
	{
		os << *movie.first;
	}

	return os;
}
