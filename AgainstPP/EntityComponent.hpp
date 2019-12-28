#pragma once
class EntityComponent
{
public:
	EntityComponent () {}
	~EntityComponent () {}

	virtual void Update () = 0;
	virtual void Draw () = 0;
};

