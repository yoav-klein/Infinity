
#include "checkers.h"

namespace checkers
{


enum Color who_won = NONE;
Game g_game(500);

void DrawFunction()
{
	g_game.Draw();
}

int TimerFunction()
{
	if(NONE != who_won)
	{
		sleep(2);
	}
	
	return 1;
}

int MouseFunction(int button, int state, int x, int y)
{
	int board_size = g_game.GetBoardSize();
	int tile_x = x / (board_size / 8);
	int tile_y = y / (board_size / 8);
	
	(void)state;	
	if(button == MOUSE_LEFT)
	{
		who_won = g_game.SelectTile(tile_y, tile_x);
		return 1;
	}
	return 0;
} 


// ---------- Game --------------
Game::Game(int board_size)
	:  m_drawer(&m_board_holder, board_size), m_board_size(board_size)
	{ }

void Game::NewGame()
{
	m_board_holder.NewBoard();
}

void Game::Play()
{
	m_drawer.StartDrawer();
}

enum Color Game::SelectTile(int x, int y)
{
	return m_board_holder->Select(x, y);
}

void Game::Draw()
{
	if(who_won == NONE)
	{
		m_drawer.DrawBoard();
	}
	else
	{
		m_drawer.AnnounceWinner(who_won);
		
		g_game.NewGame();
	}	
}

int Game::GetBoardSize()
{
	return m_board_size;
}

/******** Drawer ************/

void Drawer::StartDrawer()
{
	DrawInit(0, NULL, m_board_size, m_board_size, DrawFunction);
	
	DrawSetMouseFunc(MouseFunction);
	DrawSetTimerFunc(TimerFunction, 100);
	
	DrawMainLoop();
}

void Drawer::DrawBoard()
{
	std::size_t i = 0;
	std::size_t j = 0;
	
	int cube_size = m_board_size / 8;
	
	for(i = 0; i < 8; ++i)
	{
		int from_i = cube_size * i;
		int to_i = from_i + cube_size;
		for(j = 0; j < 8; ++j)
		{
			int from_j = j * cube_size;
			int to_j = from_j + cube_size;
			
			if(((i % 2 == 0) && (j % 2 == 0)) || ((i % 2 != 0) && (j % 2 != 0)))
			{
				DrawWhiteTile(from_i, from_j, to_i, to_j);
			}
			else
			{
				Position pos(i, j);
				DrawContent(pos, from_i, from_j, to_i, to_j);
			}
		}
	}
}

void Drawer::DrawContent(Position pos, int from_i, int from_j, int to_i, int to_j)
{	
	
	enum Board::Status status = (*m_board)->GetStatus();
	
	Piece* selected = (*m_board)->GetSelectedPiece();
	enum Color content = (*m_board)->TileContent(pos);
	if(status == Board::SELECTED && (selected->GetPosition() == pos))
	{
		DrawSelected(from_i, from_j, to_i, to_j);
	}	
	
	switch (content)
	{
		case NONE:
			break;
		case WHITE:
			if((*m_board)->GetPieceInPosition(pos)->IsQueen())
			{
				DrawWhiteQueen(from_i, from_j, to_i, to_j);
			}
			else
			{
				DrawWhitePiece(from_i, from_j, to_i, to_j);
			}
			break;
		case BLACK:
			if((*m_board)->GetPieceInPosition(pos)->IsQueen())
			{
				DrawBlackQueen(from_i, from_j, to_i, to_j);
			}
			else
			{
				DrawBlackPiece(from_i, from_j, to_i, to_j);
			}
			break;
	}
			
}

void Drawer::DrawSelected(int from_i, int from_j, int to_i, int to_j)
{
	DrawPolygon(COLOR_GREEN | COLOR_FILL, 4, from_j, from_i, from_j, to_i, to_j, to_i, to_j, from_i);
}


void Drawer::DrawWhiteTile(int from_i, int from_j, int to_i, int to_j)
{
	DrawPolygon(COLOR_WHITE | COLOR_FILL, 4, from_j, from_i, from_j, to_i, to_j, to_i, to_j, from_i);
}

void Drawer::DrawWhitePiece(int from_i, int from_j, int to_i, int to_j)
{
	int average_x = (from_j + to_j) / 2;
	int average_y = (from_i + to_i) / 2; 
	DrawCircle(COLOR_WHITE | COLOR_FILL ,average_x, average_y, m_radius);
}

void Drawer::DrawWhiteQueen(int from_i, int from_j, int to_i, int to_j)
{
	int average_x = (from_j + to_j) / 2;
	int average_y = (from_i + to_i) / 2; 
	DrawCircle(COLOR_CYAN | COLOR_FILL ,average_x, average_y, m_radius);
}
	
void Drawer::DrawBlackPiece(int from_i, int from_j, int to_i, int to_j)
{
	int average_x = (from_j + to_j) / 2;
	int average_y = (from_i + to_i) / 2; 
	DrawCircle(COLOR_RED | COLOR_FILL ,average_x, average_y, m_radius);
}

void Drawer::DrawBlackQueen(int from_i, int from_j, int to_i, int to_j)
{
	int average_x = (from_j + to_j) / 2;
	int average_y = (from_i + to_i) / 2; 
	DrawCircle(COLOR_BLUE | COLOR_FILL ,average_x, average_y, m_radius);
}


void Drawer::AnnounceWinner(enum Color winner)
{
	DrawPolygon(COLOR_RED | COLOR_FILL, 4, 0, 0, m_board_size, 0, m_board_size, m_board_size, 0, m_board_size);
	DrawPolygon(COLOR_YELLOW | COLOR_FILL, 4, 10, 10, m_board_size - 10, 10, m_board_size - 10, 
	m_board_size -10, 10, m_board_size - 10);
	
	if(winner == BLACK)
	{
		DrawCircle(COLOR_BLACK | COLOR_FILL, m_board_size / 2, m_board_size / 2, 20);
	}
	else
	{
		DrawCircle(COLOR_WHITE | COLOR_FILL, m_board_size / 2, m_board_size / 2, 20);
	}
}

} // namespace checkers


using namespace checkers;

int main()
{
	
	g_game.Play();
	
	
	return 0;
}

