class MouseClient : public Mouse
{
public:
	static	GameObjectPtr	StaticCreate()		{ return GameObjectPtr( new MouseClient() ); }
	virtual void	Read(InputMemoryBitStream& inInputStream) override;

protected:
	MouseClient();

private:

	SpriteComponentPtr	mSpriteComponent;
};