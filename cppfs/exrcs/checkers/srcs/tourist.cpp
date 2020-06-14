#include "checkers.h"

namespace checkers
{

std::vector<std::tr1::shared_ptr<Move> > TouristRegular::GetMoves()
{	
	std::vector<std::tr1::shared_ptr<Move> > moves;
	std::vector<std::tr1::shared_ptr<Move> > eats;
	
	std::vector<Direction> directions;
	enum Color my_color = m_piece.GetColor();
	Piece* candidate_to_eat;
	
	if(my_color == WHITE)
	{
		directions.push_back(m_directions[0]);
		directions.push_back(m_directions[1]);
	}
	else
	{
		directions.push_back(m_directions[2]);
		directions.push_back(m_directions[3]);
	}
	
	
	for(size_t i = 0; i < directions.size(); ++ i)
	{
		Position current_pos = m_piece.GetPosition();
		current_pos += directions[i];
		
		if(current_pos.IsValidPosition())
		{
			enum Color tile_content = m_board.TileContent(current_pos);
			if(NONE == tile_content)
			{
				moves.push_back(std::tr1::shared_ptr<Move>(new Move(m_board, m_piece, current_pos)));
			}
			else if(my_color != tile_content)
			{
				candidate_to_eat = m_board.GetPieceInPosition(current_pos);
				current_pos += directions[i];
				if(current_pos.IsValidPosition() && NONE == m_board.TileContent(current_pos))
				{
					eats.push_back(std::tr1::shared_ptr<Move>(new Eat(m_board, m_piece, current_pos, *candidate_to_eat)));
				}
			}
		}
	}
	
	if(eats.empty())
	{
		return moves;
	}
	return eats;
	
}

bool TouristRegular:: IsQueen() const
{
	return false;
}

std::vector<std::tr1::shared_ptr<Move> > TouristQueen::GetMoves()
{
	std::vector<std::tr1::shared_ptr<Move> > moves;
	std::vector<std::tr1::shared_ptr<Move> > eats;
	enum Color my_color = m_piece.GetColor();
	Piece* candidate_to_eat;
	
	for(size_t i = 0; i < m_directions.size(); ++i)
	{	
		Position current_pos = m_piece.GetPosition();
		
		current_pos += m_directions[i];
		while(current_pos.IsValidPosition() && NONE == m_board.TileContent(current_pos))
		{
			moves.push_back(std::tr1::shared_ptr<Move>(new Move(m_board, m_piece, current_pos)));
			current_pos += m_directions[i];
		}
		
		if(current_pos.IsValidPosition() && my_color != m_board.TileContent(current_pos))
		{
			candidate_to_eat = m_board.GetPieceInPosition(current_pos);
			current_pos += m_directions[i];
			if(current_pos.IsValidPosition() && NONE == m_board.TileContent(current_pos))
			{
				eats.push_back(std::tr1::shared_ptr<Move>(new Eat(m_board, m_piece, current_pos, *candidate_to_eat)));
			}
		}
	}
	
	if(eats.empty())
	{
		return moves;
	}
	return eats;
	
}

bool TouristQueen::IsQueen() const
{
	return true;
}

/*------------- Move ------------- */

Move::Move(Board& board, Piece& piece, Position where_to)
		: m_board(board), m_piece(piece), m_where_to(where_to) { }
	
Position Move::WhereTo()
{
	return m_where_to;
}

bool Move::IsEating()
{
	return false;
}

void Move::Activate()
{
	m_piece.MovePiece(m_where_to);

	m_board.SwitchTurn();
	
	
	// check if queen
	if(m_where_to.GetX() == m_piece.GetColor() * 7)
	{
		m_piece.BecomeQueen();
	}
	
	m_board.SetStatus(Board::PLAY);
}

/********** Eat ***************/
Eat::Eat(Board& board, Piece& piece, Position where_to, Piece& who)
	: Move(board, piece, where_to), m_who(who) { }
	
Piece& Eat::EatingWho()
{
	return m_who;
}

void Eat::Activate()
{
	m_who.Die();
	
	Move::Activate();
	
	if(m_piece.IsEating())
	{
		m_board.SwitchTurn();
	}
}

bool Eat::IsEating()
{
	return true;
}


} // namepsace checkers


