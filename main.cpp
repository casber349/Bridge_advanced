#define RED	 31
#define GREEN 32
#define BLUE 34
#define MAGENTA 35
#define CYAN 36

#include <iostream>
#include <vector>
#include <set>
#include <string>

using namespace std;

// Function to set text color using ASCII screen control sequence
void setTextColor(int color) {
	cout << "\033[" << to_string(color) << "m";
}

// Function to reset text color to default
void resetTextColor() {
	cout << "\033[0m";
}

int win(int trick_required, int trick, int suit, int level, bool vulnerable, bool Doubled, bool Redoubled) {
	int score = 0;
	if (!Doubled) {
		if (!Redoubled) {
			switch (suit) {	//Basic score
				case 1: case 2:		//minor(C, D)
					score = level * 20;
					break;
				case 3: case 4:		//Major(H, S)
					score = level * 30;
					break;
				case 5:					//NT
					score = level * 30 + 10;
					break;
			}

			if (score >= 100) {	//Game
				if (vulnerable) {
					score += 500;
				}
				else {
					score += 300;
				}
			}
			else {
				score += 50;	//Partscore Bonus
			}

			if (level == 6) {		//Slam
				if (vulnerable) {
					score += 750;
				}
				else {
					score += 500;
				}
			}
			if (level == 7) {		//Grand Slam
				if (vulnerable) {
					score += 1500;
				}
				else {
					score += 1000;
				}
			}

			switch (suit) {	//Overtrick score
				case 1: case 2:		//minor(C, D)
					score += (trick - trick_required) * 20;
					break;
				case 3: case 4: case 5:		//Major(H, S), NT
					score += (trick - trick_required) * 30;
					break;
			}
		}
		else {	//Redoubled
			switch (suit) {	//Basic score
				case 1: case 2:		//minor(C, D)
					score = level * 20;
					break;
				case 3: case 4:		//Major(H, S)
					score = level * 30;
					break;
				case 5:					//NT
					score = level * 30 + 10;
					break;
			}

			score *= 4;	//Redouble: Basic score * 4

			if (score >= 100) {	//Game
				if (vulnerable) {
					score += 500;
				}
				else {
					score += 300;
				}
			}
			else {
				score += 50;	//Partscore Bonus
			}

			if (level == 6) {		//Slam
				if (vulnerable) {
					score += 750;
				}
				else {
					score += 500;
				}
			}
			if (level == 7) {		//Grand Slam
				if (vulnerable) {
					score += 1500;
				}
				else {
					score += 1000;
				}
			}

			score += 100;	//Redouble Bonus

			if (vulnerable) {	//Overtrick Bonus (Redouble)
				score += (trick - trick_required) * 400;
			}
			else {
				score += (trick - trick_required) * 200;
			}
		}
	}
	else {	//Doubled
		switch (suit) {	//Basic score
			case 1: case 2:		//minor(C, D)
				score = level * 20;
				break;
			case 3: case 4:		//Major(H, S)
				score = level * 30;
				break;
			case 5:					//NT
				score = level * 30 + 10;
				break;
		}

		score *= 2;	//Double: Basic score * 2

		if (score >= 100) {	//Game
			if (vulnerable) {
				score += 500;
			}
			else {
				score += 300;
			}
		}
		else {
			score += 50;	//Partscore Bonus
		}

		if (level == 6) {		//Slam
			if (vulnerable) {
				score += 750;
			}
			else {
				score += 500;
			}
		}
		if (level == 7) {		//Grand Slam
			if (vulnerable) {
				score += 1500;
			}
			else {
				score += 1000;
			}
		}

		score += 50;	//Double Bonus

		if (vulnerable) {	//Overtrick Bonus (Double)
			score += (trick - trick_required) * 200;
		}
		else {
			score += (trick - trick_required) * 100;
		}
	}
	return score;
}

