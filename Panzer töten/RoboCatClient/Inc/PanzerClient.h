class PanzerClient : public Panzer
{
public:
	static	GameObjectPtr	StaticCreate()		{ return GameObjectPtr( new PanzerClient() ); }

	virtual void Update();
	virtual void	HandleDying() override;

	virtual void	Read( InputMemoryBitStream& inInputStream ) override;

	void DoClientSidePredictionAfterReplicationForLocalCat( uint32_t inReadState );
	void DoClientSidePredictionAfterReplicationForRemoteCat( uint32_t inReadState );



protected:
	PanzerClient();


private:

	void InterpolateClientSidePrediction( float inOldRotation, const Vector3& inOldLocation, const Vector3& inOldVelocity, bool inIsForRemoteCat );

	float				mTimeLocationBecameOutOfSync;
	float				mTimeVelocityBecameOutOfSync;
	
	SpriteComponentPtr	mSpriteComponent;
};