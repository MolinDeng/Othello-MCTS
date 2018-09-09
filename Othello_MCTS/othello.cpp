#include "othello.h"
#include "capture.h"
#include "UCT.h"
#include<future>      
#include<utility> 

static IMAGE img[6];
static MOUSEMSG m;
static int Round = 1;
void SetWindows() {
	// 加载pic
	loadimage(&img[0], "pic\\Blank.bmp");
	loadimage(&img[1], "pic\\Black.bmp");
	loadimage(&img[2], "pic\\White.bmp");
	loadimage(&img[3], "pic\\Black1.bmp");
	loadimage(&img[4], "pic\\White1.bmp");
	loadimage(&img[5], "pic\\Move.bmp");
	//初始化棋盘
	initgraph(340, 340);
	IMAGE of_board;
	loadimage(&of_board, "pic\\Board.bmp");
	putimage(0, 0, &of_board);
	setorigin(26, 26);
	SetWindowText(GetHWnd(), "Othello");
	int scrWidth, scrHeight;
	RECT rect;
	//获得屏幕尺寸
	scrWidth = GetSystemMetrics(SM_CXSCREEN);
	scrHeight = GetSystemMetrics(SM_CYSCREEN);
	//取得窗口尺寸
	GetWindowRect(GetHWnd(), &rect);
	//重新设置rect里的值
	rect.left = (scrWidth - rect.right) / 2;
	rect.top = (scrHeight - rect.bottom) / 2;
	SetWindowPos(GetHWnd(), HWND_TOP, rect.left, rect.top, 356, 379, SWP_SHOWWINDOW);
}

int Search(int size) {
	srand(time(NULL)); 
	return rand() % size;
}

int HumanChoice(std::vector<position> &Moves) {
	bool isFind;
	int i;
	while (true) {
		isFind = false;
		while (true) {
			m = GetMouseMsg();// 获取鼠标消息
			if (m.uMsg == WM_LBUTTONDOWN && m.x - 26 < 37 * 8 && m.y - 26 < 37 * 8)
				break;// 如果左键点击
		}
		int row = (m.y - 26) / 37, col = (m.x - 26) / 37;
		for (i = 0; i < Moves.size(); i++)
			if (row == Moves[i].Row && col == Moves[i].Col) {
				isFind = true;
				break;
			}
		if (isFind)	break;
	}
	return i;
}

othello CreateNewGame_AIfist()
{
	othello Othello = {};

	Othello.Board = CreateNewBoard();
	Othello.LightPlayer = CreateNewPlayer("Human", COLOR_LIGHT, false);
	Othello.DarkPlayer = CreateNewPlayer("AI", COLOR_DARK, true);

	return Othello;
}

othello CreateNewGame_Humanfist()
{
	othello Othello = {};

	Othello.Board = CreateNewBoard();
	Othello.LightPlayer = CreateNewPlayer("AI", COLOR_LIGHT, true);
	Othello.DarkPlayer = CreateNewPlayer("Human", COLOR_DARK, false);

	return Othello;
}
board CreateNewBoard()
{
	board Board = {};
	for (int col = 0; col < BOARD_SIZE; ++col)
	{
		for (int row = 0; row < BOARD_SIZE; ++row)
			Board.State[row][col] = COLOR_EMPTY;
	}

	Board.State[3][3] = COLOR_LIGHT;
	Board.State[3][4] = COLOR_DARK;
	Board.State[4][3] = COLOR_DARK;
	Board.State[4][4] = COLOR_LIGHT;
	Board.EmptyPieces = 60;

	return Board;
}

player CreateNewPlayer(std::string Name, piece_color Color, bool isAI)
{
	player Player = {};

	Player.Name = Name;
	Player.Color = Color;
	Player.Pieces = 0;
	Player.OutOfMoves = false;
	Player.AIPlayer = isAI;

	return Player;
}

player *FindActivePlayer(othello *Othello, piece_color PlayerTurn)
{
	return PlayerTurn == COLOR_DARK ? &Othello->DarkPlayer : &Othello->LightPlayer;
}

