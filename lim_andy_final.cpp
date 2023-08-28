// Andy Lim
// CIS-5 Hybrid
// 12/1/21
// Final

#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <time.h>

using namespace std;

void createBoard(string**& board, bool**& win_board, bool**& aiWin);
void printBoard(string**& board);
bool checkRepeat(vector<int>& save_num, int position);
void insert(string**& board, bool**& win_board, bool**& cpuWin, int num, string turn);
bool checkWinner(bool**& win_board);
bool checkWinnerCpu(bool**& aiWin);
bool blockPlayer(bool**& win_board, vector<int>& save_num, int& place);
bool cpuWin(bool**& aiWin, vector<int>& save_num, int& place);
void destroy(bool**& win_board, bool**& aiWin, string**& board);
void clear(string**& board, bool**& aiWin, bool**& win_board);
void menu();

int main()
{
	//variables to function the entire code
	srand(time(NULL));
	menu();
	string name;
	string turn = "X";
	string** board;
	bool** win_board;
	bool** aiWin;
	vector<int> save_num;
	int counts = 0;
	int win = 0;
	int score = 0;
	int tie = 0;
	int lose = 0;
	int position;
	bool isRepeat;
	int newPosition = 0;
	createBoard(board, win_board, aiWin);
	bool gameEnded = false;

	
	

	cout << "Enter your name:" << endl; 
	cin >> name;


	do // while loop for the game board
	{
		if (turn == "X")
		{
			cout << "Enter a position between (1 and 9 inclusive): "; // placement for playing the game
			cin >> position;
			while (cin.fail())
			{
				cout << "Please enter a number (1-9): ";
				cin.clear();
				cin.ignore(256, '\n');
				cin >> position;
			}
			cout << endl;
			isRepeat = checkRepeat(save_num, position);
			while (isRepeat || position < 1 || position > 9)
			{
				cout << "Enter a different position" << endl; // if a position is covered already it fails
				cin >> position;
				while (cin.fail())
				{
					cout << "Please enter a number (1-9): "; // placement for playing the game
					cin.clear();
					cin.ignore(256, '\n');
					cin >> position;
				}
				isRepeat = checkRepeat(save_num, position);
			}
			save_num.push_back(position);
			insert(board, win_board, aiWin, position, turn);
			if (checkWinner(win_board)) // checks to see if player won
			{
				cout << name << " has won" << endl; // if user has won score is added
				win++;
				cout << name << win++ << endl;
				gameEnded = true;
			}
			turn = "O";
		}

		else
		{
			position = (rand() % 9) + 1;
			isRepeat = checkRepeat(save_num, position);
			while (isRepeat)
			{
				position = (rand() % 9) + 1; // save the position
				isRepeat = checkRepeat(save_num, position);
			}
			if (cpuWin(aiWin, save_num, newPosition))
				position = newPosition;
			else if (blockPlayer(win_board, save_num, newPosition))
				position = newPosition;
			save_num.push_back(position);
			insert(board, win_board, aiWin, position, turn);
			if (checkWinnerCpu(aiWin)) // checks for AI win
			{
				
				cout << "AI has won" << endl; // if AI wins score is added
				lose++;
				cout << "AI score: " << lose++ << endl;
				gameEnded = true;
			}
			turn = "X";
		}
		printBoard(board);
		cout << endl;
		counts++;
		if (gameEnded || counts >= 9) // makes the user either quit or play again
		{
			if (!gameEnded)
				cout << "Game ended in a tie" << endl; // if game is tied score is added
				tie++;
			cout << "Do you want to play again?" << endl;
			cout << "Type 1 to play again and 0 to exit" << endl;
			int again;
			cin >> again;
			while (cin.fail() || again > 1 || again < 0)
			{
				cout << "Please enter a number 1 or 0 ";
				cin.clear();
				cin.ignore(256, '\n');
				cin >> again;
			}
			if (again)
			{
				counts = 0;
				gameEnded = false;
				save_num.clear();
				clear(board, aiWin, win_board);
				menu();
			}
		}
	} while (counts < 9 && !gameEnded);
	destroy(win_board, aiWin, board);

	cout << "Wins: " << win++ << endl; // shows the score board
	cout << "Ties:" << tie++ << endl;
	cout << "Losses:"<< lose++ << endl;

	system("pause");
	return 0;

}

//prints the menu game instructions

void menu()
{
	int choice;
	cout << "Welcome to Tic Tac Toe! \n" << endl;
	cout << " (1)Single Player.\n" << endl;
	cout << " (2)Multiplayer.\n" << endl;
	cout << " (3)Quit.\n" << endl;
	cout << endl;
	cin >> choice;
	switch (choice)
	{
	case 1:
		cout << "You've selected Single Player.\n" << endl;
		break;
	case 2:
		cout << "You've selected Multiplayer.\n" << endl;
		break;
	case 3:
		exit(1);
	}



	cout << " 1 | 2 " << "  | 3   " << endl; // layout of the board
	cout << "------" << "------" << endl;
	cout << " 4 | 5 " << "  | 6   " << endl;
	cout << "------" << "-------" << endl;
	cout << " 7 | 8" << "   | 9   " << endl;
}

