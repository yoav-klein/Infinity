
#include "checkers.h"

namespace checkers
{

Board::Board()
	: m_turn(WHITE), m_status(PLAY)
{
	InitBoard();
}

void Board::SetStatus(enum Status status)
{
	m_status = status;
}

void Board::SwitchTurn()
{
	if(m_turn == BLACK)
	{
		m_turn = WHITE;
	}
	else
	{
		m_turn = BLACK;
	}
}

Piece* Board::GetPieceInPosition(Position pos) const
{
	return m_board[pos.GetX()][pos.GetY()].GetPiece();
}

void Board::RemovePiece(Piece *piece)
{
	Position pos = piece->GetPosition();
	enum Color color = piece->GetColor();
	
	m_board[pos.GetX()][pos.GetY()].TakePiece(); // remove from board
	m_teams[color].remove_if(IsSamePredicator(piece)); // remove from list
}

enum Color Board::Select(int x, int y)
{
	if(PLAY == m_status)
	{
		if(!PickPiece(Position(x, y)))
		{
			std::cerr << "Not Yours!" << std::endl;
		}
	}
	else
	{
		if(!MoveOrPickAnotherPiece(Position(x, y)))
		{
			std::cerr << "Can't move there!" << std::endl;
		}
	}

	if(m_teams[WHITE].empty())
	{
		return BLACK;
	}
	if(m_teams[BLACK].empty())
	{
		return WHITE;
	}
	return NONE;
}

enum Color Board::TileContent(Position pos) const
{
	if(m_board[pos.GetX()][pos.GetY()].IsTaken())
	{
		return m_board[pos.GetX()][pos.GetY()].GetPiece()->GetColor();
	}
	return NONE;

}

bool Board::PickPiece(Position pos)
{
	std::vector<Piece*> available_pieces = GetMoveablePieces();
	
	for(std::size_t i = 0; i < available_pieces.size(); ++i)
	{
		if(available_pieces[i]->GetPosition() == pos)
		{
			m_selected_piece = available_pieces[i];
			m_status = SELECTED;
			return true;
		}
	}
	return false;
}

std::vector<Piece*> Board::GetMoveablePieces()
{
	std::vector<Piece*> res;
	bool can_eat = false;
	
	for(std::list<Piece>::iterator iter = m_teams[m_turn].begin(); 
	iter != m_teams[m_turn].end(); ++iter)
	{
		if(iter->IsEating())
		{
			res.push_back(&*iter);
			can_eat = true;
		}
	}
	if(false == can_eat)
	{
		for(std::list<Piece>::iterator iter = m_teams[m_turn].begin(); 
		iter != m_teams[m_turn].end(); ++iter)
		{
			res.push_back(&*iter);	
		}
	}
	return res;
}

bool Board::MoveOrPickAnotherPiece(Position pos)
{
	if(PickPiece(pos))
	{
		return true;
	}
	
	std::vector<std::tr1::shared_ptr<Move> > moves = m_selected_piece->GetMoves();
	
	for(std::size_t i = 0; i < moves.size(); ++i)
	{
		if(pos == moves[i]->WhereTo())
		{
			moves[i]->Activate();
			return true;
		}
	}
	
	return false;
}

void Board::SetPieceInPlace(Piece* piece, Position where)
{
	m_board[where.GetX()][where.GetY()].SetPiece(piece);
}

Piece* Board::GetSelectedPiece() const
{
	return m_selected_piece;
}

void Board::MovePiece(Piece* piece, Position where_to)
{
	Position curr_pos = piece->GetPosition();
	m_board[curr_pos.GetX()][curr_pos.GetY()].TakePiece();
	SetPieceInPlace(piece, where_to);
}

enum Board::Status Board::GetStatus() const
{
	return m_status;
}

void Board::InitBoard()
{
	Position white_positions[12] = 
	{ 
	Position(7, 0),
	Position(7, 2),
	Position(7, 4),
	Position(7, 6),
	Position(6, 1),
	Position(6, 3),
	Position(6, 5),
	Position(6, 7),
	Position(5, 0),
	Position(5, 2),
	Position(5, 4),
	Position(5, 6)
	};
	
	Position black_positions[12] =
	{ 
	Position(2, 1),
	Position(2, 3),
	Position(2, 5),
	Position(2, 7),
	Position(1, 0),
	Position(1, 2),
	Position(1, 4),
	Position(1, 6),
	Position(0, 1),
	Position(0, 3),
	Position(0, 5),
	Position(0, 7),
	};
	
	for(int i = 0; i < 12; ++i)
	{
		m_teams[WHITE].push_back(Piece(white_positions[i], *this, WHITE));
		m_teams[BLACK].push_back(Piece(black_positions[i], *this, BLACK));
	}
	
	for(std::list<Piece>::iterator iter = m_teams[WHITE].begin(); iter != m_teams[WHITE].end(); ++iter)
	{
		iter->SetupPiece();
	}
	
	for(std::list<Piece>::iterator iter = m_teams[BLACK].begin(); iter != m_teams[BLACK].end(); ++iter)
	{
		iter->SetupPiece();
	}
	
}


} // namespace checkers