void CountPiecesOfPlayer(board *Board, player *Player)
{
	Player->Pieces = 0;
	for (int Col = 0; Col < BOARD_SIZE; ++Col)
	{
		for (int Row = 0; Row < BOARD_SIZE; ++Row)
		{
			if (Board->State[Row][Col] == Player->Color)
				++Player->Pieces;
		}
	}
}

player *FindWinner(othello *Othello)
{
	if (Othello->LightPlayer.Pieces > Othello->DarkPlayer.Pieces)
		return &Othello->LightPlayer;
	else if (Othello->LightPlayer.Pieces < Othello->DarkPlayer.Pieces)
		return &Othello->DarkPlayer;
	else
		return NULL;

}

std::vector<position> GetLegalMoves(board *Board, player *Player)
{
	std::vector<position> Moves;
	for (int Col = 0; Col < BOARD_SIZE; ++Col)
	{
		for (int Row = 0; Row < BOARD_SIZE; ++Row)
		{
			position Pos = { Row, Col };
			if (IsMoveLegal(Board, &Pos, Player))
				Moves.push_back(Pos);//Pos中包含某个下子点，以及这个下子点能吃的坐标(在capture中)
		}
	}
	return Moves;
}

bool IsMoveLegal(board *Board, position *Pos, player *Player)
{
	if ((Pos->Row >= BOARD_SIZE || Pos->Row < 0) ||
		(Pos->Col >= BOARD_SIZE || Pos->Col < 0) ||
		(Board->State[Pos->Row][Pos->Col] != COLOR_EMPTY))
		return false;

	bool Result = false;

	if (CheckNorth(Board, Pos, Player))
		Result = true;
	if (CheckEast(Board, Pos, Player))
		Result = true;
	if (CheckSouth(Board, Pos, Player))
		Result = true;
	if (CheckWest(Board, Pos, Player))
		Result = true;
	if (CheckNorthEast(Board, Pos, Player))
		Result = true;
	if (CheckNorthWest(Board, Pos, Player))
		Result = true;
	if (CheckSouthEast(Board, Pos, Player))
		Result = true;
	if (CheckSouthWest(Board, Pos, Player))
		Result = true;

	return Result;
}

void PerformMove(board *Board, position *Pos, player *Player)
{
	Board->State[Pos->Row][Pos->Col] = Player->Color;
	for (int Index = 0; Index < Pos->Captured.size(); ++Index)
	{
		position Piece = Pos->Captured[Index];
		Board->State[Piece.Row][Piece.Col] = Player->Color;
	}

	//Pos->Captured.clear();
	--Board->EmptyPieces;
}

void PrintBoard(board *Board)
{
	for (int Row = 0; Row < BOARD_SIZE; ++Row)
		for (int Col = 0; Col < BOARD_SIZE; ++Col)
			switch (Board->State[Row][Col]) {
			case COLOR_DARK:
				putimage(37 * Col, 37 * Row, &img[1]);
				break;
			case COLOR_LIGHT:
				putimage(37 * Col, 37 * Row, &img[2]);
				break;
			default:
				putimage(37 * Col, 37 * Row, &img[0]);
				break;
			}
}

void MarkLastMove(position *Pos, piece_color Color) {
	putimage(37 * Pos->Col, 37 * Pos->Row, &img[Color + 3]);
}

_time getmilliseconds() {
	using namespace std::chrono;
	return (duration_cast<milliseconds>(steady_clock::now().time_since_epoch())).count();
}

