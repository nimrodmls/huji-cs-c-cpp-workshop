#include <numeric>

#include "RecommendationSystem.h"

RecommendationSystem::RecommendationSystem() :
	_movies(
		[](const sp_movie& movie1, const sp_movie& movie2)
		{
			return *movie1.get() < *movie2.get();
		})
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
	return movie;
}

sp_movie RecommendationSystem::get_movie(
	const std::string& name, int year) const
{
	const sp_movie temp_movie = std::make_shared<Movie>(name, year);
	const auto found_movie = _movies.find(temp_movie);
	if (found_movie == _movies.end())
	{
		return nullptr;
	}
	return found_movie->first;
}

sp_movie RecommendationSystem::recommend_by_content(const User& user)
{
	rank_map ranks = user.get_ranks();
	auto acc = std::accumulate(
		ranks.begin(), 
		ranks.end(), 
		0.0, 
		[](double total, const std::pair<sp_movie, double>& obj)
			{
				return total + obj.second;
			});
	return nullptr;
}

sp_movie RecommendationSystem::recommend_by_cf(const User& user, int movie_count)
{
	return nullptr;
}

double RecommendationSystem::predict_movie_score(const User& user, const sp_movie& movie, int movie_count)
{
	return 0;
}
