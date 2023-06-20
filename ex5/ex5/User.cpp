

// don't change those includes
#include "User.h"
#include "RecommendationSystem.h"


// implement your cpp code here
User::User(const std::string& username, rank_map user_ranks, up_rec_system rec_system)
{
}

const std::string& User::get_name() const
{
}

void User::add_movie_to_rs(const std::string& name, int year, const std::vector<double>& features, double rate)
{
}

rank_map User::get_ranks()
{
}

sp_movie User::get_recommendation_by_content() const
{
}

sp_movie User::get_recommendation_by_cf(int k) const
{
}

double User::get_prediction_score_for_movie(const std::string& name, int year, int k) const
{
}
