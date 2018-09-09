#include "capture.h"

bool CheckNorth(board *Board, position *Pos, player *Player)
{
	piece_color OpponentColor = Player->Color == COLOR_LIGHT ? COLOR_DARK : COLOR_LIGHT;

	int PlayerCount = 0;
	int OpponentCount = 0;

	if (Pos->Row - 2 < 0)
		return false;

	std::vector<position> Captured;
	for (int Row = Pos->Row - 1; Row >= 0; --Row)
	{
		position CheckPosition = { Row, Pos->Col };
		piece_color Piece = Board->State[CheckPosition.Row][CheckPosition.Col];
		if (Piece == OpponentColor)
		{
			Captured.push_back(CheckPosition);
			++OpponentCount;
		}
		else if (Piece == Player->Color)
		{
			++PlayerCount;
			break;
		}
		else if (Piece == COLOR_EMPTY)
		{
			Captured.clear();
			break;
		}
	}

	bool Result = OpponentCount > 0 && PlayerCount == 1;
	if (Result)
	{
		for (int Index = 0; Index < Captured.size(); ++Index)
			Pos->Captured.push_back(Captured[Index]);
	}

	return Result;
}

bool CheckSouth(board *Board, position *Pos, player *Player)
{
	piece_color OpponentColor = Player->Color == COLOR_LIGHT ? COLOR_DARK : COLOR_LIGHT;

	int PlayerCount = 0;
	int OpponentCount = 0;

	if (Pos->Row + 2 >= BOARD_SIZE)
		return false;

	std::vector<position> Captured;
	for (int Row = Pos->Row + 1; Row < BOARD_SIZE; ++Row)
	{
		position CheckPosition = { Row, Pos->Col };
		piece_color Piece = Board->State[CheckPosition.Row][CheckPosition.Col];
		if (Piece == OpponentColor)
		{
			Captured.push_back(CheckPosition);
			++OpponentCount;
		}
		else if (Piece == Player->Color)
		{
			++PlayerCount;
			break;
		}
		else if (Piece == COLOR_EMPTY)
		{
			Captured.clear();
			break;
		}
	}

	bool Result = OpponentCount > 0 && PlayerCount == 1;
	if (Result)
	{
		for (int Index = 0; Index < Captured.size(); ++Index)
			Pos->Captured.push_back(Captured[Index]);
	}

	return Result;
}

bool CheckEast(board *Board, position *Pos, player *Player)
{
	piece_color OpponentColor = Player->Color == COLOR_LIGHT ? COLOR_DARK : COLOR_LIGHT;

	int PlayerCount = 0;
	int OpponentCount = 0;

	if (Pos->Col + 2 >= BOARD_SIZE)
		return false;

	std::vector<position> Captured;
	for (int Col = Pos->Col + 1; Col < BOARD_SIZE; ++Col)
	{
		position CheckPosition = { Pos->Row, Col };
		piece_color Piece = Board->State[CheckPosition.Row][CheckPosition.Col];
		if (Piece == OpponentColor)
		{
			Captured.push_back(CheckPosition);
			++OpponentCount;
		}
		else if (Piece == Player->Color)
		{
			++PlayerCount;
			break;
		}
		else if (Piece == COLOR_EMPTY)
		{
			Captured.clear();
			break;
		}
	}

	bool Result = OpponentCount > 0 && PlayerCount == 1;
	if (Result)
	{
		for (int Index = 0; Index < Captured.size(); ++Index)
			Pos->Captured.push_back(Captured[Index]);
	}

	return Result;
}

bool CheckWest(board *Board, position *Pos, player *Player)
{
	piece_color OpponentColor = Player->Color == COLOR_LIGHT ? COLOR_DARK : COLOR_LIGHT;

	int PlayerCount = 0;
	int OpponentCount = 0;

	if (Pos->Col - 2 < 0)
		return false;

	std::vector<position> Captured;
	for (int Col = Pos->Col - 1; Col >= 0; --Col)
	{
		position CheckPosition = { Pos->Row, Col };
		piece_color Piece = Board->State[CheckPosition.Row][CheckPosition.Col];
		if (Piece == OpponentColor)
		{
			Captured.push_back(CheckPosition);
			++OpponentCount;
		}
		else if (Piece == Player->Color)
		{
			++PlayerCount;
			break;
		}
		else if (Piece == COLOR_EMPTY)
		{
			Captured.clear();
			break;
		}
	}

	bool Result = OpponentCount > 0 && PlayerCount == 1;
	if (Result)
	{
		for (int Index = 0; Index < Captured.size(); ++Index)
			Pos->Captured.push_back(Captured[Index]);
	}

	return Result;
}

