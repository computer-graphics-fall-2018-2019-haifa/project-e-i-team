#pragma once

class Light {
public:
	Light(float La, float Ld, float Ls);
	~Light();
	float La, Ld, Ls;
};
