#include <functional>

#include "RecommendationSystem.h"

int wmain()
{
	sp_rec_system rsys = std::make_shared<RecommendationSystem>();
	rank_map ranking(5, sp_movie_hash, sp_movie_equal);
	User user_1("NimM", ranking, rsys);

	user_1.add_movie_to_rs("ForestGump", 1997, {1, 7, 7, 8}, 5);
	user_1.add_movie_to_rs("StarWars", 2008, {3, 3, 4, 9}, 6);
	user_1.add_movie_to_rs("Batman", 2008, {2, 6, 4, 8}, 1);
	rsys->add_movie("Titanic", 2008, {7, 2, 9, 1});
	//user_1.add_movie_to_rs("Twilight", 2008, std::vector<double>(), 5);

	std::cout << *rsys;

	std::cout << rsys->predict_movie_score(user_1, rsys->get_movie("Titanic", 2008), 2);

	return 0;
}