int lose(int trick_required, int trick, bool vulnerable, bool Doubled, bool Redoubled) {
	int score = 0;
	if (!Doubled) {
		if (!Redoubled) {
			if (vulnerable) {
				score = (trick_required - trick) * (-100);
			}
			else {	//non_vulnerable
				score = (trick_required - trick) * (-50);
			}
		}
		else {	//Redoubled
			if (vulnerable) {	//(Redouble)
				if ((trick_required - trick) == 1) {	//down 1 trick: -400
					score = -400;
				}
				else {	//down 2 trick or more : -1000, -1600
					score = -400;
					score -= ((trick_required - trick) - 1) * 600;
				}
			}
			else {	//non_vulnerable (Redouble)
				if ((trick_required - trick) == 1) {	//down 1 trick: -200
					score = -200;
				}
				else if (((trick_required - trick) == 2) || ((trick_required - trick) == 3)) {	//down 2~ 3 trick: -600, -1000
					score = -200;
					score -= ((trick_required - trick) - 1) * 400;
				}
				else {	//down 4 trick or more : -1600, -2200
					score = -1000;
					score -= ((trick_required - trick) - 3) * 600;
				}
			}
		}
	}
	else {	//Doubled
		if (vulnerable) {	//(Double)
			if ((trick_required - trick) == 1) {	//down 1 trick: -200
				score = -200;
			}
			else {	//down 2 trick or more : -500, -800
				score = -200;
				score -= ((trick_required - trick) - 1) * 300;
			}
		}
		else {	//non_vulnerable (Double)
			if ((trick_required - trick) == 1) {	//down 1 trick: -100
				score = -100;
			}
			else if (((trick_required - trick) == 2) || ((trick_required - trick) == 3)) {	//down 2~ 3 trick: -300, -500
				score = -100;
				score -= ((trick_required - trick) - 1) * 200;
			}
			else {	//down 4 trick or more : -800, -1100
				score = -500;
				score -= ((trick_required - trick) - 3) * 300;
			}
		}
	}

	return score;
}

enum suit {
	club = 1,
	diamond,
	heart,
	spade,
	no_trump
};

enum rank {
	jack = 11,
	queen,
	king,
	ace
};

class card {
	public:
		card(int r) : suit(r / 13 + 1), rank(r % 13 + 2) {}
		int suit;
		int rank;
		bool is_trump = false;
};

class bid {
	public:
		bid(int id_temp): id(id_temp), level(id_temp / 5 + 1), suit(id_temp % 5 + 1){}
		int id;
		const int level;
		const int suit;
		bool available = true;
};

class player {
	public:
		player(string player_name, vector<int> player_hand, vector<int> player_bid) : name(player_name) {
			for (int j = 0; j < player_hand.size(); j++) {
				card temp(player_hand[j]);
				hand.push_back(temp);
			}
			for (int j = 0; j < player_bid.size(); j++) {
				bid temp(player_bid[j]);
				bids.push_back(temp);
			}
		}

		const string name;
		vector <card> hand;
		vector <bid> bids;

		bool can_double = false;
		bool can_redouble = false;

		bool club_bidden = false;
		bool diamond_bidden = false;
		bool heart_bidden = false;
		bool spade_bidden = false;
		bool no_trump_bidden = false;

		int count_of_each_suit[4] = { 0, 0, 0, 0 };	// C, D, H, S

		void print_name() {
			cout << name;
			return;
		}

		void print_cards() {
			for (int j = 0; j < hand.size(); j++) {
				switch (hand[j].suit) {
					case club:
						setTextColor(GREEN);
						cout << "C";
						break;
					case diamond:
						setTextColor(MAGENTA);
						cout << "D";
						break;
					case heart:
						setTextColor(RED);
						cout << "H";
						break;
					case spade:
						setTextColor(BLUE);
						cout << "S";
						break;
				}
				switch (hand[j].rank) {
					case jack:
						cout << "J";
						break;
					case queen:
						cout << "Q";
						break;
					case king:
						cout << "K";
						break;
					case ace:
						cout << "A";
						break;
					default:
						cout << hand[j].rank;;
						break;
				}
				resetTextColor();
				cout << " ";
			}
			cout << endl;
			return;
		}