//bool GamePlay() {
//	std::cout << "***** " << "Round " << Round++ << " *****" << std::endl;
//	othello Othello = CreateNewGame();
//	piece_color PlayerTurn = COLOR_DARK;
//	PrintBoard(&Othello.Board);
//	while (Othello.Board.EmptyPieces != 0) {
//		if (Othello.LightPlayer.OutOfMoves && Othello.DarkPlayer.OutOfMoves)
//			break;
//		player *ActivePlayer = FindActivePlayer(&Othello, PlayerTurn);
//		std::vector<position> Moves = GetLegalMoves(&Othello.Board, ActivePlayer);
//		for (auto p : Moves)  putimage(37 * p.Col, 37 * p.Row, &img[5]);
//		if (!Moves.empty()) {
//			int Choice;
//			ActivePlayer->OutOfMoves = false;
//			if (ActivePlayer->AIPlayer)
//				Choice = Search(Moves.size());
//			else
//				Choice = HumanChoice(Moves);
//			PerformMove(&Othello.Board, &Moves[Choice], ActivePlayer);
//			PrintBoard(&Othello.Board);
//			MarkLastMove(&Moves[Choice], ActivePlayer->Color);
//		}
//		else ActivePlayer->OutOfMoves = true;
//		PlayerTurn = PlayerTurn == COLOR_LIGHT ? COLOR_DARK : COLOR_LIGHT;
//	}
//	PrintBoard(&Othello.Board);
//	CountPiecesOfPlayer(&Othello.Board, &Othello.LightPlayer);
//	CountPiecesOfPlayer(&Othello.Board, &Othello.DarkPlayer);
//	player *Winner = FindWinner(&Othello);
//
//	HWND wnd = GetHWnd();
//	char info[30];
//	std::ostrstream strout(info, 30);
//	if (!Winner)
//		strout << "Draw!" << std::endl;
//	else
//		strout << Winner->Name << " won with " << std::to_string(Winner->Pieces) << " pieces!" << std::endl;
//	strout << "One more game?" << std::ends;
//	return IDYES == MessageBox(wnd, info, "Game over", MB_YESNO | MB_ICONQUESTION);
//}

bool UCT_GamePlay() {
	std::cout << "***** " << "Round " << Round++ << " *****" << std::endl;
	othello Othello = CreateNewGame_AIfist();
	piece_color PlayerTurn = COLOR_DARK;
	_time total_time = 0;
	PrintBoard(&Othello.Board);
	while (Othello.Board.EmptyPieces != 0) {
		if (Othello.LightPlayer.OutOfMoves && Othello.DarkPlayer.OutOfMoves)
			break;
		player *ActivePlayer = FindActivePlayer(&Othello, PlayerTurn);
		std::vector<position> Moves = GetLegalMoves(&Othello.Board, ActivePlayer);
		for (auto p : Moves)  putimage(37 * p.Col, 37 * p.Row, &img[5]);
		if (!Moves.empty()) {
			position _at;
			ActivePlayer->OutOfMoves = false;
			if (ActivePlayer->AIPlayer) {
				UCT uct;
				piece_color tmp_turn = PlayerTurn == COLOR_LIGHT ? COLOR_DARK : COLOR_LIGHT;
				player Player = *ActivePlayer;
				player Opponent = *FindActivePlayer(&Othello, tmp_turn);
				_time t1 = getmilliseconds();
				/*std::future<position> futurePosition = std::async(std::mem_fn(&UCT::run), uct, Player, Opponent, Othello.Board);
				_at = futurePosition.get();*/
				_at = uct.run(Player, Opponent, Othello.Board);
				_time t2 = getmilliseconds();
				total_time += (t2 - t1);
				std::cout << "AI: (" << _at.Row + 1
					<< "," << (char)(_at.Col + 65) << ") "
					<< "with " << (double)(t2 - t1) / CLOCKS_PER_SEC << " s" << std::endl;
			}
			else _at = Moves[HumanChoice(Moves)];
			PerformMove(&Othello.Board, &_at, ActivePlayer);
			PrintBoard(&Othello.Board);
			MarkLastMove(&_at, ActivePlayer->Color);
		}
		else ActivePlayer->OutOfMoves = true;
		PlayerTurn = PlayerTurn == COLOR_LIGHT ? COLOR_DARK : COLOR_LIGHT;
	}
	PrintBoard(&Othello.Board);
	CountPiecesOfPlayer(&Othello.Board, &Othello.LightPlayer);
	CountPiecesOfPlayer(&Othello.Board, &Othello.DarkPlayer);
	player *Winner = FindWinner(&Othello);
	std::cout << "AI total time: " << (double)total_time / CLOCKS_PER_SEC << " s" << std::endl;
	HWND wnd = GetHWnd();
	char info[50];
	std::ostrstream strout(info, 50);
	if (!Winner)
		strout << "Draw!" << std::endl;
	else
		strout << Winner->Name << " won with " << std::to_string(Winner->Pieces) << " pieces!" << std::endl;
	strout << "One more game?" << std::ends;
	return IDYES == MessageBox(wnd, info, "Game over", MB_YESNO | MB_ICONQUESTION);
}