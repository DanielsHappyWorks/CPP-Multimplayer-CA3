


class NetworkManagerClient : public NetworkManager
{
public:
	enum NetworkClientState
	{
		NCS_Uninitialized,
		NCS_SayingHello,
		NCS_Welcomed,
		NCS_Menu,
		NCS_ErrorRevieved
	};
	static NetworkManagerClient*	sInstance;

	static	void	StaticInit( const SocketAddress& inServerAddress, const string& inName, const string& inPass);

			void	SendOutgoingPackets();

	virtual void	ProcessPacket( InputMemoryBitStream& inInputStream, const SocketAddress& inFromAddress ) override;

			const	WeightedTimedMovingAverage&		GetAvgRoundTripTime()	const	{ return mAvgRoundTripTime; }
			float									GetRoundTripTime()		const	{ return mAvgRoundTripTime.GetValue(); }
			int		GetPlayerId()											const	{ return mPlayerId; }
			float	GetLastMoveProcessedByServerTimestamp()					const	{ return mLastMoveProcessedByServerTimestamp; }
			bool RecievedError();
			string mError;
			void SetState(NetworkClientState s) { mState = s; }
			NetworkClientState GetState() { return mState; }
private:
			NetworkManagerClient();
			void Init( const SocketAddress& inServerAddress, const string& inName, const string& inPass);

			void	UpdateSayingHello();
			void	SendHelloPacket();

			void	HandleWelcomePacket( InputMemoryBitStream& inInputStream );
			void	HandleStatePacket( InputMemoryBitStream& inInputStream );
			void	ReadLastMoveProcessedOnServerTimestamp( InputMemoryBitStream& inInputStream );

			void	HandleGameObjectState( InputMemoryBitStream& inInputStream );
			void	HandleScoreBoardState( InputMemoryBitStream& inInputStream );
			void	HandleErrorPacket(InputMemoryBitStream& inInputStream);

			void	UpdateSendingInputPacket();
			void	SendInputPacket();

			void	DestroyGameObjectsInMap( const IntToGameObjectMap& inObjectsToDestroy );
	
	DeliveryNotificationManager	mDeliveryNotificationManager;
	ReplicationManagerClient	mReplicationManagerClient;

	SocketAddress		mServerAddress;

	NetworkClientState	mState;

	float				mTimeOfLastHello;
	float				mTimeOfLastInputPacket;

	string				mName, mPassword;
	int					mPlayerId;

	float				mLastMoveProcessedByServerTimestamp;

	WeightedTimedMovingAverage	mAvgRoundTripTime;
	float						mLastRoundTripTime;

};