		void print_bids() {
			for (int j = 0; j < bids.size(); j++) {
				if (bids[j].available) {
					switch (bids[j].suit) {
						case club:
							setTextColor(GREEN);
							cout << bids[j].level;
							cout << "C";
							break;
						case diamond:
							setTextColor(MAGENTA);
							cout << bids[j].level;
							cout << "D";
							break;
						case heart:
							setTextColor(RED);
							cout << bids[j].level;
							cout << "H";
							break;
						case spade:
							setTextColor(BLUE);
							cout << bids[j].level;
							cout << "S";
							break;
						case no_trump:
							resetTextColor();
							cout << bids[j].level;
							cout << "NT";
							break;
					}
				}
				else {
					cout << "  ";
				}
				resetTextColor();
				if ((j + 1) % 5 == 0) {
					cout << endl;
				}
				else {
					cout << " ";
				}
			}
			cout << "Pass ";
			if (can_double) {
				setTextColor(CYAN);
				cout << "Double" << endl;
				resetTextColor();
			}
			if (can_redouble) {
				setTextColor(CYAN);
				cout << "Redouble" << endl;
				resetTextColor();
			}

			return;
		}

		void disable_bids(int highest_bid_id) {
			for (int j = 0; j < bids.size(); j++) {
				if (bids[j].id <= highest_bid_id) {
					bids[j].available = false;
				}
			}

			return;
		}

		void print_contract(int id, bool doubled, bool redoubled) {
			cout << endl << "Contract: ";
			
			switch (id % 5 + 1) {
				case club:
					setTextColor(GREEN);
					cout << id / 5 + 1;
					cout << "C";
					break;
				case diamond:
					setTextColor(MAGENTA);
					cout << id / 5 + 1;
					cout << "D";
					break;
				case heart:
					setTextColor(RED);
					cout << id / 5 + 1;
					cout << "H";
					break;
				case spade:
					setTextColor(BLUE);
					cout << id / 5 + 1;
					cout << "S";
					break;
				case no_trump:
					resetTextColor();
					cout << id / 5 + 1;
					cout << "NT";
					break;
			}
			if (doubled) {
				setTextColor(CYAN);
				cout << " Doubled";
			}
			else if (redoubled) {
				setTextColor(CYAN);
				cout << " Redoubled";
			}
			resetTextColor();
			cout << ", " << "Declarer: " << name << endl;
			cout << endl;
			return;
		}

		void promote_trump(int trump) {
			for (int j = 0; j < hand.size(); j++) {
				if (hand[j].suit == trump) {
					hand[j].is_trump = true;
				}
			}
		}

		void how_many_cards_in_each_suit() {
			for (int j = 0; j < hand.size(); j++) {
				count_of_each_suit[hand[j].suit - 1]++;
			}
			return;
		}
};