//creates the game board
//and two other board
//one for the player and the other for AI
void createBoard(string**& board, bool**& win_board, bool**& aiWin)
{
	const int row = 3;
	const int col = 3;
	board = new string*[row];
	win_board = new bool*[row];
	aiWin = new bool*[row];
	for (int i = 0; i < row; i++)
	{
		board[i] = new string[col];
		win_board[i] = new bool[col];
		aiWin[i] = new bool[col];
		for (int j = 0; j < col; j++)
		{
			board[i][j] = " ";
			win_board[i][j] = false;
			aiWin[i][j] = false;
		}
	}
}

//prints the board
void printBoard(string**& board)
{
	const int row = 3;
	const int col = 3;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; ++j)
		{
			cout << board[i][j] << " |";
		}
		cout << endl << "--+-----+" << endl;
	}
}

//check to see if there is any duplicate
//returns true if there is
bool checkRepeat(vector<int>& save_num, int position)
{
	for (unsigned int i = 0; i < save_num.size(); i++)
	{
		if (save_num[i] == position)
		{
			return true;
		}
	}
	return false;
}

//inserts into the board with the position
//the player/CPU chose
void insert(string**& board, bool**& win_board, bool**& aiWin, int num, string turn)
{
	switch (num)
	{
	case 1:
		board[0][0] = turn;
		if (turn == "X")
			win_board[0][0] = true;
		else
			aiWin[0][0] = true;
		break;
	case 2:
		board[0][1] = turn;
		if (turn == "X")
			win_board[0][1] = true;
		else
			aiWin[0][1] = true;
		break;
	case 3:
		board[0][2] = turn;
		if (turn == "X")
			win_board[0][2] = true;
		else
			aiWin[0][2] = true;
		break;
	case 4:
		board[1][0] = turn;
		if (turn == "X")
			win_board[1][0] = true;
		else
			aiWin[1][0] = true;
		break;
	case 5:
		board[1][1] = turn;
		if (turn == "X")
			win_board[1][1] = true;
		else
			aiWin[1][1] = true;
		break;
	case 6:
		board[1][2] = turn;
		if (turn == "X")
			win_board[1][2] = true;
		else
			aiWin[1][2] = true;
		break;
	case 7:
		board[2][0] = turn;
		if (turn == "X")
			win_board[2][0] = true;
		else
			aiWin[2][0] = true;
		break;
	case 8:
		board[2][1] = turn;
		if (turn == "X")
			win_board[2][1] = true;
		else
			aiWin[2][1] = true;
		break;
	case 9:
		board[2][2] = turn;
		if (turn == "X")
			win_board[2][2] = true;
		else
			aiWin[2][2] = true;
		break;
	}

}

//checks to see if the player has won
//returns true if so
bool checkWinner(bool**& win_board)
{
	const int row = 3;
	const int col = 3;
	for (int i = 0; i < row; ++i)
	{
		if (win_board[i][0] && win_board[i][1] && win_board[i][2])
		{
			return true;
		}
	}
	for (int j = 0; j < col; ++j)
	{
		if (win_board[0][j] && win_board[1][j] && win_board[2][j])
		{
			return true;
		}
	}

	if (win_board[0][2] && win_board[1][1] && win_board[2][0])
	{
		return true;
	}

	if (win_board[0][0] && win_board[1][1] && win_board[2][2])
	{
		return true;
	}

	return false;
}

//checks to see if the CPU has won
//returns true if so
bool checkWinnerCpu(bool**& aiWin)
{
	const int row = 3;
	const int col = 3;
	for (int i = 0; i < row; ++i)
	{
		if (aiWin[i][0] && aiWin[i][1] && aiWin[i][2])
		{
			return true;
		}
	}
	for (int j = 0; j < col; ++j)
	{
		if (aiWin[0][j] && aiWin[1][j] && aiWin[2][j])
		{
			return true;
		}
	}

	if (aiWin[0][2] && aiWin[1][1] && aiWin[2][0])
	{
		return true;
	}

	if (aiWin[0][0] && aiWin[1][1] && aiWin[2][2])
	{
		return true;
	}

	return false;
}

