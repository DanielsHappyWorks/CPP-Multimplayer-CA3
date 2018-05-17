#include "PanzerPCH.h"

std::unique_ptr< ScoreBoardManager >	ScoreBoardManager::sInstance;


void ScoreBoardManager::StaticInit()
{
	sInstance.reset( new ScoreBoardManager() );
}

ScoreBoardManager::ScoreBoardManager()
{
	mDefaultColors.push_back( Colors::LightYellow );
	mDefaultColors.push_back( Colors::LightBlue );
	mDefaultColors.push_back( Colors::LightPink );
	mDefaultColors.push_back( Colors::LightGreen );
}

ScoreBoardManager::Entry::Entry( uint32_t inPlayerId, const string& inPlayerName, const Vector3& inColor, int totalScore) :
mPlayerId( inPlayerId ),
mPlayerName( inPlayerName ),
mColor( inColor ),
mTotalScore( totalScore )
{
	SetSessionScore( 0 );
}

void ScoreBoardManager::Entry::SetSessionScore( int32_t inScore )
{
	mSessionScore = inScore;

	char	buffer[ 256 ];
	snprintf( buffer, 256, "%-15s%-15d%-15d", mPlayerName.c_str(), mSessionScore, mTotalScore);
	mFormattedNameScore = string( buffer );

}

void ScoreBoardManager::Entry::SetTotalScore(int32_t inScore)
{
	mTotalScore = inScore;
}


ScoreBoardManager::Entry* ScoreBoardManager::GetEntry( uint32_t inPlayerId )
{
	for ( Entry &entry: mEntries )
	{
		if( entry.GetPlayerId() == inPlayerId )
		{
			return &entry;
		}
	}

	return nullptr;
}

bool ScoreBoardManager::RemoveEntry( uint32_t inPlayerId )
{
	for( auto eIt = mEntries.begin(), endIt = mEntries.end(); eIt != endIt; ++eIt )
	{
		if( ( *eIt ).GetPlayerId() == inPlayerId )
		{
			mEntries.erase( eIt );
			return true;
		}
	}

	return false;
}

void ScoreBoardManager::AddEntry( uint32_t inPlayerId, const string& inPlayerName, int totalScore )
{
	//if this player id exists already, remove it first- it would be crazy to have two of the same id
	RemoveEntry( inPlayerId );
	
	mEntries.emplace_back( inPlayerId, inPlayerName, mDefaultColors[ inPlayerId % mDefaultColors.size() ], totalScore);
}

void ScoreBoardManager::IncSessionScore( uint32_t inPlayerId, int inAmount )
{
	Entry* entry = GetEntry( inPlayerId );
	if( entry )
	{
		entry->SetSessionScore( entry->GetSessionScore() + inAmount );
	}
}

void ScoreBoardManager::IncTotalScore(uint32_t inPlayerId, int inAmount)
{
	Entry* entry = GetEntry(inPlayerId);
	if (entry)
	{
		entry->SetTotalScore(entry->GetTotalScore() + inAmount);
	}
}

void ScoreBoardManager::SetSessionWinner(uint32_t inPlayerId) {
	for (Entry& entry : mEntries)
	{
		entry.SetSessionScore(0);
	}

	IncTotalScore(inPlayerId, 1);
}

bool ScoreBoardManager::Write( OutputMemoryBitStream& inOutputStream ) const
{
	int entryCount = mEntries.size();
	
	//we don't know our player names, so it's hard to check for remaining space in the packet...
	//not really a concern now though
	inOutputStream.Write( entryCount );
	for( const Entry& entry: mEntries )
	{
		entry.Write( inOutputStream );
	}
	
	return true;
}



bool ScoreBoardManager::Read( InputMemoryBitStream& inInputStream )
{
	int entryCount;
	inInputStream.Read( entryCount );
	//just replace everything that's here, it don't matter...
	mEntries.resize( entryCount );
	for( Entry& entry: mEntries )
	{
		entry.Read( inInputStream );
	}

	return true;
}


bool ScoreBoardManager::Entry::Write( OutputMemoryBitStream& inOutputStream ) const
{
	bool didSucceed = true;

	inOutputStream.Write( mColor );
	inOutputStream.Write( mPlayerId );
	inOutputStream.Write( mPlayerName );
	inOutputStream.Write( mSessionScore );
	inOutputStream.Write( mTotalScore );

	return didSucceed;
}

bool ScoreBoardManager::Entry::Read( InputMemoryBitStream& inInputStream )
{
	bool didSucceed = true;

	inInputStream.Read( mColor );
	inInputStream.Read( mPlayerId );

	inInputStream.Read( mPlayerName );

	int scoreSess;
	inInputStream.Read(scoreSess);
	int scoreTot;
	inInputStream.Read(scoreTot);
	if( didSucceed )
	{
		SetSessionScore(scoreSess);
		SetTotalScore(scoreTot);
	}


	return didSucceed;
}