int main() {	// in this project, we use "player" as main objects
	initializing:
	system("cls");
	srand(time(NULL));
	vector <bool> used(52, false);

	vector <vector<int>> player_card_id;
	set<int> card_id_temp_set;
	vector<int> card_id_temp_vector;
	while (1) {
		int r = rand() % 52;
		if (player_card_id.size() == 4) {
			break;
		}
		if (card_id_temp_set.size() == 13) {
			auto it = card_id_temp_set.begin();
			for (it; it != card_id_temp_set.end(); it++) {
				card_id_temp_vector.push_back(*it);
			}
			player_card_id.push_back(card_id_temp_vector);
			card_id_temp_set.clear();
			card_id_temp_vector.clear();
		}
		if (!used[r]) {
			card_id_temp_set.insert(r);
			used[r] = true;
		}
	}
	vector<int> bid_id_temp;
	for (int b = 0; b < 35; b++) {
		bid_id_temp.push_back(b);
	}



	player south("South", player_card_id[0], bid_id_temp);
	player west("West", player_card_id[1], bid_id_temp);
	player north("North", player_card_id[2], bid_id_temp);
	player east("East", player_card_id[3], bid_id_temp);

	player* which_player = NULL;
	player* declarer = NULL;
	player* dummy = NULL;



	//bidding
	srand(time(NULL));
	int dealer = rand() % 4;	// 0: S, 1: W, 2: N, 3: E
	int v = rand() % 4;	// 0: none, 1: NS, 2: EW, 3: both
	bool NS_vul = v % 2, EW_vul = v >= 2;

	switch (dealer) {
		case 0:
			which_player = &south;
			break;
		case 1:
			which_player = &west;
			break;
		case 2:
			which_player = &north;
			break;
		case 3:
			which_player = &east;
			break;
	}

	

	static int highest_bid_id = -1;
	static int trick_required = 0;
	static int trump = 0;
	static int passes = 0;
	bool doubled = false, redoubled = false;
	vector<vector<string>> bid_history;
	vector<string> bid_history_initialize;
	static int bid_history_index = 0;
	for (int j = 0; j < 4; j++) {
		bid_history_initialize.push_back(which_player->name);
		bid_history.push_back(bid_history_initialize);
		if (which_player == &south) {
			which_player = &west;	// next player
		}
		else if (which_player == &west) {
			which_player = &north;	// next player
		}
		else if (which_player == &north) {
			which_player = &east;	// next player
		}
		else if (which_player == &east) {
			which_player = &south;	// next player
		}
		bid_history_initialize.clear();
	}
	
	while (1) {
		switch (dealer) {
			case 0:
				cout << "Dealer: South" << endl;
				break;
			case 1:
				cout << "Dealer: West" << endl;
				break;
			case 2:
				cout << "Dealer: North" << endl;
				break;
			case 3:
				cout << "Dealer: East" << endl;
				break;
		}

		switch (v) {
			case 0:
				cout << "Vulnerable: none" << endl;
				break;
			case 1:
				cout << "Vulnerable: NS" << endl;
				break;
			case 2:
				cout << "Vulnerable: EW" << endl;
				break;
			case 3:
				cout << "Vulnerable: both" << endl;
				break;
		}

		cout << endl;

		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < bid_history[j].size(); k++) {
				if (k) {
					if (bid_history[j][k] == "Pass") {
						cout << bid_history[j][k] << "     ";
					}
					else if (bid_history[j][k] == "Double") {
						setTextColor(CYAN);
						cout << bid_history[j][k] << "   ";
						resetTextColor();
					}
					else if (bid_history[j][k] == "Redouble") {
						setTextColor(CYAN);
						cout << bid_history[j][k] << " ";
						resetTextColor();
					}
					switch (bid_history[j][k][1]) {
						case 'C':
							setTextColor(GREEN);
							cout << bid_history[j][k] << "       ";
							resetTextColor();
							break;
						case 'D':
							setTextColor(MAGENTA);
							cout << bid_history[j][k] << "       ";
							resetTextColor();
							break;
						case 'H':
							setTextColor(RED);
							cout << bid_history[j][k] << "       ";
							resetTextColor();
							break;
						case 'S':
							setTextColor(BLUE);
							cout << bid_history[j][k] << "       ";
							resetTextColor();
							break;
						case 'N':
							resetTextColor();
							cout << bid_history[j][k] << "      ";
							break;
					}
				 }
				else {
					cout << bid_history[j][k] << ": ";
					if ((bid_history[j][k] == "West") || (bid_history[j][k] == "East")) {
						cout << " ";
					}
				}
			}
			cout << endl;
		}
		cout << endl;
		


		which_player->print_name();
		cout << ":" << endl;

		which_player->print_cards();
		which_player->print_bids();
		cout << endl;

		string input_bid;
		int input_bid_id = -1;
		enter_bid:
		cout << "Enter your bid: ";
		cin >> input_bid;
		cout << endl;
		if (input_bid != "Pass") {
			if (input_bid != "Double") {
				if (input_bid != "Redouble") {
					player* partner = NULL;
					if (which_player == &south) {
						partner = &north;
					}
					else if (which_player == &west) {
						partner = &east;
					}
					else if (which_player == &north) {
						partner = &south;
					}
					else if (which_player == &east) {
						partner = &west;
					}
					switch (input_bid[0]) {
						case '1': case '2': case '3': case '4': case '5': case '6': case '7':
							trick_required = (int)input_bid[0] - 48 + 6;
							break;
						default:
							cout << endl << "Invalid bid!" << endl;
							goto enter_bid;
							break;
					}
					switch (input_bid[1]) {
						case 'C':
							input_bid_id = ((int)input_bid[0] - 48 - 1) * 5;
							// valid input (but not valid bid)
							if (input_bid_id <= highest_bid_id) {
								cout << endl << "Invalid bid!" << endl;
								goto enter_bid;
							}
							trump = club;
							which_player->club_bidden = true;
							if (!partner->club_bidden) {
								declarer = which_player;
								dummy = partner;
							}
							break;
						case 'D':
							input_bid_id = ((int)input_bid[0] - 48 - 1) * 5 + 1;
							// valid input (but not valid bid)
							if (input_bid_id <= highest_bid_id) {
								cout << endl << "Invalid bid!" << endl;
								goto enter_bid;
							}
							trump = diamond;
							which_player->diamond_bidden = true;
							if (!partner->diamond_bidden) {
								declarer = which_player;
								dummy = partner;
							}
							break;
						case 'H':
							input_bid_id = ((int)input_bid[0] - 48 - 1) * 5 + 2;
							// valid input (but not valid bid)
							if (input_bid_id <= highest_bid_id) {
								cout << endl << "Invalid bid!" << endl;
								goto enter_bid;
							}
							trump = heart;
							which_player->heart_bidden = true;
							if (!partner->heart_bidden) {
								declarer = which_player;
								dummy = partner;
							}
							break;
						case 'S':
							input_bid_id = ((int)input_bid[0] - 48 - 1) * 5 + 3;
							// valid input (but not valid bid)
							if (input_bid_id <= highest_bid_id) {
								cout << endl << "Invalid bid!" << endl;
								goto enter_bid;
							}
							trump = spade;
							which_player->spade_bidden = true;
							if (!partner->spade_bidden) {
								declarer = which_player;
								dummy = partner;
							}
							break;
						case 'N':
							if (input_bid[2] == 'T') {
								input_bid_id = ((int)input_bid[0] - 48 - 1) * 5 + 4;
								// valid input (but not valid bid)
								if (input_bid_id <= highest_bid_id) {
									cout << endl << "Invalid bid!" << endl;
									goto enter_bid;
								}
								trump = no_trump;
								which_player->no_trump_bidden = true;
								if (!partner->no_trump_bidden) {
									declarer = which_player;
									dummy = partner;
								}
								break;
							}
							else {
								cout << endl << "Invalid input!" << endl;
								goto enter_bid;
								break;
							}
						default:
							cout << endl << "Invalid input!" << endl;
							goto enter_bid;
							break;
					}
					south.disable_bids(input_bid_id);
					west.disable_bids(input_bid_id);
					north.disable_bids(input_bid_id);
					east.disable_bids(input_bid_id);
					highest_bid_id = input_bid_id;

					cout << endl << endl;

					south.can_double = false;
					west.can_double = false;
					north.can_double = false;
					east.can_double = false;
					south.can_redouble = false;
					west.can_redouble = false;
					north.can_redouble = false;
					east.can_redouble = false;

					doubled = false;
					redoubled = false;
					passes = 0;
					if ((which_player == &south) || (which_player == &north)) {
						west.can_double = true;
						east.can_double = true;
					}
					else if((which_player == &west) || (which_player == &east)) {
						south.can_double = true;
						north.can_double = true;
					}
				}
				else {	// Redouble
					if (which_player->can_redouble) {
						passes = 0;
						doubled = false;
						redoubled = true;
						if ((which_player == &south) || (which_player == &north)) {
							south.can_redouble = false;
							north.can_redouble = false;
						}
						else if ((which_player == &west) || (which_player == &east)) {
							west.can_redouble = false;
							east.can_redouble = false;
						}
					}
					else {
						cout << "You can't Redouble now!" << endl;
						goto enter_bid;
					}
				}
			}
			else {	// Double
				if (which_player->can_double) {
					passes = 0;
					doubled = true;
					if ((which_player == &south) || (which_player == &north)) {
						west.can_redouble = true;
						east.can_redouble = true;
						south.can_double = false;
						north.can_double = false;
					}
					else if ((which_player == &west) || (which_player == &east)) {
						south.can_redouble = true;
						north.can_redouble = true;
						west.can_double = false;
						east.can_double = false;
					}
				}
				else {
					cout << "You can't Double now!" << endl;
					goto enter_bid;
				}
			}
		}
		else {	// Pass
			passes++;
			if ((highest_bid_id == -1) && (passes == 4)) {
				bid_history[bid_history_index % 4].push_back(input_bid);
				passes = 0;
				bid_history_index = 0;
				cout << "All player passed, redeal cards to start a new game." << endl;
				cout << "Press Enter to continue";
				cin.get();
				cin.get();
				goto initializing;
			}
			else if ((highest_bid_id >= 0) && (passes == 3)) {
				bid_history[bid_history_index % 4].push_back(input_bid);
				goto start_to_play;
			}
		}

		// valid bid
		bid_history[bid_history_index % 4].push_back(input_bid);
		if (which_player == &south) {
			which_player = &west;	// next player
		}
		else if (which_player == &west) {
			which_player = &north;	// next player
		}
		else if (which_player == &north) {
			which_player = &east;	// next player
		}
		else if (which_player == &east) {
			which_player = &south;	// next player
		}
		bid_history_index++;
		system("cls");
	}



	start_to_play:
	system("cls");
	south.how_many_cards_in_each_suit();
	west.how_many_cards_in_each_suit();
	north.how_many_cards_in_each_suit();
	east.how_many_cards_in_each_suit();
	south.promote_trump(trump);
	west.promote_trump(trump);
	north.promote_trump(trump);
	east.promote_trump(trump);

	if (declarer == &south) {
		which_player = &west;
	}
	else if (declarer == &west) {
		which_player = &north;
	}
	else if (declarer == &north) {
		which_player = &east;
	}
	else if (declarer == &east) {
		which_player = &south;
	}

	int NS_trick = 0, EW_trick = 0;
	for (int round = 1; round <= 13; round++) {
		int suit_in_this_round = 0;
		int highest_rank_in_this_round = 0;
		typedef struct played_card {
			int suit = 0;
			int rank = 0;
			string from_who = "N/A";
			string play_input = "N/A";
		};
		played_card played_cards[4];

		for (int seat = 1; seat <= 4; seat++) {
			declarer->print_contract(highest_bid_id, doubled, redoubled);
			cout << endl;

			switch (v) {
				case 0:
					cout << "Vulnerable: none" << endl;
					break;
				case 1:
					cout << "Vulnerable: NS" << endl;
					break;
				case 2:
					cout << "Vulnerable: EW" << endl;
					break;
				case 3:
					cout << "Vulnerable: both" << endl;
					break;
			}

			cout << endl;

			for (int j = 0; j < 4; j++) {
				for (int k = 0; k < bid_history[j].size(); k++) {
					if (k) {
						if (bid_history[j][k] == "Pass") {
							cout << bid_history[j][k] << "     ";
						}
						else if (bid_history[j][k] == "Double") {
							setTextColor(CYAN);
							cout << bid_history[j][k] << "   ";
							resetTextColor();
						}
						else if (bid_history[j][k] == "Redouble") {
							setTextColor(CYAN);
							cout << bid_history[j][k] << " ";
							resetTextColor();
						}
						switch (bid_history[j][k][1]) {
							case 'C':
								setTextColor(GREEN);
								cout << bid_history[j][k] << "       ";
								resetTextColor();
								break;
							case 'D':
								setTextColor(MAGENTA);
								cout << bid_history[j][k] << "       ";
								resetTextColor();
								break;
							case 'H':
								setTextColor(RED);
								cout << bid_history[j][k] << "       ";
								resetTextColor();
								break;
							case 'S':
								setTextColor(BLUE);
								cout << bid_history[j][k] << "       ";
								resetTextColor();
								break;
							case 'N':
								resetTextColor();
								cout << bid_history[j][k] << "      ";
								break;
						}
					}
					else {
						cout << bid_history[j][k] << ": ";
						if ((bid_history[j][k] == "West") || (bid_history[j][k] == "East")) {
							cout << " ";
						}
					}
				}
				cout << endl;
			}
			cout << endl;

			cout << "NS trick: " << NS_trick << endl;
			cout << "EW trick: " << EW_trick << endl;
			cout << endl;


			for (int j = 0; j < seat - 1; j++) {
				cout << played_cards[j].from_who << " played: ";
				switch (played_cards[j].suit) {
					case club:
						setTextColor(GREEN);
						break;
					case diamond:
						setTextColor(MAGENTA);
						break;
					case heart:
						setTextColor(RED);
						break;
					case spade:
						setTextColor(BLUE);
						break;
				}
				cout << played_cards[j].play_input;
				resetTextColor();
				cout << endl;
			}

			if ((round != 1) || (seat != 1)) {	// Dummy's card face up
				if (which_player->name == dummy->name) {
					cout << "(Dummy's turn)" << endl;
					cout << "Declarer (";
					cout << declarer->name << ")'s cards:" << endl;
					declarer->print_cards();
					cout << endl;
				}
				else {
					cout << "Dummy (";
					cout << dummy->name << ")'s cards:" << endl;
					dummy->print_cards();
					cout << endl;
				}
			}

			cout << which_player->name << "'s cards:" << endl;
			which_player->print_cards();
			cout << "Enter the card to play: ";
			string card_to_play;
			play:
			cin >> card_to_play;
			played_cards[seat - 1].from_who = which_player->name;
			switch (card_to_play[0]) {
				case 'C':
					played_cards[seat - 1].suit = club;
					if (seat == 1) {
						suit_in_this_round = club;
					}
					break;
				case 'D':
					played_cards[seat - 1].suit = diamond;
					if (seat == 1) {
						suit_in_this_round = diamond;
					}
					break;
				case 'H':
					played_cards[seat - 1].suit = heart;
					if (seat == 1) {
						suit_in_this_round = heart;
					}
					break;
				case 'S':
					played_cards[seat - 1].suit = spade;
					if (seat == 1) {
						suit_in_this_round = spade;
					}
					break;
				default:
					cout << "Invalid play!" << endl;
					goto play;
			}
			switch (card_to_play[1]) {
				case 'J':
					played_cards[seat - 1].rank = jack;
					break;
				case 'Q':
					played_cards[seat - 1].rank = queen;
					break;
				case 'K':
					played_cards[seat - 1].rank = king;
					break;
				case 'A':
					played_cards[seat - 1].rank = ace;
					break;
				case '1':	// ten
					if (card_to_play[2] == '0') {
						played_cards[seat - 1].rank = 10;
						break;
					}
					else {
						cout << "Invalid play!" << endl;
						goto play;
					}
				case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
					played_cards[seat - 1].rank = (int)card_to_play[1] - 48;
					break;
				default:
					cout << "Invalid play!" << endl;
					goto play;
			}

			auto it = which_player->hand.begin();
			for (it; it != which_player->hand.end(); it++) {	// find the card
				if ((played_cards[seat - 1].rank == it->rank) && (played_cards[seat - 1].suit == it->suit)) {
					break;
				}
				if (it + 1 == which_player->hand.end()) {	// not found
					cout << "You don't have this card!" << endl;
					goto play;
				}
			}

			if ((which_player->count_of_each_suit[suit_in_this_round - 1] != 0) && (played_cards[seat - 1].suit != suit_in_this_round)) {	// revoke
				cout << "You must follow suit when possible!" << endl;
				goto play;
			}

			// valid play
			played_cards[seat - 1].play_input = card_to_play;
			if (it->is_trump) {
				played_cards[seat - 1].rank += 13;
			}
			else if (it->suit != suit_in_this_round) {
				played_cards[seat - 1].rank = 1;
			}

			which_player->count_of_each_suit[it->suit - 1]--;	// remove the card
			which_player->hand.erase(it);	// remove the card

			if (which_player == &south) {
				which_player = &west;	// next player
			}
			else if (which_player == &west) {
				which_player = &north;	// next player
			}
			else if (which_player == &north) {
				which_player = &east;	// next player
			}
			else if (which_player == &east) {
				which_player = &south;	// next player
			}

			system("cls");
		}

		played_card winning_card;
		winning_card.from_who = "N/A";
		winning_card.suit = 0;
		winning_card.rank = 0;
		for (int j = 0; j < 4; j++) {
			if (played_cards[j].rank > winning_card.rank) {
				winning_card = played_cards[j];
			}
		}

		if (winning_card.from_who == "South") {
			which_player = &south;
			NS_trick++;
		}
		else if (winning_card.from_who == "West") {
			which_player = &west;
			EW_trick++;
		}
		else if (winning_card.from_who == "North") {
			which_player = &north;
			NS_trick++;
		}
		else if (winning_card.from_who == "East") {
			which_player = &east;
			EW_trick++;
		}

		cout << endl;
	}

	// hand over
	declarer->print_contract(highest_bid_id, doubled, redoubled);
	cout << endl;

	switch (v) {
		case 0:
			cout << "Vulnerable: none" << endl;
			break;
		case 1:
			cout << "Vulnerable: NS" << endl;
			break;
		case 2:
			cout << "Vulnerable: EW" << endl;
			break;
		case 3:
			cout << "Vulnerable: both" << endl;
			break;
	}

	cout << endl;

	cout << "NS trick: " << NS_trick << endl;
	cout << "EW trick: " << EW_trick << endl;

	if ((declarer->name == "South") || (declarer->name == "North")) {
		if (NS_trick >= trick_required) {
			cout << "score: " << win(trick_required, NS_trick, trump, trick_required - 6, NS_vul, doubled, redoubled) << endl;
		}
		else {
			cout << "score: " << lose(trick_required, NS_trick, NS_vul, doubled, redoubled) << endl;
		}
	}
	else if ((declarer->name == "West") || (declarer->name == "East")) {
		if (EW_trick >= trick_required) {
			cout << "score: " << -1 * win(trick_required, EW_trick, trump, trick_required - 6, EW_vul, doubled, redoubled) << endl;
		}
		else {
			cout << "score: " << -1 * lose(trick_required, EW_trick, EW_vul, doubled, redoubled) << endl;
		}
	}

	cout << "Press Enter to quit";
	cin.get();
	cin.get();
	return 0;
}