bool CheckNorthEast(board *Board, position *Pos, player *Player)
{
	piece_color OpponentColor = Player->Color == COLOR_LIGHT ? COLOR_DARK : COLOR_LIGHT;

	int PlayerCount = 0;
	int OpponentCount = 0;

	if (Pos->Row - 2 < 0 || Pos->Col + 2 >= BOARD_SIZE)
		return false;

	std::vector<position> Captured;
	for (int Row = Pos->Row - 1, Col = Pos->Col + 1;
		Row >= 0 && Col < BOARD_SIZE;
		--Row, ++Col)
	{
		position CheckPosition = { Row, Col };
		piece_color Piece = Board->State[CheckPosition.Row][CheckPosition.Col];
		if (Piece == OpponentColor)
		{
			Captured.push_back(CheckPosition);
			++OpponentCount;
		}
		else if (Piece == Player->Color)
		{
			++PlayerCount;
			break;
		}
		else if (Piece == COLOR_EMPTY)
		{
			Captured.clear();
			break;
		}
	}

	bool Result = OpponentCount > 0 && PlayerCount == 1;
	if (Result)
	{
		for (int Index = 0; Index < Captured.size(); ++Index)
			Pos->Captured.push_back(Captured[Index]);
	}

	return Result;
}

bool CheckNorthWest(board *Board, position *Pos, player *Player)
{
	piece_color OpponentColor = Player->Color == COLOR_LIGHT ? COLOR_DARK : COLOR_LIGHT;

	int PlayerCount = 0;
	int OpponentCount = 0;

	if (Pos->Row - 2 < 0 || Pos->Col - 2 < 0)
		return false;

	std::vector<position> Captured;
	for (int Row = Pos->Row - 1, Col = Pos->Col - 1;
		Row >= 0 && Col >= 0;
		--Row, --Col)
	{
		position CheckPosition = { Row, Col };
		piece_color Piece = Board->State[CheckPosition.Row][CheckPosition.Col];
		if (Piece == OpponentColor)
		{
			Captured.push_back(CheckPosition);
			++OpponentCount;
		}
		else if (Piece == Player->Color)
		{
			++PlayerCount;
			break;
		}
		else if (Piece == COLOR_EMPTY)
		{
			Captured.clear();
			break;
		}
	}

	bool Result = OpponentCount > 0 && PlayerCount == 1;
	if (Result)
	{
		for (int Index = 0; Index < Captured.size(); ++Index)
			Pos->Captured.push_back(Captured[Index]);
	}

	return Result;
}

bool CheckSouthEast(board *Board, position *Pos, player *Player)
{
	piece_color OpponentColor = Player->Color == COLOR_LIGHT ? COLOR_DARK : COLOR_LIGHT;

	int PlayerCount = 0;
	int OpponentCount = 0;

	if (Pos->Row + 2 >= BOARD_SIZE || Pos->Col + 2 >= BOARD_SIZE)
		return false;

	std::vector<position> Captured;
	for (int Row = Pos->Row + 1, Col = Pos->Col + 1;
		Row < BOARD_SIZE && Col < BOARD_SIZE;
		++Row, ++Col)
	{
		position CheckPosition = { Row, Col };
		piece_color Piece = Board->State[CheckPosition.Row][CheckPosition.Col];
		if (Piece == OpponentColor)
		{
			Captured.push_back(CheckPosition);
			++OpponentCount;
		}
		else if (Piece == Player->Color)
		{
			++PlayerCount;
			break;
		}
		else if (Piece == COLOR_EMPTY)
		{
			Captured.clear();
			break;
		}
	}

	bool Result = OpponentCount > 0 && PlayerCount == 1;
	if (Result)
	{
		for (int Index = 0; Index < Captured.size(); ++Index)
			Pos->Captured.push_back(Captured[Index]);
	}

	return Result;
}

bool CheckSouthWest(board *Board, position *Pos, player *Player)
{
	piece_color OpponentColor = Player->Color == COLOR_LIGHT ? COLOR_DARK : COLOR_LIGHT;

	int PlayerCount = 0;
	int OpponentCount = 0;

	if (Pos->Row + 2 >= BOARD_SIZE || Pos->Col - 2 < 0)
		return false;

	std::vector<position> Captured;
	for (int Row = Pos->Row + 1, Col = Pos->Col - 1;
		Row < BOARD_SIZE && Col >= 0;
		++Row, --Col)
	{
		position CheckPosition = { Row, Col };
		piece_color Piece = Board->State[CheckPosition.Row][CheckPosition.Col];
		if (Piece == OpponentColor)
		{
			Captured.push_back(CheckPosition);
			++OpponentCount;
		}
		else if (Piece == Player->Color)
		{
			++PlayerCount;
			break;
		}
		else if (Piece == COLOR_EMPTY)
		{
			Captured.clear();
			break;
		}
	}

	bool Result = OpponentCount > 0 && PlayerCount == 1;
	if (Result)
	{
		for (int Index = 0; Index < Captured.size(); ++Index)
			Pos->Captured.push_back(Captured[Index]);
	}

	return Result;
}
