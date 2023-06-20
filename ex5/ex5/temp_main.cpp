#include "RecommendationSystem.h"

int wmain()
{
	sp_rec_system rsys = std::make_shared<RecommendationSystem>();
	rank_map ranking(5, sp_movie_hash, sp_movie_equal);
	User user_1("NimM", ranking, rsys);

	user_1.add_movie_to_rs("Titanic", 1998, std::vector<double>(), 5);

	return 0;
}