//checks to see if the CPU can block the player
//returns true if it can
bool blockPlayer(bool**& win_board, vector<int>& save_num, int& place)
{
	const int row = 3;
	const int col = 3;
	bool noDuplicate = false;
	for (int i = 0; i < row; i++)
	{
		//right column
		if (win_board[i][0] && win_board[i][1])
		{
			place = (3 * i) + 3;
			noDuplicate = checkRepeat(save_num, place);
			if (!noDuplicate) return true;
		}

		//bottom row
		if (win_board[0][i] && win_board[1][i])
		{
			place = (i % 7) + 7;
			noDuplicate = checkRepeat(save_num, place);
			if (!noDuplicate) return true;
		}

		//top row
		if (win_board[2][i] && win_board[1][i])
		{
			place = i + 1;
			noDuplicate = checkRepeat(save_num, place);
			if (!noDuplicate) return true;
		}

		//side left column
		if (win_board[i][2] && win_board[i][1])
		{
			place = (3 * i) + 1;
			noDuplicate = checkRepeat(save_num, place);
			if (!noDuplicate) return true;
		}

		//middle column
		if (win_board[i][2] && win_board[i][0])
		{
			place = (3 * i) + 2;
			noDuplicate = checkRepeat(save_num, place);
			if (!noDuplicate) return true;
		}

		//middle row
		if (win_board[2][i] && win_board[0][i])
		{
			place = (i % 4) + 4;
			noDuplicate = checkRepeat(save_num, place);
			if (!noDuplicate) return true;
		}
	}

	//corners check
	for (int i = 0; i < col;)
	{
		//bottom right
		//and bottom left
		if (win_board[0][i] && win_board[1][1])
		{
			place = 9 - i;
			noDuplicate = checkRepeat(save_num, place);
			if (!noDuplicate) return true;
		}

		//top right and top left
		if (win_board[2][i] && win_board[1][1])
		{
			place = 3 - i;
			noDuplicate = checkRepeat(save_num, place);
			if (!noDuplicate) return true;
		}
		i += 2;
	}

	//check middle
	if (win_board[0][2] && win_board[2][0])
	{
		place = 5;
		noDuplicate = checkRepeat(save_num, place);
		if (!noDuplicate) return true;
	}
	if (win_board[0][0] && win_board[2][2])
	{
		place = 5;
		noDuplicate = checkRepeat(save_num, place);
		if (!noDuplicate) return true;
	}

	return false;
}

//checks to see if the CPU can win
//returns true if so
bool cpuWin(bool**& aiWin, vector<int>& save_num, int& place)
{
	const int row = 3;
	const int col = 3;
	bool noDuplicate = false;
	for (int i = 0; i < row; i++)
	{
		//right column
		if (aiWin[i][0] && aiWin[i][1])
		{
			place = (3 * i) + 3;
			noDuplicate = checkRepeat(save_num, place);
			if (!noDuplicate) return true;
		}

		//bottom row
		if (aiWin[0][i] && aiWin[1][i])
		{
			place = (i % 7) + 7;
			noDuplicate = checkRepeat(save_num, place);
			if (!noDuplicate) return true;
		}

		//top row
		if (aiWin[2][i] && aiWin[1][i])
		{
			place = i + 1;
			noDuplicate = checkRepeat(save_num, place);
			if (!noDuplicate) return true;
		}

		//side left column
		if (aiWin[i][2] && aiWin[i][1])
		{
			place = (3 * i) + 1;
			noDuplicate = checkRepeat(save_num, place);
			if (!noDuplicate) return true;
		}

		//middle column
		if (aiWin[i][2] && aiWin[i][0])
		{
			place = (3 * i) + 2;
			noDuplicate = checkRepeat(save_num, place);
			if (!noDuplicate) return true;
		}

		//middle row
		if (aiWin[2][i] && aiWin[0][i])
		{
			place = (i % 4) + 4;
			noDuplicate = checkRepeat(save_num, place);
			if (!noDuplicate) return true;
		}
	}

	//corners check
	for (int i = 0; i < col;)
	{
		//bottom right
		//and bottom left
		if (aiWin[0][i] && aiWin[1][1])
		{
			place = 9 - i;
			noDuplicate = checkRepeat(save_num, place);
			if (!noDuplicate) return true;
		}

		//top right and top left
		if (aiWin[2][i] && aiWin[1][1])
		{
			place = 3 - i;
			noDuplicate = checkRepeat(save_num, place);
			if (!noDuplicate) return true;
		}
		i += 2;
	}

	//check middle
	if (aiWin[0][2] && aiWin[2][0])
	{
		place = 5;
		noDuplicate = checkRepeat(save_num, place);
		if (!noDuplicate) return true;
	}
	if (aiWin[0][0] && aiWin[2][2])
	{
		place = 5;
		noDuplicate = checkRepeat(save_num, place);
		if (!noDuplicate) return true;
	}

	return false;
}

//clears the heap
//after game is over
void destroy(bool**& win_board, bool**& aiWin, string**& board)
{
	const int row = 3;
	for (int i = 0; i < row; i++)
	{
		delete[] board[i];
		delete[] aiWin[i];
		delete[] win_board[i];
	}
	delete[] board;
	delete[] win_board;
	delete[] aiWin;
}

//clears the board for rematch
void clear(string**& board, bool**& aiWin, bool**& win_board)
{
	const int row = 3;
	const int col = 3;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			board[i][j] = " ";
			win_board[i][j] = false;
			aiWin[i][j] = false;
		}
	}
}