#pragma once


class IScene
{
public:
	// Initialisiert die Scene
	virtual void Initialize() = 0;


	// Updated die Scene
	virtual void Update(float p_DeltaTime) = 0;

	// Rendert die Scene
	virtual void Render() = 0;
};