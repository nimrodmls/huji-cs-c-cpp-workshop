#include <numeric>
#include <cmath>

#include "RecommendationSystem.h"

RecommendationSystem::RecommendationSystem() :
	_movies(
		[](const sp_movie& movie1, const sp_movie& movie2)
		{
			return *movie1.get() < *movie2.get();
		}),
	_feature_count(0)
{}

sp_movie RecommendationSystem::add_movie(
	const std::string& name, 
	int year, 
	const std::vector<double>& features)
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

sp_movie RecommendationSystem::recommend_by_content(const User& user)
{
	up_rank_map normalized_ranks = 
		_normalize_ranks(user.get_ranks());

	double rec_value = 0;
	sp_movie rec_movie = nullptr;

	for (const auto& movie : _movies)
	{
		// Only checking the movies which weren't watched by the user
		if (normalized_ranks->end() == 
			normalized_ranks->find(movie.first))
		{
			double similarity = _calculate_movie_similarity(
				_calculate_preferences(
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

sp_movie RecommendationSystem::recommend_by_cf(const User& user, int movie_count)
{
	return nullptr;
}

double RecommendationSystem::predict_movie_score(
	const User& user, const sp_movie& movie, int movie_count)
{
	return 0;
}

up_rank_map RecommendationSystem::_normalize_ranks(const rank_map& user_ranks)
{
	double ranks_sum = 0;
	for (const auto& rank : user_ranks)
	{
		ranks_sum += rank.second;
	}
	// Calculating the average, as the sum has no use for us
	ranks_sum = ranks_sum / user_ranks.size();

	up_rank_map normalized = std::make_unique<rank_map>(user_ranks);

	for (auto& rank : *normalized)
	{
		rank.second -= ranks_sum;
	}

	return std::move(normalized);
}

double RecommendationSystem::_get_norm(const movie_features& vec)
{
	double quadratic_sum = 0;

	for (const auto& value : vec)
	{
		quadratic_sum += value;
	}

	return std::sqrt(quadratic_sum);
}

double RecommendationSystem::_dot_product(
	const movie_features& vec1, const movie_features& vec2)
{
	double product = 0;

	for (uint32_t index = 0; index < vec1.size(); index++)
	{
		product += vec1[index] * vec2[index];
	}

	return product;
}

movie_features RecommendationSystem::_calculate_preferences(
	const rank_map& normalized_ranks)
{
	movie_features preferences(_feature_count);

	for (auto& rank : normalized_ranks)
	{
		// fetching the features of the current movie in the ranks
		// and multiplying it by the normalized rank
		for (uint32_t index = 0; index < _feature_count; index++)
		{
			preferences[index] = 
				rank.second * _movies[rank.first][index];
		}
	}

	return preferences;
}

double RecommendationSystem::_calculate_movie_similarity(
	const movie_features& preferences, 
	const movie_features& movie_pref)
{
	double norm = _get_norm(preferences) * _get_norm(movie_pref);
	return _dot_product(preferences, movie_pref) / norm;
}

std::ostream& operator<<(std::ostream& os, const RecommendationSystem& rs)
{
	for (auto& movie : rs._movies)
	{
		os << *movie.first;
	}

	return os;
}
