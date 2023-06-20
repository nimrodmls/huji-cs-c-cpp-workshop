#include "RecommendationSystem.h"

RecommendationSystem::RecommendationSystem()
{
}

sp_movie RecommendationSystem::add_movie(
	const std::string& name, 
	int year, 
	const std::vector<double>& features)
{
	auto movie = std::make_shared<Movie>(name, year);
	movies[name + std::to_string(year)] = 
		std::make_pair(
			movie, features);
	return movie;
}

sp_movie RecommendationSystem::get_movie(
	const std::string& name, int year) const
{
	return movies.at(name + std::to_string(year)).first;
}

sp_movie RecommendationSystem::recommend_by_content(const User& user)
{
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
