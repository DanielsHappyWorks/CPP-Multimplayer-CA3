class ScoreBoardManager
{
public:

	static void StaticInit();
	static std::unique_ptr< ScoreBoardManager >	sInstance;

	class Entry
	{
	public:
		Entry() {};

		Entry( uint32_t inPlayerID, const string& inPlayerName, const Vector3& inColor, int totalScore );

		const Vector3&	GetColor()		const	{ return mColor; }
		uint32_t		GetPlayerId()	const	{ return mPlayerId; }
		const string&	GetPlayerName()	const	{ return mPlayerName; }
		const string&	GetFormattedNameScore()	const	{ return mFormattedNameScore; }
		int				GetSessionScore()		const	{ return mSessionScore; }
		int				GetTotalScore()		const { return mTotalScore; }

		void			SetSessionScore( int inScore );
		void			SetTotalScore(int inScore);

		bool			Write( OutputMemoryBitStream& inOutputStream ) const;
		bool			Read( InputMemoryBitStream& inInputStream );
		static uint32_t	GetSerializedSize();
	private:
		Vector3			mColor;
		
		uint32_t		mPlayerId;
		string			mPlayerName;
		
		int				mSessionScore;
		int				mTotalScore;

		string			mFormattedNameScore;
	};

	Entry*	GetEntry( uint32_t inPlayerId );
	bool	RemoveEntry( uint32_t inPlayerId );
	void	AddEntry( uint32_t inPlayerId, const string& inPlayerName, int totalScore);
	void	IncSessionScore( uint32_t inPlayerId, int inAmount );
	void	IncTotalScore(uint32_t inPlayerId, int inAmount);
	void	SetSessionWinner(uint32_t inPlayerId);

	bool	Write( OutputMemoryBitStream& inOutputStream ) const;
	bool	Read( InputMemoryBitStream& inInputStream );

	const vector< Entry >&	GetEntries()	const	{ return mEntries; }

private:

	ScoreBoardManager();

	vector< Entry >	mEntries;

	vector< Vector3 >	mDefaultColors;


};