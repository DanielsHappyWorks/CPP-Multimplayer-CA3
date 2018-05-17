enum ECatControlType
{
	ESCT_Human,
	ESCT_AI
};

class PanzerServer : public Panzer
{
public:
	static GameObjectPtr	StaticCreate() { return NetworkManagerServer::sInstance->RegisterAndReturn( new PanzerServer() ); }
	virtual void HandleDying() override;

	virtual void Update();

	void SetCatControlType( ECatControlType inCatControlType ) { mCatControlType = inCatControlType; }

	void TakeDamage( int inDamagingPlayerId );
	void TakeDamage();

protected:
	PanzerServer();

private:

	void HandleShooting();

	ECatControlType	mCatControlType;


	float		mTimeOfNextShot;
	float		mTimeBetweenShots;

};