#include <functional>

#include "RecommendationSystem.h"

int wmain()
{
	sp_rec_system rsys = std::make_shared<RecommendationSystem>();
	rank_map ranking(5, sp_movie_hash, sp_movie_equal);
	User user_1("NimM", ranking, rsys);

	user_1.add_movie_to_rs("Titanic", 1997, std::vector<double>(), 5);
	user_1.add_movie_to_rs("Wanted", 2008, std::vector<double>(), 5);
	user_1.add_movie_to_rs("Twilight", 2008, std::vector<double>(), 5);

	auto a = rsys->get_movie("Titanic", 1997);
	user_1.get_recommendation_by_content();

	return 0;
}