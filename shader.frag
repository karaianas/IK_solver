#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
//in float sampleExtraOutput;

/*
 My input
 */
in vec3 P;
in vec3 N;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objColor;

void directional();

float PI = 3.141593;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;

void main()
{
    
	directional();
   
}

void directional()
{
    vec3 L = normalize(lightPos);
    vec3 norm = normalize(N);
    
    // diffuse
    float k_d = 0.6f;
    float inProduct = max(dot(L, norm), 0);
    vec3 diff = k_d * inProduct * lightColor;
    
    // specular ref. Phong model
    float k_s = 0.9f;
    vec3 V = normalize(viewPos - P);
    vec3 R = reflect(-L, norm);
    float alpha = pow(max(dot(V, R), 0.0), 16);// shininess
    vec3 spec = k_s * alpha * lightColor;
    
    // ambient
    vec3 ambi = vec3(0.1f, 0.2f, 0.4f);
	//vec3 ambi = vec3(0.1f, 0.1f, 0.1f);
    
    // combined
    vec3 result = (diff + spec) * objColor + ambi * objColor;
    color = vec4(result, 1.0f);
    
}
