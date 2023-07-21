// don't change those includes
#include "User.h"
#include "RecommendationSystem.h"

// See documentation at header file
User::User(const std::string& username, 
		   const rank_map& user_ranks,	
		   const sp_rec_system& rec_system) :
	_username(username),
	_user_ranking(user_ranks),
	_recommendation_system(rec_system)
{}

// See documentation at header file
const std::string& User::get_name() const
{
	return _username;
}

// See documentation at header file
const rank_map& User::get_ranks() const
{
	return _user_ranking;
}

// See documentation at header file
void User::add_movie_to_rs(const std::string& name, 
						   int year, 
						   const movie_features& features, 
						   double rate)
{
	const auto movie = 
		_recommendation_system->add_movie(name, year, features);
	_user_ranking[movie] = rate;
}

// See documentation at header file
sp_movie User::get_recommendation_by_content() const
{
	return _recommendation_system->recommend_by_content(*this);
}

// See documentation at header file
sp_movie User::get_recommendation_by_cf(int movie_count) const
{
	return _recommendation_system->recommend_by_cf(
		*this, movie_count);
}

// See documentation at header file
double User::get_prediction_score_for_movie(
	const std::string& name, int year, int movie_count) const
{
	const auto movie = 
		_recommendation_system->get_movie(name, year);
	return _recommendation_system->predict_movie_score(
		*this, movie, movie_count);
}

// See documentation at header file
std::ostream& operator<<(
	 std::ostream& os, const User& user)
{
	os << "name: " << user._username << std::endl;
	os << *user._recommendation_system;

	return os;
}
