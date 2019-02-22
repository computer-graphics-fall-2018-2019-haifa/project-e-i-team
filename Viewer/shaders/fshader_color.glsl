#version 330 core

float random (vec4 st) {
    return fract(sin(dot(st.xyzw,vec4(12.9898,78.233,78.233,78.233)))*43758.5453123);
}

struct Material
{
	sampler2D textureMap;

	vec4 AmbientColor;
	vec4 DiffuseColor;
	vec4 SpecualrColor;
    vec4 modelColor;

	float Ka;
	float Kd;
	float Ks;
	float Ksh;
};

// We set this field's properties from the C++ code
uniform Material material;
uniform vec4 ambientColorScene;
uniform vec4 lightPos1;
uniform vec4 lightPos2;
uniform vec4 lightColor1;
uniform vec4 lightColor2;
uniform int needTexture;
uniform int needToon;
uniform int needNoiseBump;
uniform int numColors;

// Inputs from vertex shader (after interpolation was applied)
in vec4 fragPos;
in vec4 fragNormal;
in vec2 fragTexCoords;

// The final color of the fragment (pixel)
out vec4 frag_color;

void main()
{
    vec4 lightPos[2];
    lightPos[0] = lightPos1;
    lightPos[1] = lightPos2;
    vec4 lightColor[2];
    lightColor[0] = lightColor1;
    lightColor[1] = lightColor2;

	vec4 AmbientColor = material.AmbientColor;
	vec4 DiffuseColor = material.DiffuseColor;
	vec4 SpecualrColor = material.SpecualrColor;

	//texturing if any
	if (needTexture == 1) {
		vec3 textureColor = vec3(texture(material.textureMap, fragTexCoords));
		AmbientColor = vec4(textureColor, 1.0f);
		DiffuseColor = vec4(textureColor, 1.0f);
		SpecualrColor = vec4(textureColor, 1.0f);
	}

	vec3 N = normalize(fragNormal.xyz / fragNormal.w);
	vec3 V = normalize(fragPos.xyz / fragPos.w);
	// ambient is only needed once
    vec4 ambColor = material.Ka * AmbientColor * material.modelColor;
	vec4 IA = clamp(vec4(ambColor.x * ambientColorScene.x, ambColor.y * ambientColorScene.y, ambColor.z * ambientColorScene.z, 1.0f), 0.0f, 1.0f);

	vec4 ID = vec4(0.0f);
	vec4 IS = vec4(0.0f);

    // until 2 lights boundary
	for (int i = 0; i < 2; i++) {
		vec4 lightColor = lightColor[i];
		vec3 pos = lightPos[i].xyz / lightPos[i].w;

		vec3 L = normalize(pos - (fragPos.xyz / fragPos.w));
        float n;
        if(needNoiseBump == 1){
            float a = 0.5; //wave height
            float b = 10.0; //wave frequency
            float x = fragTexCoords.s;
            float y = fragTexCoords.t;
            float v1 = a*(b*cos(b*x))*sin(b*y);
            float v2 = a*sin(b*x)*(b*cos(b*y));
            N = N + max(dot(vec3(v1,v2,1.0),L), 0.0);
        }
        vec3 R = normalize(reflect(-L, N));
		float LN = max(dot(L,N), 0.0f);

		vec4 diffColor = material.Kd * LN * DiffuseColor;
		ID = ID + clamp(vec4(diffColor.x * lightColor.x, diffColor.y * lightColor.y, diffColor.z * lightColor.z, 1.0f), 0.0f, 1.0f);

		float RV = pow(max(dot(R, V), 0.0f), material.Ksh);
		vec4 specColor = material.Ks * RV * SpecualrColor;
		IS = IS + clamp(vec4(specColor.x * lightColor.x, specColor.y * lightColor.y, specColor.z * lightColor.z, 1.0f), 0.0f, 1.0f);
	}

    vec4 illum = IA + ID + IS;
    frag_color = clamp(illum, 0.0f, 1.0f);

    // number of colors is about toon shading issue
    if(needToon == 1){
	    frag_color = (round(frag_color * 255 / numColors) * numColors) / 255;
    }
}